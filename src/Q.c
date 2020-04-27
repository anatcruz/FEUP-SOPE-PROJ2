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
    //Recieving request
    char private_fifo[256];
    int i, dur, pid, server_pid = getpid();
    long int tid, server_tid = pthread_self();

    sscanf((char *) arg,"[ %d, %d, %ld, %d, -1 ]\n",&i,&pid,&tid,&dur);
    logRegister(i, pid, tid, dur, -1, "RECVD");    //Request received

    sprintf(private_fifo, "/tmp/%d.%ld", pid, tid);
    printf("Server pfifo %s\n", private_fifo);    

    int fd_private;
    if((fd_private = open(private_fifo, O_WRONLY)) != -1){    //Opens private FIFO for writing
        printf("Private FIFO is open write %d \n", fd_private);
    }
    else{
        logRegister(i, getpid(), tid, dur, -1, "GAVUP");
        return NULL;
    }

    //Replying to client
    char client_reply[256];
    if(getElapsedTime()*1e-3 + dur*1e-3 < args.nsecs){    //Request accepted
        sprintf(client_reply, "[ %d, %d, %ld, %d, %d ]\n",i, server_pid, server_tid, dur, ++pl);
        logRegister(i, server_pid, server_tid, dur, pl, "ENTER");
    }
    else{    //WC is closing
        sprintf(client_reply, "[ %d, %d, %ld, %d, %d ]\n",i, server_pid, server_tid, -1, -1);
        logRegister(i, server_pid, server_tid, dur, -1, "2LATE");
    }
    write(fd_private, &client_reply, 256);

    usleep(dur*1000);
    logRegister(1, server_pid, server_tid, dur, pl-1, "TIMUP");
    close(fd_private);    //Closes private FIFO
    return NULL;
}


int main(int argc, char *argv[], char *envp[]){
    if(get_server_args(&args, argc, argv)==-1){
        perror("Error getting args!");
        exit(1);
    }

    getBeginTime();

    if (mkfifo(args.fifoname, 0660) != 0){    //Makes FIFO
        printf("Error, can't create FIFO!\n");
        exit(1);
    }
    else{
        printf("FIFO was created!\n");
    }

    int fd;
    if((fd=open(args.fifoname, O_RDONLY | O_NONBLOCK)) != -1){   //Opens FIFO for reading
        printf("FIFO is open read\n");
    }
    else{
        printf("Can't open FIFO\n");
        if(unlink(args.fifoname) < 0)
            printf("Error can't destroy FIFO!\n");
        else
            printf("FIFO has been destroyed!\n");
        exit(1);
    }
    
    //Getting client requests
    char msg[256];
    pthread_t t;

    while(getElapsedTime()*1e-3 < args.nsecs){
        
       if (read(fd, &msg, 256) > 0 && msg[0] == '['){
            pthread_create(&t, NULL, thr_func, (void *) msg);
            printf(msg);
            pthread_join(t, NULL);
       }
     
    }

    close(fd);    //Closes FIFO
    if(unlink(args.fifoname) < 0)    //Destroys FIFO
       printf("Error can't destroy FIFO!\n");
    else
        printf("FIFO has been destroyed!\n");

    pthread_exit(0);
}