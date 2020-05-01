#include "utils.h"

struct timespec start;

void getBeginTime(){
    clock_gettime(CLOCK_MONOTONIC, &start);
}

double getElapsedTime(){
    struct timespec current;
    clock_gettime(CLOCK_MONOTONIC, &current);
    
    double elapsed_time = (current.tv_sec - start.tv_sec) * 1e9;
    elapsed_time = (elapsed_time + (current.tv_sec - start.tv_sec)) * 1e-9;

    return elapsed_time;
}
    
void logRegister(int i, int pid, long tid, int dur, int pl, char *oper) {
    printf("%ld ; %d ; %d ; %ld ; %d ; %d ; %s\n", time(NULL), i, pid, tid, dur, pl, oper);
    fflush(stdout);
}