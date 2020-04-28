#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>  
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include "args.h"
#include "utils.h"

int i=0;
pthread_mutex_t client_mut=PTHREAD_MUTEX_INITIALIZER;

void *thr_func(void *arg){
    int client_pid = getpid();
    long int client_tid = pthread_self();
    int duration = (rand() % (100 - 20 + 1)) + 20; //random duration in WC in ms

    //Incrementing request nr
    pthread_mutex_lock(&client_mut);
    i++;
    pthread_mutex_unlock(&client_mut);

    logRegister(i, client_pid, client_tid, duration, -1, "IWANT");

    int fd = open((char *)arg, O_WRONLY);
    if (fd==-1){ 
        logRegister(i, client_pid, client_tid, duration, -1, "CLOSD");
        return NULL;
    }
    printf("FIFO is open write\n");

    //Send request to WC
    char message[MAX_LEN];
    sprintf(message, "[ %d, %d, %ld, %d, %d ]\n", i, client_pid, client_tid, duration, -1);
    write(fd, &message, MAX_LEN);
    close(fd);


    //Reading WC response
    char private_fifo[MAX_LEN];
    int id, pid, pl, dur, fd_private;
    long tid;

    sprintf(private_fifo, "/tmp/%d.%ld", client_pid, client_tid);
    printf("Client pfifo %s\n", private_fifo);
    if (mkfifo(private_fifo, 0660) != 0){   //Makes private FIFO
        printf("Error, can't create private FIFO!\n");
        exit(1);
    }
    else{
        printf("Private FIFO was created!\n");
    }

    if((fd_private=open(private_fifo, O_RDONLY)) != -1){    //Opens private FIFO for reading
        printf("Private FIFO is open read %d\n", fd_private);
    }
    else{
        printf("Can't open FIFO\n");
        logRegister(i, client_pid, client_tid, duration, -1, "FAILD");
        if(unlink(private_fifo) < 0)
            printf("Error can't destroy private FIFO!\n");
        else
            printf("Private FIFO has been destroyed!\n");
        exit(1);
    }

    if (read(fd_private, &message, MAX_LEN)>0) printf(message);    //Reads message from private FIFO
    sscanf(message, "[ %d, %d, %ld, %d, %d ]\n", &id, &pid, &tid, &dur, &pl);   

    if (pl == -1 && dur == -1){    //WC is closing
        logRegister(id, pid, tid, dur, pl, "CLOSD");
    }
    else{    //WC is open
        logRegister(id, pid, tid, dur, pl, "IAMIN");
    }

    close(fd_private);    //Closes private FIFO
    if(unlink(private_fifo) < 0)    //Destroys private FIFO
       printf("Error can't destroy Private FIFO!\n");
    else
        printf("Private FIFO has been destroyed!\n");

    return NULL;
}


int main(int argc, char *argv[], char *envp[]){
    client_args args;

    if(get_client_args(&args, argc, argv)==-1){
        perror("Error getting args!");
        exit(1);
    }

    getBeginTime();

    pthread_t threads[MAX_THREADS];
    int id = 0;

    while(getElapsedTime()*1e-3<args.nsecs){
        pthread_create(&threads[id], NULL, thr_func, args.fifoname);
        pthread_detach(threads[id]);
        id++;
        sleep(1);
    }

    pthread_exit(0);
}