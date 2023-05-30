#ifndef _PRIORITY_QUEUE_H
#define _PRIORITY_QUEUE_H

typedef int ElementType;
typedef struct HeapStruct {
    int capacity;       // 最大元素数量
    int size;           // 堆元素数量
    ElementType* eles;  // 堆元素数组
} PriorityQueue;

#define true 1
#define false 0
#define SUCCESS 0
#define FAILURE 1

PriorityQueue* pqInit(int maxEleNum);
int pqIsFull(PriorityQueue* pq);
int pqIsEmpty(PriorityQueue* pq);
int pqInsert(PriorityQueue* pq, ElementType value);
int pqFindMin(PriorityQueue* pq, ElementType* value);
int pqDeleteMin(PriorityQueue* pq, ElementType* min);
int pqDestory(PriorityQueue* pq);

#endif
