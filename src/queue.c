#include "queue.h"

queue createQueue(unsigned capacity) {
    queue q;
    q.capacity = capacity;
    q.size = 0;
    q.front = 0;
    q.rear = capacity - 1;
    q.array = (int*) malloc(capacity * sizeof(int));
    fillQueue(&q);
    return q;
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

void fillQueue(queue* q) {
    for (unsigned i = 1; i <= q->capacity; i++) {
        makeAvailable(q, i);
    }
}