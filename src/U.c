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

int i;

void *thr_func(void *arg){
    sleep(1);
    int fd = open((char *)arg, O_WRONLY);
    
    if (fd==-1){
        printf("WC closed!\n");
        return NULL;
    }
    printf("FIFO is open write\n");

    char message[256];
    sprintf(message, "[ %d, %d, %ld, %d, %d]", i, getpid(), (long int)pthread_self(), 10, -1);

    write(fd, &message, 256);
    close(fd);

    return NULL;
}

int main(int argc, char *argv[], char *envp[]){
    client_args args;
    if(get_client_args(&args, argc, argv)==-1){
        perror("Error getting args!");
        exit(1);
    }

    pthread_t tid;

    pthread_create(&tid, NULL, thr_func, "fifoname");
    pthread_join(tid, NULL);

    pthread_exit(0);
}