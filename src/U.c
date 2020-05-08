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

    //Creating Client private fifo
    char private_fifo[MAX_LEN];
    sprintf(private_fifo, "/tmp/%d.%ld", client_pid, client_tid);
    if (mkfifo(private_fifo, 0660) != 0){   //Makes private FIFO
        logRegister(r.i, client_pid, client_tid, duration, -1, "FAILD");
        perror("Can't make private fifo!");
        return NULL;
    }

    //Opening public fifo
    int fd = open(r.fifoname, O_WRONLY);
    if (fd==-1){
        closed = 1;
        logRegister(r.i, client_pid, client_tid, duration, -1, "FAILD");
        perror("Can't open public fifo WRONLY!");
        if(unlink(private_fifo) < 0)
            perror("Can't destroy private fifo!");
        closed=1;
        return NULL;
    }

    //Send request to WC
    char message[MAX_LEN];
    sprintf(message, "[ %d, %d, %ld, %d, %d ]\n", r.i, client_pid, client_tid, duration, -1);
    if (write(fd, &message, MAX_LEN)<0){
        logRegister(r.i, client_pid, client_tid, duration, -1, "FAILD");
        perror("Can't write to public fifo!");
        if(unlink(private_fifo) < 0)
            perror("Can't destroy private fifo!");
        closed=1;
        return NULL;
    }
    close(fd);

    logRegister(r.i, client_pid, client_tid, duration, -1, "IWANT");


    //Reading WC response
    int id, pid, pl, dur, fd_private;
    long tid;

    //Opens private FIFO for reading and checks for error
    if((fd_private=open(private_fifo, O_RDONLY)) == -1){
        logRegister(r.i, client_pid, client_tid, duration, -1, "FAILD");
        perror("Can't open private fifo RDONLY!");
        if(unlink(private_fifo) < 0)
            perror("Can't destroy private fifo!");
        return NULL;
    }

    //Reads message from private FIFO
    if (read(fd_private, &message, MAX_LEN)<0){
        logRegister(r.i, client_pid, client_tid, duration, -1, "FAILD");
        perror("Can't read from private fifo!");
        return NULL;
    }
    sscanf(message, "[ %d, %d, %ld, %d, %d ]\n", &id, &pid, &tid, &dur, &pl);   

    if (pl == -1 && dur == -1){    //WC is closing
        //closed = 1;
        logRegister(r.i, client_pid, client_tid, dur, pl, "CLOSD");
    }
    else{    //WC is open
        logRegister(r.i, client_pid, client_tid, dur, pl, "IAMIN");
    }

    close(fd_private);    //Closes private FIFO
    if(unlink(private_fifo) < 0)    //Destroys private FIFO
       perror("Can't destroy Private FIFO!");

    return NULL;
}


int main(int argc, char *argv[], char *envp[]){
    client_args args = get_client_args(argc, argv);

    getBeginTime();

    int id = 0;

    while(getElapsedTime()<args.nsecs && !closed){
        struct request r = {args.fifoname, id};
        pthread_t thread;
        pthread_create(&thread, NULL, thr_func, &r);
        pthread_detach(thread);
        usleep(50*1000); //create WC requests every 50 ms
        id++;
    }

    pthread_exit(0);
}