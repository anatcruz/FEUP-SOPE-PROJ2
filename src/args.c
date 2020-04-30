#include "args.h"

int get_client_args(client_args *args, int argc, char *argv[]){
    if(argc!=4){
        printf("Usage: %s <-t nsecs> fifoname\n", argv[0]);
        return -1;
    }

    args->nsecs=-1;
    for (int i=1; i<argc; i++){
        if (!strcmp(argv[i], "-t")){
            if (args->nsecs!=-1 || i+1 == argc) return -1;
            if (atoi(argv[i+1])){
                args->nsecs = atoi(argv[i+1]);
                i++;
            }
            else{
                return -1; //The next argument is not a number
            } 
        }
        else if (argv[i][0] != '-'){
            strncpy(args->fifoname, argv[i], sizeof(args->fifoname));
        }
        else {
            return -1;
        }
    }

    // printf("fifoname: %s\n", args->fifoname);
    // printf("nsecs: %d\n", args->nsecs);
    
    return 0;
}

int get_server_args(server_args *args, int argc, char *argv[]){
    if(argc>8){
        printf("Usage: %s <-t nsecs> fifoname", argv[0]);
        return -1;
    }

    args->nsecs=-1;
    args->nplaces=-1;
    args->nthreads=-1;

    for (int i=1; i<argc; i++){
        if (!strcmp(argv[i], "-t")){
            if (args->nsecs!=-1 || i+1 == argc) return -1;
            if (atoi(argv[i+1])){
                args->nsecs = atoi(argv[i+1]);
                i++;
            }
            else{
                return -1; //The next argument is not a number
            } 
        }
        else if (!strcmp(argv[i], "-l")){
            if (args->nplaces!=-1 || i+1 == argc) return -1;
            if (atoi(argv[i+1])){
                args->nplaces = atoi(argv[i+1]);
                i++;
            }
            else{
                return -1; //The next argument is not a number
            } 
        }
        else if (!strcmp(argv[i], "-n")){
            if (args->nthreads!=-1 || i+1 == argc) return -1;
            if (atoi(argv[i+1])){
                args->nthreads = atoi(argv[i+1]);
                i++;
            }
            else{
                return -1; //The next argument is not a number
            } 
        }
        else if (argv[i][0] != '-'){
            strncpy(args->fifoname, argv[i], sizeof(args->fifoname));
        }
    }

    // printf("fifoname: %s\n", args->fifoname);
    // printf("nsecs: %d\n", args->nsecs);
    // printf("nplaces: %d\n", args->nplaces);
    // printf("nthreads: %d\n", args->nthreads);

    return 0;
}