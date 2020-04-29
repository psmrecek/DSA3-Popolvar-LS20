#include <stdio.h>
#include <stdlib.h>
#define INF 2147483647

typedef struct vertex {
	int x, y;				// indexy do reprezentacie grafu
	char status;			// binarne hodnoty stavu hladania: bit 0: je zabity drak, bit 1: je zachranena 1. princezna; bit 2: 2. princezna; ...; bit 5: 5. princezna
	int lenght;				// dlzka cesty
	int parentX, parentY;	// indexy na rodica
	char type;
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

int getMin(VERTEX** heap) {
	return (*heap)[0].lenght;
}

void exchange(VERTEX* a, VERTEX* b, int*** mapHeap) {
	int vertex1 = (*mapHeap)[(*a).x][(*a).y];
	(*mapHeap)[(*a).x][(*a).y] = (*mapHeap)[(*b).x][(*b).y];
	(*mapHeap)[(*b).x][(*b).y] = vertex1;

	VERTEX temp = *a;
	*a = *b;
	*b = temp;
}

//void exchange(VERTEX* a, VERTEX* b) {
//	VERTEX temp = *a;
//	*a = *b;
//	*b = temp;
//}

void fixHeapUp(VERTEX** heap, int fix, int*** mapHeap) {
	int parent = getParent(fix);

	if (parent < 0)
		return;

	while (fix != 0 && (*heap)[parent].lenght > (*heap)[fix].lenght) {
		exchange(&(*heap)[parent], &(*heap)[fix], &(*mapHeap));

		fix = parent;
		parent = getParent(fix);
	}
}

void insert(VERTEX** heap, int* heapSize, int key, int*** mapHeap) {
	(*heap)[(*heapSize)++].lenght = key;
	fixHeapUp(&*heap, *heapSize - 1, &(*mapHeap));
}

void decrease(VERTEX** heap, int i, int key, int*** mapHeap) {
	(*heap)[i].lenght = key;
	fixHeapUp(*&heap, i, &(*mapHeap));
}

void heapify(VERTEX** heap, int heapSize, int fix, int*** mapHeap) {
	int left = getLeft(fix);
	int right = getRight(fix);
	int min;

	if (left < heapSize && (*heap)[left].lenght < (*heap)[fix].lenght)
		min = left;
	else
		min = fix;

	if (right < heapSize && (*heap)[right].lenght < (*heap)[min].lenght)
		min = right;
	if (min != fix) {
		exchange(&(*heap)[fix], &(*heap)[min], &(*mapHeap));
		heapify(&*heap, heapSize, min, &(*mapHeap));
	}
}

VERTEX extractMin(VERTEX** heap, int* heapSize, int*** mapHeap) {
	
	//	printf("%d\n", *heapSize);
	(*heapSize)--;
	VERTEX min;
	min.x = -1;
	min.y = -1;
	if (*heapSize < 0) 
		return min;	
	if (*heapSize == 0)
		return (*heap)[0];

	//printf("Najmensi prvok je [%d %d]\n", (*heap)[0].x, (*heap)[0].y);
	//printf("Povodny index najmensieho prvku %d\n", (*mapHeap)[(*heap)[0].x][(*heap)[0].y]);
	//printf("Povodny index posledneho prvku %d\n", (*mapHeap)[(*heap)[(*heapSize)].x][(*heap)[(*heapSize)].y]);

	int index = (*mapHeap)[(*heap)[0].x][(*heap)[0].y];
	(*mapHeap)[(*heap)[0].x][(*heap)[0].y] = (*mapHeap)[(*heap)[(*heapSize)].x][(*heap)[(*heapSize)].y];
	(*mapHeap)[(*heap)[(*heapSize)].x][(*heap)[(*heapSize)].y] = index;

	min = (*heap)[0];
	(*heap)[0] = (*heap)[(*heapSize)];
	(*heap)[(*heapSize)] = min;

	heapify(*&heap, *heapSize, 0, &(*mapHeap));
	return min;
}

void search(int* heap, int* heapSize) {

}

void printHeap(VERTEX* heap, int heapSize, int*** mapHeap) {
	for (int i = 0; i < heapSize; i++)
		printf("[%d %d] %d %c | Poradie %d = %d mapHeap\n", heap[i].x, heap[i].y, heap[i].lenght, heap[i].type, i, (*mapHeap)[heap[i].x][heap[i].y]);
	printf("\n");
}

void printExtractMin(VERTEX** heap, int* heapSize, int*** mapHeap) {
	VERTEX min = extractMin(&*heap, &*heapSize, &(*mapHeap));
	printf("[%d %d] %d %c | HeapSize %d = %d Poloha\n", min.x, min.y, min.lenght, min.type, *heapSize, (*mapHeap)[min.x][min.y]);
	printHeap(*heap, *heapSize, &(*mapHeap));
}

void delete(VERTEX** heap, int* heapSize, int i, int*** mapHeap) {
	decrease(&*heap, i, -500, &(*mapHeap));
	extractMin(&*heap, &*heapSize, &(*mapHeap));
}


void transform(char** mapa) {

}

int* zachran_princezne(char** mapa, int n, int m, int t, int* dlzka_cesty) {
	// Zadanim pozadovana funkcia
	int* path = NULL;
	*dlzka_cesty = 0;

	int capacity = 2 * n * m;
	VERTEX* heap = malloc(capacity * sizeof(VERTEX));
	int heapSize = n * m;
	int index = 0;


	int** mapHeap = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++)
		mapHeap[i] = (int*)malloc(m * sizeof(int));

	int dragonX, dragonY;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++) {
				heap[index].x = i;
				heap[index].y = j;
				heap[index].lenght = INF;
				heap[index].parentX = NULL;
				heap[index].parentY = NULL;
				heap[index].type = mapa[i][j];
				mapHeap[i][j] = index;
				if (mapa[i][j] == 'D')
				{
					dragonX = i;
					dragonY = j;
				}
				index++;
			}
		}
		heap[0].lenght = 0;

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				printf("%3c", mapa[i][j]);
			}
			printf("\n");
		}
		printf("-----------------\n");
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				printf("%3d", mapHeap[i][j]);
			}
			printf("\n");
		}
		printf("-----------------\n");
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				int id = mapHeap[i][j];
				printf("%3c", heap[id].type);
			}
			printf("\n");
		}
		printf("-----------------\n");

		printExtractMin(&heap, &heapSize, &mapHeap);
		decrease(&heap, mapHeap[dragonX][dragonY], 5, &mapHeap);
		printf("-----------------\n");
		printExtractMin(&heap, &heapSize, &mapHeap);

}

//void pom2(int*** pole) {
//	printf("%d\n", (*pole)[0][0]);
//}
//
//void pom(int*** pole) {
//	printf("%d\n", (*pole)[0][0]);
//	pom2(&(*pole));
//}

int main()
{
	//int** mapHeap = (int**)malloc(5 * sizeof(int*));
	//for (int i = 0; i < 5; i++)
	//	mapHeap[i] = (int*)malloc(10 * sizeof(int));
	//int index = 100;
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 10; j++)
	//	{
	//		mapHeap[i][j] = index++;
	//	}
	//}
	//pom(&mapHeap);
	
	char** mapa;
	int i, test, dlzka_cesty, cas, * cesta;
	int n = 0, m = 0, t = 0;

		
	n = 10;	// pocet riadkov
	m = 10;	// pocet stlpcov
	t = 12;
	mapa = (char**)malloc(n * sizeof(char*));
	mapa[0] = "CCHCNHCCHN";
	mapa[1] = "NNCCCHHCCC";
	mapa[2] = "DNCCNNHHHC";
	mapa[3] = "CHHHCCCCCC";
	mapa[4] = "CCCCCNHHHH";
	mapa[5] = "PCHCCCNNNN";
	mapa[6] = "NNNNNHCCCC";
	mapa[7] = "CCCCCPCCCC";
	mapa[8] = "CCCNNHHHHH";
	mapa[9] = "HHHPCCCCCC";
	cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			
		
	cas = 0;
	//for (i = 0; i < dlzka_cesty; i++) {
	//	printf("%d %d\n", cesta[i * 2], cesta[i * 2 + 1]);
	//	if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'H')
	//		cas += 2;
	//	else
	//		cas += 1;
	//	if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'D' && cas > t)
	//		printf("Nestihol si zabit draka!\n");
	//	if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'N')
	//		printf("Prechod cez nepriechodnu prekazku!\n");
	//	if (i > 0 && abs(cesta[i * 2 + 1] - cesta[(i - 1) * 2 + 1]) + abs(cesta[i * 2] - cesta[(i - 1) * 2]) > 1)
	//		printf("Neplatny posun Popolvara!\n");
	//}
	//printf("%d\n", cas);
//	free(cesta);
	//for (i = 0; i < n; i++) {
	//	free(mapa[i]);
	//}
//	free(mapa);
	
	return 0;
}