#include <stdio.h>
#include <mpi/mpi.h>

/******************************************************************************/
/*                                print array                                 */
/******************************************************************************/

void printArr(int *arr, int n) {
    printf("[ ");
    for (int i  = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
    printf(" ]\n");
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
/*                                    main                                    */
/******************************************************************************/

int main(int argc, char **argv) {
    int rank=-1;
    int nbprocs=0;
    int arr[] = {
        2, 3, 1, 9, 7, 8, 5, 6
    };

    bubbleSort(arr, 8, &greater);
    printArr(arr, 8);

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size(MPI_COMM_WORLD, &nbprocs);
    printf("Hello, World!\n");
    MPI_Finalize();
    return 0;
}
