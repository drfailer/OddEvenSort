#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************/
/*                                print array                                 */
/******************************************************************************/

void printArr(int *arr, long n) {
    printf("[ ");
    for (int i  = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
    printf("]\n");
}

/******************************************************************************/
/*                           generate random array                            */
/******************************************************************************/

int * generateRandomArr(long size) {
    int *arr = malloc(size * sizeof(int));

    srand(0);

    for (long i = 0; i < size; ++i) {
        arr[i] = rand();
    }
    return arr;
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

/* less for qsort */
int qless(const void * a, const void * b) {
    int lhs = *(const int*) a;
    int rhs = *(const int*) b;
    return rhs - lhs;
}

/* greater for qsort */
int qgreater(const void * a, const void * b) {
    int lhs = *(const int*) a;
    int rhs = *(const int*) b;
    return lhs - rhs;
}

/******************************************************************************/
/*                        check if the array is sorted                        */
/******************************************************************************/

bool assertSorted(int *arr, long size) {
    for (long i = 0; i < size - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}
