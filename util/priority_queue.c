#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/priority_queue.h>

PriorityQueue* pqInit(int maxEleNum) {
    PriorityQueue* pq = NULL;

    if (maxEleNum <= 0)
        return NULL;
    pq = malloc(sizeof(PriorityQueue));
    if (NULL == pq) {
        printf("malloc failed\n");
        return NULL;
    }

    /*下标为0的位置保留，不作使用*/
    pq->eles = malloc((maxEleNum + 1) * sizeof(ElementType));
    if (NULL == pq->eles) {
        printf("malloc failed\n");
        free(pq);
        return NULL;
    }

    /*初始化成员*/
    memset(pq->eles, 0, (maxEleNum + 1) * sizeof(ElementType));
    pq->capacity = maxEleNum;
    pq->size = 0;

    pthread_mutex_init(&pq->mutex, NULL);
    return pq;
}

/*判断优先队列是否满*/
int pqIsFull(PriorityQueue* pq) {
    if (NULL == pq)
        return false;
    if (pq->capacity == pq->size)
        return true;
    else
        return false;
}

/*判断优先队列是否为空*/
int pqIsEmpty(PriorityQueue* pq) {
    if (NULL == pq)
        return false;
    if (0 == pq->size)
        return true;
    else
        return false;
}

int pqInsert(PriorityQueue* pq, ElementType value) {
    int i = 0;
    pthread_mutex_lock(&pq->mutex);
    /*确保优先队列没有满*/
    if (pqIsFull(pq)) {
        printf("priorityQueue is full\n");
        pthread_mutex_unlock(&pq->mutex);
        return FAILURE;
    }
    printf("insert %d\n", value);
    /*不断和父节点探测比较，直到找到属于它的位置*/
    for (i = pq->size + 1; pq->eles[i / 2] > value && i > 1; i /= 2) {
        pq->eles[i] = pq->eles[i / 2];
    }
    pq->eles[i] = value;
    pq->size++;
    pthread_mutex_unlock(&pq->mutex);
    return SUCCESS;
}

int pqFindMin(PriorityQueue* pq, ElementType* value) {
    pthread_mutex_lock(&pq->mutex);
    if (pqIsEmpty(pq)) {
        printf("priorityQueue is empty\n");
        pthread_mutex_unlock(&pq->mutex);
        return FAILURE;
    }
    /*0处的元素作为哨兵没有使用*/
    *value = pq->eles[1];
    pthread_mutex_unlock(&pq->mutex);
    return SUCCESS;
}

int pqDeleteMin(PriorityQueue* pq, ElementType* min) {
    int i = 1;
    int minChild = 0;

    pthread_mutex_lock(&pq->mutex);
    if (pqIsEmpty(pq)) {
        printf("priorityqueue is empty\n");
        pthread_mutex_unlock(&pq->mutex);
        return FAILURE;
    }
    /*取得最小值*/
    *min = pq->eles[1];

    /*暂时取出最后的元素*/
    ElementType last = pq->eles[pq->size];
    pq->size--;
    if (0 == pq->size) {
        pq->eles[i] = 0;
        pthread_mutex_unlock(&pq->mutex);
        return SUCCESS;
    }
    /*不断将空穴下滑*/
    for (i = 1; i * 2 <= pq->size; i = minChild) {
        minChild = i * 2;
        /*找到更小的孩子*/
        if (minChild != pq->size && pq->eles[minChild + 1] < pq->eles[minChild])
            minChild += 1;

        /*如果最后一个元素比空穴处的小儿子大，则继续下滑空穴，将该孩子上滤*/
        if (last > pq->eles[minChild])
            pq->eles[i] = pq->eles[minChild];
        /*否则说明last放的位置不会破坏堆性质，则直接退出循环*/
        else
            break;
    }

    /*将最后的元素放在空穴位置*/
    pq->eles[i] = last;
    pthread_mutex_unlock(&pq->mutex);
    return SUCCESS;
}

/*销毁队列，按照与申请内存时相反的顺序释放内存*/
int pqDestory(PriorityQueue* pq) {
    if (NULL == pq) {
        return FAILURE;
    }
    pthread_mutex_destroy(&pq->mutex);
    free(pq->eles);
    pq->eles = NULL;
    free(pq);
    pq = NULL;
    printf("destory pq success\n");
    return SUCCESS;
}
