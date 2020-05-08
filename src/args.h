#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    unsigned int nsecs;
    char fifoname[256];
}client_args;

typedef struct {
    unsigned int nsecs;
    unsigned int nplaces;
    unsigned int nthreads;
    char fifoname[256];
}server_args;

client_args get_client_args(int argc, char *argv[]);
server_args get_server_args(int argc, char *argv[]);