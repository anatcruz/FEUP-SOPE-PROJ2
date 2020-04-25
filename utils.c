#include "utils.h"

struct timeval initTime, currentTime;
double elapsedTime;

void getBeginTime(){
    gettimeofday(&initTime, NULL);
    elapsedTime = 0;
}

double getElapsedTime(){
    gettimeofday(&currentTime, NULL);

    elapsedTime = (currentTime.tv_sec - initTime.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (currentTime.tv_usec - initTime.tv_usec) / 1000.0;   // us to ms

    return elapsedTime;
}