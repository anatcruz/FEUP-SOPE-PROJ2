#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

typedef struct Arguments {
    unsigned int nsecs;
    char fifoname[256];
}Args;

void init_args(Args *args){
    args->nsecs=-1;
    args->fifoname[0]=0;
}

int get_args(Args *args, int argc, char *argv[]){
    if(argc!=4){
        printf("Usage: %s <-t nsecs> fifoname\n", argv[0]);
        return -1;
    }

    for (int i=0; i<argc; i++){
        if (!strcmp(argv[i], "-t")){
            if (args->nsecs!=-1) return -1;
            if (atoi(argv[i+1])){
                args->nsecs = atoi(argv[i+1]);
                i++;
            }
            else{
                return -1; //The next argument is not a number
            } 
        }
        else if (argv[i][1] != '-'){
            strncpy(args->fifoname, argv[i], sizeof(args->fifoname));
        }
    }
    return 0;
}

void print_args(Args args){
    printf("nsecs: %d\n", args.nsecs);
    printf("fifoname: %s\n", args.fifoname);
}

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
    /*Args args;
    init_args(&args);
    if(get_args(&args, argc, argv)==-1){
        perror("Error getting args!");
        exit(1);
    }*/
    //print_args(args);

    pthread_t tid;

    pthread_create(&tid, NULL, thr_func, "fifoname");
    pthread_join(tid, NULL);

    pthread_exit(0);
}