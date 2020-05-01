#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>  
#include <fcntl.h>
#include <pthread.h>
#include "args.h"
#include "utils.h"

struct request {
    char* fifoname;
    int i;
};

int closed = 0;

void *thr_func(void *arg){
    struct request r = *(struct request*) arg;
    int client_pid = getpid();
    long int client_tid = pthread_self();
    int duration = rand() % 100 + 1; //random duration in WC in ms

    int fd = open(r.fifoname, O_WRONLY);
    if (fd==-1){
        closed = 1;
        logRegister(r.i, client_pid, client_tid, duration, -1, "FAILD");
        return NULL;
    }
    printf("FIFO is open write\n");
    logRegister(r.i, client_pid, client_tid, duration, -1, "IWANT");

    //Send request to WC
    char message[MAX_LEN];
    sprintf(message, "[ %d, %d, %ld, %d, %d ]\n", r.i, client_pid, client_tid, duration, -1);
    if (write(fd, &message, MAX_LEN)<0) return NULL;
    close(fd);


    //Reading WC response
    char private_fifo[MAX_LEN];
    int id, pid, pl, dur, fd_private;
    long tid;

    sprintf(private_fifo, "/tmp/%d.%ld", client_pid, client_tid);
    //printf("Client pfifo %s\n", private_fifo);
    if (mkfifo(private_fifo, 0660) != 0){   //Makes private FIFO
        logRegister(r.i, client_pid, client_tid, duration, -1, "FAILD");
        printf("Error, can't create private FIFO!\n");
        return NULL;
    }
    else{
        //printf("Private FIFO was created!\n");
    }

    if((fd_private=open(private_fifo, O_RDONLY)) != -1){    //Opens private FIFO for reading
        //printf("Private FIFO is open read %d\n", fd_private);
    }
    else{
        logRegister(r.i, client_pid, client_tid, duration, -1, "FAILD");
        printf("Can't open FIFO\n");
        if(unlink(private_fifo) < 0)
            printf("Error can't destroy private FIFO!\n");
        else
            printf("Private FIFO has been destroyed!\n");
        return NULL;
    }

    if (read(fd_private, &message, MAX_LEN)>0) //printf(message);    //Reads message from private FIFO
    sscanf(message, "[ %d, %d, %ld, %d, %d ]\n", &id, &pid, &tid, &dur, &pl);   

    if (pl == -1 && dur == -1){    //WC is closing
        logRegister(r.i, client_pid, client_tid, dur, pl, "CLOSD");
    }
    else{    //WC is open
        logRegister(r.i, client_pid, client_tid, dur, pl, "IAMIN");
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
        printf("Error getting args!\n");
        exit(1);
    }

    getBeginTime();

    pthread_t threads[MAX_THREADS];
    int id = 0;

    while(getElapsedTime()*1e-3<args.nsecs && !closed){
        struct request r = {args.fifoname, id};
        pthread_create(&threads[id], NULL, thr_func, &r);
        pthread_detach(threads[id]);
        usleep(50*1000); //create WC requests every 500 ms
        id++;
    }

    pthread_exit(0);
}