#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include "args.h"
#include "utils.h"
#include "queue.h"

int pl = 1, closed = 0, limited_places = 0, limited_threads = 0;
pthread_mutex_t server_mut=PTHREAD_MUTEX_INITIALIZER;
sem_t sem_places;
sem_t sem_threads;
queue q;

void *thr_func(void *arg){
    //Recieving request
    char private_fifo[MAX_LEN];
    int i, dur, pid, server_pid = getpid();
    long int tid, server_tid = pthread_self();

    sscanf((char *) arg,"[ %d, %d, %ld, %d, -1 ]\n",&i,&pid,&tid,&dur);
    logRegister(i, server_pid, server_tid, dur, -1, "RECVD");    //Request received

    sprintf(private_fifo, "/tmp/%d.%ld", pid, tid);

    //Opens private FIFO for writing and checking error
    int fd_private;
    if((fd_private = open(private_fifo, O_WRONLY)) == -1){
        logRegister(i, server_pid, server_tid, dur, -1, "GAVUP");
        perror("Can't open private fifo WRONLY!");

        if (limited_threads) {
            sem_post(&sem_threads);
        }

        return NULL;
    }

    int place = pl;
    int entered = 0;

    //Replying to client
    char client_reply[MAX_LEN];
    if(!closed){    //Request accepted
        entered = 1;

        if (limited_places){
            sem_wait(&sem_places);
            pthread_mutex_lock(&server_mut);
            place = usePlace(&q);
            pthread_mutex_unlock(&server_mut);
        }
        else{
            pthread_mutex_lock(&server_mut);
            pl++;
            pthread_mutex_unlock(&server_mut);
        }

        sprintf(client_reply, "[ %d, %d, %ld, %d, %d ]\n",i, server_pid, server_tid, dur, place);
        logRegister(i, server_pid, server_tid, dur, place, "ENTER");
    }
    else{    //WC is closed
        sprintf(client_reply, "[ %d, %d, %ld, %d, %d ]\n",i, server_pid, server_tid, -1, -1);
        logRegister(i, server_pid, server_tid, dur, -1, "2LATE");
        
        if (limited_threads){
            sem_post(&sem_threads);
        }
    }

    if(write(fd_private, &client_reply, MAX_LEN)<0) {
        logRegister(i, server_pid, server_tid, dur, -1, "GAVUP");
        perror("Can't write to private fifo!");

        if (limited_places){
            pthread_mutex_lock(&server_mut);
            makeAvailable(&q, place);
            pthread_mutex_unlock(&server_mut);
            sem_post(&sem_places); 
        }
        if (limited_threads){
            sem_post(&sem_threads);
        }

        return NULL;
    }

    if(entered){
        usleep(dur*1000);
        logRegister(i, server_pid, server_tid, dur, place, "TIMUP");
        
        if (limited_places) { 
            pthread_mutex_lock(&server_mut);
            makeAvailable(&q, place);
            pthread_mutex_unlock(&server_mut);
            sem_post(&sem_places); 
        }
        if (limited_threads) { 
            sem_post(&sem_threads); 
        }
    }
    close(fd_private);    //Closes private FIFO
    return NULL;
}


int main(int argc, char *argv[], char *envp[]){
    server_args args = get_server_args(argc, argv);

    if (args.nplaces > 0) limited_places = 1;
    if (args.nthreads > 0) limited_threads = 1;

    getBeginTime();

    if (mkfifo(args.fifoname, 0660) != 0){    //Makes FIFO
        perror("Error, can't create publc fifo!");
        exit(1);
    }

    int fd;
    if((fd=open(args.fifoname, O_RDONLY | O_NONBLOCK)) == -1){   //Opens FIFO for reading
        perror("Can't open public fifo RDONLY");
        if(unlink(args.fifoname) < 0)
            perror("Error can't destroy public fifo!");
        exit(1);
    }
    
    if(limited_places){
        sem_init(&sem_places, 0,  args.nplaces);
        q = createQueue(args.nplaces);
        fillQueue(&q);
    }
    if(limited_threads){
        sem_init(&sem_threads, 0, args.nthreads);
    }

    //Getting client requests
    char msg[MAX_LEN];
    while(getElapsedTime() < args.nsecs){
       if (read(fd, &msg, MAX_LEN) > 0 && msg[0] == '['){
            if (limited_threads){ 
                sem_wait(&sem_threads); 
            }
            pthread_t thread;
            pthread_create(&thread, NULL, thr_func, (void *) msg);
            pthread_detach(thread);
       }
    }

    closed = 1;
    if(unlink(args.fifoname) < 0)    //Destroys FIFO
       perror("Error can't destroy public fifo!");

    //Checking if a client tried to access WC but it closed
    while (read(fd, &msg, MAX_LEN) > 0 && msg[0] == '['){
        if (limited_threads){ 
            sem_wait(&sem_threads); 
        }
        pthread_t thread;
        pthread_create(&thread, NULL, thr_func, (void *) msg);
        pthread_detach(thread);
    }

    close(fd);    //Closes FIFO

    pthread_exit(0);
}