#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include <string.h>
#define SIZE 8

/******************************************************************************/
/*                                print array                                 */
/******************************************************************************/

void printArr(int *arr, int n) {
    printf("[ ");
    for (int i  = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
    printf("]\n");
}

/******************************************************************************/
/*                           tri a bulle séquentiel                           */
/******************************************************************************/

void bubbleSort(int *arr, int n, int (*compare)(int, int)) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j <= i; ++j) {
            if (compare(arr[j], arr[j + 1])) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

/******************************************************************************/
/*                                comparators                                 */
/******************************************************************************/

int less(int a, int b) {
    return a < b;
}

int greater(int a, int b) {
    return a > b;
}

/******************************************************************************/
/*                              test bubble sort                              */
/******************************************************************************/

void runBubbleSort() {
    int arr[] = {
        2, 3, 1, 9, 7, 8, 5, 6
    };

    bubbleSort(arr, 8, &greater);
    printArr(arr, 8);
}

/******************************************************************************/
/*                                   merge                                    */
/******************************************************************************/

void merge(int *dest, int *a, int *b, int n) {
    int idx = 0, idxa = 0, idxb = 0;

    while (idxa < n && idxb < n) {
        if (a[idxa] < b[idxb]) {
            dest[idx++] = a[idxa++];
        } else {
            dest[idx++] = b[idxb++];
        }
    }

    if (idxa < n) {
        while (idxa < n) {
            dest[idx++] = a[idxa++];
        }
    } else if (idxb < n) {
        while (idxb < n) {
            dest[idx++] = b[idxb++];
        }
    }
}

void sortSubArrays(int *subArr, int *subArrShared, int subArrSize) {
    int *tmp = malloc(subArrSize * 2 * sizeof(int));

    merge(tmp, subArr, subArrShared, subArrSize);
    memcpy(subArr, tmp, sizeof(int) * subArrSize);
    memcpy(subArrShared, tmp + subArrSize, sizeof(int) * subArrSize);
}


/******************************************************************************/
/*                                    main                                    */
/******************************************************************************/

int main(int argc, char **argv) {
    int rank=-1;
    int nbprocs=0;
    int tag = 50;
    MPI_Status status;
    int arr[SIZE] = {
        2, 3, 1, 4, 7, 8, 5, 6
    };

    /* runBubbleSort(); */

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nbprocs);

    int subArrSize = SIZE / nbprocs;
    int *subArr = malloc(subArrSize * sizeof(int));
    int *subArrShared = malloc(subArrSize * sizeof(int));

    MPI_Scatter(arr, subArrSize, MPI_INT, subArr, subArrSize, MPI_INT, 0, MPI_COMM_WORLD);

    // on trie le sous-tableau
    bubbleSort(subArr, subArrSize, &greater);

    for (int i = 0; i < nbprocs/2; ++i) {
        if (rank % 2 == 0) {
            // phase 1: the even sent to its N + 1
            if (rank < nbprocs - 1) {
                MPI_Recv(subArrShared, subArrSize, MPI_INT, rank + 1, tag, MPI_COMM_WORLD, &status);
                sortSubArrays(subArr, subArrShared, subArrSize);
                MPI_Send(subArrShared, subArrSize, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
            }
            // phase 2: the odd (N - 1) works with its N + 1
            if (rank > 0) {
                MPI_Send(subArr, subArrSize, MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
                MPI_Recv(subArr, subArrSize, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &status);
            }
        } else {
            // phase 1: the event (N - 1) works with its N + 1
            if (rank > 0) {
                MPI_Send(subArr, subArrSize, MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
                MPI_Recv(subArr, subArrSize, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &status);
            }
            // phase 2: the odd works to its N + 1
            if (rank < nbprocs - 1) {
                MPI_Recv(subArrShared, subArrSize, MPI_INT, rank + 1, tag, MPI_COMM_WORLD, &status);
                sortSubArrays(subArr, subArrShared, subArrSize);
                MPI_Send(subArrShared, subArrSize, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
            }
        }
    }

    MPI_Gather(subArr, subArrSize, MPI_INT, arr, subArrSize, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printArr(arr, SIZE);
    }

    MPI_Finalize();
    return 0;
}
