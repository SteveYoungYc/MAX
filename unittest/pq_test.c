#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <util/priority_queue.h>
#include <time.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 100

typedef struct {
    PriorityQueue* pq;
    int* arr;
    int size;
    int threadId;
} ThreadData;

void* insertElements(void* data) {
    ThreadData* threadData = (ThreadData*)data;
    PriorityQueue* pq = threadData->pq;
    int* arr = threadData->arr;
    int size = threadData->size;
    int threadId = threadData->threadId;

    // Calculate the range of elements to be inserted by this thread
    int startIndex = threadId * (size / NUM_THREADS);
    int endIndex = (threadId == NUM_THREADS - 1) ? size : (threadId + 1) * (size / NUM_THREADS);

    for (int i = startIndex; i < endIndex; i++) {
        pqInsert(pq, arr[i]);
    }

    pthread_exit(NULL);
}

int main(void) {
    PriorityQueue* pq = pqInit(ARRAY_SIZE);
    int arr[ARRAY_SIZE];

    srand(time(NULL));

    // Fill the array with random numbers
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = rand() % 1000;  // Random number between 0 and 999
    }

    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].pq = pq;
        threadData[i].arr = arr;
        threadData[i].size = ARRAY_SIZE;
        threadData[i].threadId = i;
        pthread_create(&threads[i], NULL, insertElements, (void*)&threadData[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("The arr value is:");
    for (int i = 0; i < pq->size; i++) {
        printf(" %d", pq->eles[i + 1]);
    }
    printf("\n");
    printf("pq size is %d\n", pq->size);

    ElementType min;
    int size = pq->size;

    for (int i = 0; i < size; i++) {
        if (SUCCESS == pqFindMin(pq, &min)) {
            printf("the min is %d\n", min);
            pqDeleteMin(pq, &min);
        }
    }

    pqDestory(pq);
    return 0;
}
