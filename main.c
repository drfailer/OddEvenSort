#include <stdlib.h>
#include <mpi/mpi.h>
#include "utils.h"
#include <assert.h>
#ifdef RUN_BUBBLESORT
#include "bubblesort.h"
#else
#include "evenoddsort.h"
#endif

/******************************************************************************/
/*                                    main                                    */
/******************************************************************************/

int main(int argc, char **argv) {
    int *arr = generateRandomArr(80);
    int rank = -1;

    #ifdef RUN_BUBBLESORT
    bubbleSort(arr, 80, &greater);
    printArr(arr, 80);
    #else
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    sortEvenOdd(arr, 80);
    if (rank == 0) {
        assert(assertSorted(arr, 80));
    }
    MPI_Finalize();
    #endif

    free(arr);
    return 0;
}
