#include <stdio.h>
#include <stdlib.h>
#define INF 2147483647

struct Vertex {
	int x, y;				// index do reprezentacie grafu
	char status;			// binarne hodnoty stavu hladania: bit 0: je zabity drak, bit 1: je zachranena 1. princezna; bit 2: 2. princezna; ...; bit 5: 5. princezna
	int lenght;				// dlzka cesty
}VERTEX;

int heap[255];				// minimalna binarna halda / min heap
							// moze to byt globalna premenna?

							// https://www.youtube.com/watch?v=lAXZGERcDf4

int getParent(int i) {
	return (i - 1) / 2;
}

int getLeft(int i) {
	return i * 2 + 1;
}

int getRight(int i) {
	return i * 2 + 2;
}

void exchange(int* x, int* y) {
	int temp = *x;
	*x = *y;
	*y = temp;
}

void fixHeapUp(int **heap, int fix) {						
	int parent = getParent(fix);

	if (parent < 0)
		return;

	while (fix != 0 && (*heap)[parent] > (*heap)[fix]) {
		exchange(&(*heap)[parent], &(*heap)[fix]);

		fix = parent;
		parent = getParent(fix);
	}
}

void insert(int **heap, int *heapSize, int key) {
	(*heap)[(*heapSize)++] = key;
	fixHeapUp(&*heap, *heapSize - 1);
}

void decrease(int** heap, int i, int key) {
	(*heap)[i] = key;
	fixHeapUp(*&heap, i);
}

void heapify(int** heap, int heapSize, int fix) {
	int left = getLeft(fix);
	int right = getRight(fix);
	int min;

	if (left < heapSize && (*heap)[left] < (*heap)[fix])
		min = left;
	else
		min = fix;

	if (right < heapSize && (*heap)[right] >(*heap)[min])
		min = right;
	if (min != fix) {
		exchange(&(*heap)[fix], &(*heap)[min]);
		heapify(&*heap, heapSize, min);
	}
}

int extractMin(int **heap, int *heapSize) {
	printf("%d\n", *heapSize);
	(*heapSize)--;
	if (*heapSize < 0)
		return INF;
	if (*heapSize == 0) 
		return (*heap)[0];

	int min = (*heap)[0];
	(*heap)[0] = (*heap)[(*heapSize)];
	heapify(*&heap, *heapSize, 0);
	return min;
}

void search(int* heap, int* heapSize) {

}

void printHeap(int* heap, int heapSize) {
	for (int i = 0; i < heapSize; i++)
		printf("%d ", heap[i]);
	printf("\n");
}

void printMin(int** heap, int *heapSize) {
	int min = extractMin(&*heap, &*heapSize);
	printf("Min je %d\n", min);
	printHeap(*heap, *heapSize);
}


void transform(char** mapa) {

}

int* zachran_princezne(char** mapa, int n, int m, int t, int* dlzka_cesty) {
	// Zadanim pozadovana funkcia
	int *minHeap = NULL, * path = NULL;
	*dlzka_cesty = 0;

	return NULL;
}

int main()
{
	int capacity = 100;
	int *heap = malloc(capacity * sizeof(int));
	int heapSize = 0;

	insert(&heap, &heapSize, 5);
	printHeap(heap, heapSize);

	insert(&heap, &heapSize, 4);
	printHeap(heap, heapSize);

	insert(&heap, &heapSize, 3);
	printHeap(heap, heapSize);

	insert(&heap, &heapSize, 2);
	printHeap(heap, heapSize);

	decrease(&heap, 2, -1);
	printHeap(heap, heapSize);

	printMin(&heap, &heapSize);

	return 0;
}