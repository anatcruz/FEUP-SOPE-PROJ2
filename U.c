#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

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

/*void *thr_func(void *arg){
    int fd=
}*/

int main(int argc, char *argv[], char *envp[]){
    Args args;
    init_args(&args);
    if(get_args(&args, argc, argv)==-1){
        perror("Error getting args!");
        exit(1);
    }
    //print_args(args);
    int fd, messagelen;
    char message[256];
    do{
        fd = open(args.fifoname, O_WRONLY);
        if (fd==-1) sleep(1);
    } while(fd==-1);

    for (int i=1;i<=3;i++){
        sprintf(message, "Hello no. %d from process %d\n", i, getpid());
        messagelen=strlen(message)+1;
        write(fd, message, messagelen);
        sleep(3);
    }
    close(fd);

    return 0;
}