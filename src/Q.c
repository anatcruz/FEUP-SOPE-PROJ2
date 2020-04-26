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

int pl = 1;
server_args args;

void *thr_func(void *arg){
    char client_fifo[64];
    sprintf(client_fifo, "/tmp/%d.%d", getpid(), (long int)pthread_self());
    char message[256] = (char *) arg;
    int i,duration, pid;
    long tid;
    sscanf(message,"[ %d, %d, %ld, %d, -1]",&i,&pid,&tid,&duration);
    logRegister(i, pid, tid, duration, -1, "RECVD"); //request received

    int fd;
    while(fd = open(client_fifo, O_WRONLY) < 0){
        logRegister(i, pid, tid, duration, -1, "GAVUP");
        printf("Failed to open FIFO!\n"); //private client fifo is already closed
        sleep(1);
    }

    char client_reply[256];
    if(getElapsedTime() + duration*1e-3 < args.nsecs){ //request accepted
        sprintf(client_reply, "[ %d, %d, %ld, %d, %d]",i,getpid(), (long int)pthread_self(),duration,pl++);
        logRegister(i, getpid(), (long int)pthread_self(), duration, pl-1, "ENTER");
    }
    else{ //bathroom closed
        sprintf(client_reply, "[ %d, %d, %ld, %d, %d]",i,getpid(), (long int)pthread_self(),-1,-1);
        logRegister(i, getpid(), (long int)pthread_self, duration, pl-1, "2LATE");
    }

    sleep(duration);
    logRegister(1, getpid(), (long int)pthread_self, duration, pl-1, "TIMUP");
    close(fd);
    return NULL;
}

int main(int argc, char *argv[], char *envp[]){
    
    if(get_server_args(&args, argc, argv)==-1){
        perror("Error getting args!");
        exit(1);
    }

    if (mkfifo(args.fifoname, 0660) != 0){ //Makes fifo
        printf("Error, can't create FIFO!\n");
        exit(1);
    }
    else{
        printf("FIFO was created!\n");
    }

    int fd;
    if((fd=open(args.fifoname, O_RDONLY)) != -1){
        printf("FIFO is open read\n");
    }
    else{
        printf("Can't open FIFO\n");
        if(unlink(args.fifoname) < 0)
            printf("Error can't destroy FIFO!\n");
        else
            printf("FIFO has been destroyed!\n");
        
    }
    
    long int time = 0;
    char msg[256];
    pthread_t t;

    sleep(2);
    if (read(fd, &msg, 256) > 0 && msg[0] == '[') printf(msg);
    pthread_create(&t, NULL, thr_func, (void *) msg);
    pthread_join(t, NULL);

    // while(time < /*args.nsecs*/ 5){
        
    //     while (read(fd, &msg, 256) > 0 && msg[0] == '[' && time < /*args.nsecs*/ 5)
    //     {
    //         printf(msg);
    //         time += 1;
    //         //pthread_create(&t, NULL, thr_func, (void *) arg);
    //         //pthread_join(t, NULL);
    //     }    
    // }

    close(fd);

    pthread_exit(0);
}