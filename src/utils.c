#include "utils.h"

struct timespec start;

void getBeginTime(){
    clock_gettime(CLOCK_MONOTONIC, &start);
}

double getElapsedTime(){
    struct timespec current;
    clock_gettime(CLOCK_MONOTONIC, &current);
    return ((current.tv_sec - start.tv_sec)*1000 + (current.tv_nsec - start.tv_nsec)/1e6);
}
    
void logRegister(int i, int pid, long tid, int dur, int pl, char *oper) {
    printf("%f ; %d ; %d ; %ld ; %d ; %d ; %s\n", getElapsedTime(), i, pid, tid, dur, pl, oper);
    fflush(stdout);
}