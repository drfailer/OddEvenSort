

/******************************************************************************/
/*                           tri a bulle séquentiel                           */
/******************************************************************************/

void bubbleSort(int *arr, int n, int (*compare)(int, int)) {
    for (int i = n - 1; i >= 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (compare(arr[j], arr[j + 1])) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}
