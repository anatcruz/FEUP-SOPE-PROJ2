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

int get_client_args(client_args *args, int argc, char *argv[]);
int get_server_args(server_args *args, int argc, char *argv[]);