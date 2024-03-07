#include "evenoddsort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mpi.h>
#include "utils.h"

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
    // merge the end of arrays
    memcpy(dest + idx, a + idxa, (n - idxa) * sizeof(int));
    memcpy(dest + idx, b + idxb, (n - idxb) * sizeof(int));
}

void sortSubArrays(int *subArr, int *subArrShared, int subArrSize) {
    int *tmp = malloc(subArrSize * 2 * sizeof(int));

    merge(tmp, subArr, subArrShared, subArrSize);
    memcpy(subArr, tmp, sizeof(int) * subArrSize);
    memcpy(subArrShared, tmp + subArrSize, sizeof(int) * subArrSize);
}

/******************************************************************************/
/*                               sort even odd                                */
/******************************************************************************/

/*
 * Manage communication for even rank processes.
 */
void execEven(int *subArr, int *subArrShared, int subArrSize, int nbprocs,
        int rank, int tag, MPI_Status* status) {
    // phase 1: the even sent to its N + 1
    if (rank < nbprocs - 1) {
        MPI_Recv(subArrShared, subArrSize, MPI_INT, rank + 1, tag, MPI_COMM_WORLD, status);
        sortSubArrays(subArr, subArrShared, subArrSize);
        MPI_Send(subArrShared, subArrSize, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
    }
    // phase 2: the odd (N - 1) works with its N + 1
    if (rank > 0) {
        MPI_Send(subArr, subArrSize, MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
        MPI_Recv(subArr, subArrSize, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, status);
    }
}

/*
 * Manage communication for odd rank processes.
 */
void execOdd(int *subArr, int *subArrShared, int subArrSize, int nbprocs,
        int rank, int tag, MPI_Status* status) {
    // phase 1: the event (N - 1) works with its N + 1
    if (rank > 0) {
        MPI_Send(subArr, subArrSize, MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
        MPI_Recv(subArr, subArrSize, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, status);
    }
    // phase 2: the odd works to its N + 1
    if (rank < nbprocs - 1) {
        MPI_Recv(subArrShared, subArrSize, MPI_INT, rank + 1, tag, MPI_COMM_WORLD, status);
        sortSubArrays(subArr, subArrShared, subArrSize);
        MPI_Send(subArrShared, subArrSize, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
    }
}

/*
 * The sort algorithm.
 */
void sortEvenOdd(int *arr, int size, bool measureTime) {
    int rank = -1;
    int nbprocs = 0;
    int tag = 50;
    MPI_Status status;
    int subArrSize;
    int *subArr;
    int *subArrShared;
    double tStart, tEnd;

    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nbprocs);

    // verify if the SIZE is valid
    assert(size >= nbprocs && size % nbprocs == 0);

    subArrSize = size / nbprocs;
    subArr = malloc(subArrSize * sizeof(int));
    subArrShared = malloc(subArrSize * sizeof(int));

    MPI_Scatter(arr, subArrSize, MPI_INT, subArr, subArrSize, MPI_INT, 0, MPI_COMM_WORLD);

    /* time measure */
    if (measureTime) {
        MPI_Barrier(MPI_COMM_WORLD);
        tStart = MPI_Wtime();
    }

    // sort the sub array
    #ifdef USE_BUBBLESORT
    bubbleSort(subArr, subArrSize, &greater);
    #else
    qsort(subArr, subArrSize, sizeof(int), &qgreater);
    #endif

    for (int i = 0; i < nbprocs/2; ++i) {
        if (rank % 2 == 0) {
            execEven(subArr, subArrShared, subArrSize, nbprocs, rank, tag, &status);
        } else {
            execOdd(subArr, subArrShared, subArrSize, nbprocs, rank, tag, &status);
        }
    }

    MPI_Gather(subArr, subArrSize, MPI_INT, arr, subArrSize, MPI_INT, 0, MPI_COMM_WORLD);

    /* time measure */
    if (measureTime) {
        MPI_Barrier(MPI_COMM_WORLD);
        tEnd = MPI_Wtime();
        printf("process: %d, time: %lf\n", rank, tEnd - tStart);
    }

    // FREE
    free(subArr);
    free(subArrShared);
}
