#pragma once

#include <stdlib.h>

typedef struct Queue
{
    int front, rear, size;  
    unsigned capacity;  
    int* array;
} queue;

queue createPlaceQueue(unsigned capacity);
int isFull(queue* q);
int isEmpty(queue* q);
void makeAvailable(queue* q, unsigned item);
unsigned usePlace(queue* q);
void fillPlaceQueue(queue* q);