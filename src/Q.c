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

int pl = 0;
server_args args;

void *thr_func(void *arg){
    char private_fifo[256];
    int i,duration, pid;
    long tid;

    sscanf((char *) arg,"[ %d, %d, %ld, %d, -1]",&i,&pid,&tid,&duration);
    logRegister(i, pid, tid, duration, -1, "RECVD"); //request received

    sprintf(private_fifo, "/tmp/%d.%ld", pid, tid);

    int fd_private;
    // if((fd_private = open(private_fifo, O_WRONLY)) != -1){
    //     printf("Private FIFO is open write\n");
    // }
    while(fd_private = open(private_fifo, O_WRONLY) < 0){
        // logRegister(i, pid, tid, duration, -1, "GAVUP");
        // printf("Failed to open FIFO!\n"); //private client fifo is already closed
        usleep(1000);
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
    
    write(fd_private, &client_reply, 256);

    usleep(duration*1000);
    logRegister(1, getpid(), (long int)pthread_self, duration, pl-1, "TIMUP");
    close(fd_private);
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

    int fd_private;
    if((fd_private=open(args.fifoname, O_RDONLY)) != -1){
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
    if (read(fd_private, &msg, 256) > 0 && msg[0] == '[') printf("%s\n",msg);
    pthread_create(&t, NULL, thr_func, (void *) msg);
    pthread_join(t, NULL);

    // while(time < /*args.nsecs*/ 5){
        
    //     while (read(fd_private, &msg, 256) > 0 && msg[0] == '[' && time < /*args.nsecs*/ 5)
    //     {
    //         printf(msg);
    //         time += 1;
    //         //pthread_create(&t, NULL, thr_func, (void *) arg);
    //         //pthread_join(t, NULL);
    //     }    
    // }

    close(fd_private);


    if(unlink(args.fifoname) < 0)
       printf("Error can't destroy FIFO!\n");
    else
        printf("FIFO has been destroyed!\n");


    pthread_exit(0);
}