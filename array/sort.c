#include <assert.h>
/*
 * in-place sort routines for array
 */
#define arraySize(A) (sizeof((A)) / sizeof(*(A)))

typedef void (*arraySortFn)(int A[], int n);

static inline void swap(int A[], int i, int j) {
	int temp = A[i];
	A[i] = A[j];
	A[j] = temp;
}

void arraySelectionSort(int A[], int n) {
	for (int i = 0; i < n - 1; i++) {
		int min = i;
		for (int j = i + 1; j < n; j++) {
			if (A[j] < A[min]) {
				min = j;
			}
		}
		swap(A, i, min);
	}
}

/*-----------------------
 * Utilities for testing
 *-----------------------*/
typedef struct {
	int size;
	int capacity;
	int data[1];
} VarLenArray;

int main(void) {
	return 0;
}
