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

#define MAX_THREADS 100

int i;

void *thr_func(void *arg){
    //Send request to WC    
    int fd = open((char *)arg, O_WRONLY);
    
    if (fd==-1){
        printf("WC closed!\n");
        return NULL;
    }
    printf("FIFO is open write\n");

    char message[256];
    sprintf(message, "[ %d, %d, %ld, %d, %d ]\n", i, getpid(), (long int)pthread_self(), 50, -1);

    write(fd, &message, 256);
    close(fd);

    //Reading WC response
    char private_fifo[256];
    sprintf(private_fifo, "/tmp/%d.%ld", getpid(), (long int)pthread_self());
    printf("Client pfifo %s\n", private_fifo);

    if (mkfifo(private_fifo, 0660) != 0){ //Makes fifo
        printf("Error, can't create private FIFO!\n");
        exit(1);
    }
    else{
        printf("Private FIFO was created!\n");
    }

    int fd_private;
    if((fd_private=open(private_fifo, O_RDONLY)) != -1){
        printf("Private FIFO is open read %d\n", fd_private);
    }
    else{
        printf("Can't open FIFO\n");
        if(unlink(private_fifo) < 0)
            printf("Error can't destroy private FIFO!\n");
        else
            printf("Private FIFO has been destroyed!\n");
        exit(1);
    }

    if (read(fd_private, &message, 256)>0) printf(message);

    int id, pid, pl, dur;
    long tid;

    sscanf(message, "[ %d, %d, %ld, %d, %d ]\n", &id, &pid, &tid, &dur, &pl);

    if (pl == -1 && dur == -1){
        logRegister(id, pid, tid, dur, pl, "CLOSD");
    }
    else{
        logRegister(id, pid, tid, dur, pl, "IAMIN");
    }

    close(fd_private);

    if(unlink(private_fifo) < 0)
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
        pthread_join(threads[id], NULL);
        sleep(1);
    }

    pthread_exit(0);
}