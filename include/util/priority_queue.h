#ifndef _PRIORITY_QUEUE_H
#define _PRIORITY_QUEUE_H

#include <pthread.h>

typedef int ElementType;
typedef struct HeapStruct {
    int capacity;
    int size;
    ElementType* eles;
    pthread_mutex_t mutex;
} PriorityQueue;

#define true 1
#define false 0
#define SUCCESS 0
#define FAILURE 1

PriorityQueue* pqInit(int maxEleNum);
int pqInsert(PriorityQueue* pq, ElementType value);
int pqFindMin(PriorityQueue* pq, ElementType* value);
int pqDeleteMin(PriorityQueue* pq, ElementType* min);
int pqDestory(PriorityQueue* pq);

#endif
