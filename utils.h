#ifndef UTILS_HPP
#define UTILS_HPP
#include <stdbool.h>

bool assertSorted(int *arr, long size);
void printArr(int *arr, long n);
int * generateRandomArr(long size);
int less(int a, int b);
int greater(int a, int b);
int qless(const void * a, const void * b);
int qgreater(const void * a, const void * b);

#endif
