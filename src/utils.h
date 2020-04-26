#pragma once
#include <sys/time.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void getBeginTime();

double getElapsedTime();

void logRegister(int i, int pid, long tid, int dur, int pl, char *oper);