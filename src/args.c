#include "args.h"

client_args get_client_args(int argc, char *argv[]){
    if(argc!=4){
        printf("Usage: %s <-t nsecs> fifoname\n", argv[0]);
        exit(1);
    }

    client_args args = {0, "\0"};

    for (int i=1; i<argc; i++){
        if (!strcmp(argv[i], "-t")){
            if (args.nsecs!=0 || i+1 == argc){
                perror("Repeated nsecs argument");
                exit(1);
            }
            if (atoi(argv[i+1]) > 0){
                args.nsecs = atoi(argv[i+1]);
                i++;
            }
            else{
                perror("nsecs must be a positive integer"); //The next argument is not a valid number
                exit(1);
            } 
        }
        else if (argv[i][0] != '-'){
            if(args.fifoname[0] != '\0'){
                perror("Repeated fifoname argument");
                exit(1);
            }
            strncpy(args.fifoname, argv[i], sizeof(args.fifoname));
        }
        else {
            exit(1);
        }
    }

    if(args.fifoname[0] == '\0'){
        perror("No fifoname provided");
        exit(1);
    }
    if(args.nsecs == 0){
        perror("No nsecs provided");
        exit(1);
    }

    // printf("fifoname: %s\n", args.fifoname);
    // printf("nsecs: %d\n", args.nsecs);
    
    return args;
}

server_args get_server_args(int argc, char *argv[]){
    if(argc>8 || argc<4){
        printf("Usage: %s <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n", argv[0]);
        exit(1);
    }

    server_args args = {0, 0, 0, "\0"};

    for (int i=1; i<argc; i++){
        if (!strcmp(argv[i], "-t")){
            if (args.nsecs!=0 || i+1 == argc){
                perror("Repeated nsecs argument");
                exit(1);
            }
            if (atoi(argv[i+1]) > 0){
                args.nsecs = atoi(argv[i+1]);
                i++;
            }
            else{
                perror("nsecs must be a positive integer"); //The next argument is not a valid number
                exit(1);
            }
        }
        else if (!strcmp(argv[i], "-l")){
            if (args.nplaces!=0 || i+1 == argc){
                perror("Repeated nplaces argument");
                exit(1);
            }
            if (atoi(argv[i+1]) > 0){
                args.nplaces = atoi(argv[i+1]);
                i++;
            }
            else{
                perror("nplaces must be a positive integer"); //The next argument is not a valid number
                exit(1);
            } 
        }
        else if (!strcmp(argv[i], "-n")){
            if (args.nthreads!=0 || i+1 == argc){
                perror("Repeated nplaces argument");
                exit(1);
            }
            if (atoi(argv[i+1]) > 0){
                args.nthreads = atoi(argv[i+1]);
                i++;
            }
            else{
                perror("nthreads must be a positive integer"); //The next argument is not a valid number
                exit(1);
            } 
        }
        else if (argv[i][0] != '-'){
            if(args.fifoname[0] != '\0'){
                perror("Repeated fifoname argument");
                exit(1);
            }
            strncpy(args.fifoname, argv[i], sizeof(args.fifoname));
        }
        else {
            exit(1);
        }
    }

    if(args.fifoname[0] == '\0'){
        perror("No fifoname provided");
        exit(1);
    }
    if(args.nsecs == 0){
        perror("No nsecs provided");
        exit(1);
    }

    // printf("fifoname: %s\n", args.fifoname);
    // printf("nsecs: %d\n", args.nsecs);
    // printf("nplaces: %d\n", args.nplaces);
    // printf("nthreads: %d\n", args.nthreads);

    return args;
}