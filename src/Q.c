#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include "args.h"

int main(int argc, char *argv[], char *envp[]){
    server_args args;
    if(get_server_args(&args, argc, argv)==-1){
        perror("Error getting args!");
        exit(1);
    }

    if (mkfifo("fifoname", 0660) != 0){ //Makes fifo
        printf("Error, can't create FIFO!\n");
        exit(1);
    }
    else{
        printf("FIFO was created!\n");
    }

    int fd;
    if((fd=open("fifoname", O_RDONLY)) != -1){
        printf("FIFO is open read\n");
    }
    else{
        printf("Can't open FIFO\n");
        if(unlink("fifoname") < 0)
            printf("Error can't destroy FIFO!\n");
        else
            printf("FIFO has been destroyed!\n");
        
    }
    
    long int time = 0;
    char msg[256];
    pthread_t t;

    sleep(2);
    if (read(fd, &msg, 256) > 0 && msg[0] == '[') printf(msg);

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