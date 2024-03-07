#include <stdlib.h>
#include <mpi.h>
#include "utils.h"
#include <string.h>
#include "bubblesort.h"
#include "evenoddsort.h"
#include "quick-test-c/quicktest.h"

/******************************************************************************/
/*                                   tests                                    */
/******************************************************************************/

Test(bubblesort) {
    int rank = -1;

    MPI_Comm_rank( MPI_COMM_WORLD, &rank);

    /* sequential test is run just for 0 */
    if (rank == 0) {
        int *arr = generateRandomArr(20);

        bubbleSort(arr, 20, &greater);
        assert(assertSorted(arr, 20));

        /* same value */
        memset(arr, 0, 20 * sizeof(int));
        bubbleSort(arr, 20, &greater);
        assert(assertSorted(arr, 20));

        /* worst case */
        for (int i = 0; i < 20; ++i) {
            arr[i] = 20 - i;
        }
        bubbleSort(arr, 20, &greater);
        assert(assertSorted(arr, 20));

        free(arr);
    }
    return 0;
}

Test(evenoddsortOneElement) {
    int nbprocs = -1;

    MPI_Comm_size(MPI_COMM_WORLD, &nbprocs);
    int *arr = generateRandomArr(nbprocs);

    sortEvenOdd(arr, nbprocs, false);
    MPI_assert(assertSorted(arr, nbprocs));

    free(arr);
    return 0;
}

Test(evenoddsortGeneral) {
    int nbprocs = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &nbprocs);
    int size = nbprocs * 20;
    int *arr = generateRandomArr(size);

    sortEvenOdd(arr, size, false);
    MPI_assert(assertSorted(arr, size));

    free(arr);
    return 0;
}

Test(evenoddsortWorst) {
    int nbprocs = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &nbprocs);
    int size = nbprocs * 20;
    int *arr = malloc(size * sizeof(int));

    for (int i = 0; i < size; ++i) {
        arr[i] = size - i;
    }

    sortEvenOdd(arr, size, false);
    MPI_assert(assertSorted(arr, size));

    free(arr);
    return 0;
}

Test(evenoddsortAlreadySorted) {
    int nbprocs = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &nbprocs);
    int size = nbprocs * 20;
    int *arr = malloc(size * sizeof(int));

    for (int i = 0; i < size; ++i) {
        arr[i] = i;
    }

    sortEvenOdd(arr, size, false);
    MPI_assert(assertSorted(arr, size));

    free(arr);
    return 0;
}

Test(evenoddsortSame) {
    int nbprocs = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &nbprocs);
    int size = nbprocs * 20;
    int *arr = malloc(size * sizeof(int));

    memset(arr, 1, size*sizeof(int));

    sortEvenOdd(arr, size, false);
    MPI_assert(assertSorted(arr, size));

    free(arr);
    return 0;
}

/******************************************************************************/
/*                                    main                                    */
/******************************************************************************/

int main(int argc, char **argv) {
    MPI_Init( &argc, &argv );
    MPI_TestInit();

    TestRun(bubblesort, "test sequential bubblesort");
    TestRun(evenoddsortOneElement, "evenoddsort with on element per thread");
    TestRun(evenoddsortSame, "evenoddsort same value in the array");
    TestRun(evenoddsortGeneral, "evenoddsort general case");
    TestRun(evenoddsortWorst, "evenoddsort worst case");
    TestRun(evenoddsortAlreadySorted, "the array is already sorted");

    MPI_TestEnd();
    MPI_Finalize();
    return TEST_RESULT;
}
