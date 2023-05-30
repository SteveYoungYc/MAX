#include <input_manager.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <util/priority_queue.h>

int main(void) {
    PriorityQueue* pq = pqInit(6);
    int arr[] = {3, 4, 5, 6, 8, 2, 9, 10};
    int i = 0;

    for (i = 0; i < 8; i++) {
        pqInsert(pq, arr[i]);
    }

    printf("the arr value is:");
    for (i = 0; i < pq->size; i++) {
        printf(" %d", pq->eles[i + 1]);
    }
    printf("\n");
    printf("pq size is %d\n", pq->size);
    ElementType min;
    int size = pq->size;

    for (i = 0; i < size; i++) {
        if (SUCCESS == pqFindMin(pq, &min)) {
            printf("the min is %d\n", min);
            pqDeleteMin(pq, &min);
        }
    }

    pqDestory(pq);
    return 0;
}
