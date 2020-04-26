#include "utils.h"

struct timeval initTime, currentTime;

void getBeginTime(){
    gettimeofday(&initTime, NULL);
}

double getElapsedTime(){
    gettimeofday(&currentTime, NULL);

    double elapsedTime = (currentTime.tv_sec - initTime.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (currentTime.tv_usec - initTime.tv_usec) / 1000.0;   // us to ms

    return elapsedTime;
}

void logRegister(int i, int pid, long tid, int dur, int pl, char *oper) {
    fprintf(stdout, "%f ; %d ; %d ; %ld ; %d ; %d ; %s\n", getElapsedTime(), i, pid, tid, dur, pl, oper);
    fflush(stdout);
}