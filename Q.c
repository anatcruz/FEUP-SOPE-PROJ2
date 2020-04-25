#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>

typedef struct Arguments {
    unsigned int nsecs;
    unsigned int nplaces;
    unsigned int nthreads;
    char fifoname[256];
}Args;

void init_args(Args *args){
    args->nsecs=-1;
    args->nplaces=-1;
    args->nthreads=-1;
    args->fifoname[0]=0;
}

int get_args(Args *args, int argc, char *argv[]){
    if(argc>8){
        printf("Usage: %s <-t nsecs> fifoname", argv[0]);
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
        else if (!strcmp(argv[i], "-l")){
            if (args->nplaces!=-1) return -1;
            if (atoi(argv[i+1])){
                args->nplaces = atoi(argv[i+1]);
                i++;
            }
            else{
                return -1; //The next argument is not a number
            } 
        }
        else if (!strcmp(argv[i], "-n")){
            if (args->nthreads!=-1) return -1;
            if (atoi(argv[i+1])){
                args->nthreads = atoi(argv[i+1]);
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
    printf("nplaces: %d\n", args.nplaces);
    printf("nthreads: %d\n", args.nthreads);
    printf("fifoname: %s\n", args.fifoname);
}


int main(int argc, char *argv[], char *envp[]){
    Args args;
    init_args(&args);
    if(get_args(&args, argc, argv)==-1){
        perror("Error getting args!");
        exit(1);
    }
    //print_args(args);

    if (mkfifo(args.fifoname, 0660) != 0){ //Makes fifo
        printf("Error, can't create FIFO!\n");
        exit(1);
    }
    else{
        printf("FIFO was created!\n");
    }

    int fd;
    char str[100];
    if((fd=open(args.fifoname, O_RDONLY | O_NONBLOCK)) != -1){
        printf("FIFO is opened!\n");
        close(fd);
    }
    else{
        printf("Can't open FIFO\n");
        if(unlink(args.fifoname) < 0)
            printf("Error can't destroy FIFO!\n");
        else
            printf("FIFO has been destroyed!\n");
        
    }
    
    //long int timeout = args.nsecs * 1000000;
    long int time = 0;
    char msg[256];
    pthread t;

    while(time < args.nsecs){
        
        while (read(fd, &msg, 256) > 0 && msg[0] == '[' && time < args.nsecs)
        {
            printf(msg);
            time += 1;
            //pthread_create(&t, NULL, thr_func, (void *) arg);
            //pthread_join(t, NULL);
        }
        
        
    }


    
    return 0;
}