#include "queue.h"

queue createPlaceQueue(unsigned capacity) {
    queue pq;
    pq.capacity = capacity;
    pq.size = 0;
    pq.front = 0;
    pq.rear = capacity - 1;
    pq.array = (int*) malloc(capacity * sizeof(int));
    return pq;
}

int isFull(queue* q) {
    return (q->size == q->capacity);
}

int isEmpty(queue* q) {
    return (q->size == 0);
}

void makeAvailable(queue* q, unsigned item) {
    if (isFull(q)) {
        return;
    }
    q->rear = (q->rear + 1) % q->capacity;
    q->array[q->rear] = item;
    q->size++;
}

unsigned usePlace(queue* q) {
    if (isEmpty(q)) {
        return 0;
    }
    unsigned item = q->array[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return item;
}

void fillPlaceQueue(queue* q) {
    for (unsigned i = 1; i <= q->capacity; i++) {
        makeAvailable(q, i);
    }
}