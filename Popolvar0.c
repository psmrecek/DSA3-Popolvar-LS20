//#include <stdio.h>
//#include <stdlib.h>
//#define INF 2147483647
//
//typedef struct vertex {
//	int x, y;				// index do reprezentacie grafu
//	char status;			// binarne hodnoty stavu hladania: bit 0: je zabity drak, bit 1: je zachranena 1. princezna; bit 2: 2. princezna; ...; bit 5: 5. princezna
//	int lenght;				// dlzka cesty
//	int heapIndex;			// index do heapu
//	int parentIndex;		// index na rodica
//}VERTEX;
//
//int TOTOJETUNACO[255];		// minimalna binarna halda / min heap
//							// moze to byt globalna premenna?
//
//							// https://www.youtube.com/watch?v=lAXZGERcDf4
//
//int getParent(int i) {
//	return (i - 1) / 2;
//}
//
//int getLeft(int i) {
//	return i * 2 + 1;
//}
//
//int getRight(int i) {
//	return i * 2 + 2;
//}
//
//int getMin(int** heap) {
//	return (*heap)[0];
//}
//
//void exchange(int* x, int* y, VERTEX *a, VERTEX *b) {
//	int temp = *x;
//	*x = *y;
//	*y = temp;
//
//	VERTEX temp2 = *a;
//	int lenA = (*a).heapIndex;
//	*a = *b;
//	(*a).heapIndex = (*b).heapIndex;
//	*b = temp2;
//	(*b).heapIndex = lenA;
//}
//
//void fixHeapUp(int **heap, int fix, VERTEX *vertices) {						
//	int parent = getParent(fix);
//
//	if (parent < 0)
//		return;
//
//	while (fix != 0 && (*heap)[parent] > (*heap)[fix]) {
//		exchange(&(*heap)[parent], &(*heap)[fix], &vertices[parent], &vertices[fix]);
//
//		fix = parent;
//		parent = getParent(fix);
//	}
//}
//
//void insert(int **heap, int *heapSize, int key, VERTEX *vertices) {
//	(*heap)[(*heapSize)++] = key;
//	fixHeapUp(&*heap, *heapSize - 1, vertices);
//}
//
//void decrease(int** heap, int i, int key, VERTEX *vertices) {
//	(*heap)[i] = key;
//	fixHeapUp(*&heap, i, vertices);
//}
//
//void heapify(int** heap, int heapSize, int fix, VERTEX *vertices) {
//	int left = getLeft(fix);
//	int right = getRight(fix);
//	int min;
//
//	if (left < heapSize && (*heap)[left] < (*heap)[fix]) {
//		min = left;
//	}
//	else {
//		min = fix;
//	}
//
//	if (right < heapSize && (*heap)[right] < (*heap)[min]) {
//		min = right;
//	}
//	if (min != fix) {
//		int indexmin, indexfix, temp;
//		exchange(&(*heap)[fix], &(*heap)[min], &vertices[fix], &vertices[min]);
//		heapify(&*heap, heapSize, min, vertices);
//	}
//}
//
//int extractMin(int **heap, int *heapSize, VERTEX **vertices) {
//	(*heapSize)--;
//	if (*heapSize < 0)
//		return INF;
//	if (*heapSize == 0) 
//		return (*heap)[0];
//
//	int min = (*heap)[0];
//	(*heap)[0] = (*heap)[(*heapSize)];
//	(*vertices)[0] = (*vertices)[(*heapSize)];
//	(*vertices)[0].heapIndex = 0;
//	(*vertices)[(*heapSize)].heapIndex = *heapSize;
//	heapify(*&heap, *heapSize, 0, *&vertices);
//	return min;
//}
//
//void printHeap(int* heap, int heapSize) {
//	for (int i = 0; i < heapSize; i++)
//		printf("%d ", heap[i]);
//	printf("\n");
//}
//
////void printExtractMin(int** heap, int *heapSize) {
////	int min = extractMin(&*heap, &*heapSize);
////	printf("Min je %d\n", min);
////	printHeap(*heap, *heapSize);
////}
//
////void delete(int **heap, int *heapSize, int i) {
////	decrease(&*heap, i, -500);
////	extractMin(&*heap, &*heapSize);
////}
//
//void transform(char** mapa) {
//
//}
//
//int** djikstra() {
//
//}
//
//int* zachran_princezne(char** mapa, int n, int m, int t, int* dlzka_cesty) {
//	// Zadanim pozadovana funkcia
//	int *path = NULL;
//	*dlzka_cesty = 0;
//
//	int capacity = 2*n*m;
//	int* heap = malloc(capacity * sizeof(int));
//	int heapSize = n*m;
//	memset(heap, INF, capacity * sizeof(int));
//
//	VERTEX* vertices = malloc(capacity * sizeof(VERTEX));
////	memset(heap, 0, capacity * sizeof(VERTEX));
//
//	int dragon;
//
//	int k = 0;
//	for (int i = 0; i < n; i++)
//	{
//		for (int j = 0; j < m; j++) {
//			int index = i * m + j;
//			vertices[index].x = i;
//			vertices[index].y = j;
//			vertices[index].lenght = INF;
//			vertices[index].heapIndex = k++;
//			vertices[index].parentIndex = INF;
//			if (mapa[i][j] == 'D')
//			{
//				dragon = index;
//			}
//		}
//	}
//	int set;
//	if (mapa[0][0] == 'N')
//		printf("Na zaciatku cesty sa vyskytla prekazka!");
//	else if(mapa[0][0] == 'H')
//		set = 2;
//	else
//		set = 1;
//
//	heap[0] = set;
//	vertices[0].lenght = set;
//	vertices[0].parentIndex = NULL;
//
//	while (heapSize != 0)
//	{
//		int mapx = vertices[0].x;
//		int mapy = vertices[0].y;
//		int min = extractMin(&heap, &heapSize, &vertices);
//
//		int x1 = mapx + 1, y1 = mapy, x2 = mapx, y2 = mapy + 1, x3 = mapx - 1, y3 = mapy, x4 = mapx, y4 = mapy -1;
//
//		//if (x1 < n)
//		//{
//		//	printf("x1 a y1 %d %d\n", x1, y1);
//		//	if (mapa[x1][y1] == 'N')
//		//		printf("");
//		//	else if (mapa[x1][y1] == 'H')
//		//		set = 2;
//		//	else
//		//		set = 1;
//		//	for (int i = 0; i < n*m; i++)
//		//	{
//		//		if (vertices[i].x == x1 && vertices[i].y == y1) {
//		//			vertices[i].lenght = min + set;
//		//		}
//		//	}
//		//}
//
//		if (y2 < m)
//		{
////			printf("x2 a y2 %d %d\n", x2, y2);
//			if (mapa[x2][y2] == 'N')
//				printf("");
//			else if (mapa[x2][y2] == 'H')
//				set = 2;
//			else
//				set = 1;
//			for (int i = 0; i < n * m; i++)
//			{
//				if (vertices[i].x == x2 && vertices[i].y == y2) {
//					vertices[i].lenght = min + set;
//					heap[i] = min + set;
//					heapify(&heap, &heapSize, i, vertices);
//				}
//			}
//		}
//
//		printf("%d %d %c %d %d\n", mapx, mapy, mapa[mapx][mapy], vertices[0].heapIndex, min);
//	}
//
//
//	return NULL;
//}
//
//int main()
//{
//	char** mapa;
//	int i, test, dlzka_cesty, cas, * cesta;
//	int n = 0, m = 0, t = 0;
//
//		
//	n = 10;	// pocet riadkov
//	m = 10;	// pocet stlpcov
//	t = 12;
//	mapa = (char**)malloc(n * sizeof(char*));
//	mapa[0] = "CCHCNHCCHN";
//	mapa[1] = "NNCCCHHCCC";
//	mapa[2] = "DNCCNNHHHC";
//	mapa[3] = "CHHHCCCCCC";
//	mapa[4] = "CCCCCNHHHH";
//	mapa[5] = "PCHCCCNNNN";
//	mapa[6] = "NNNNNHCCCC";
//	mapa[7] = "CCCCCPCCCC";
//	mapa[8] = "CCCNNHHHHH";
//	mapa[9] = "HHHPCCCCCC";
//	cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
//			
//		
//	cas = 0;
//	//for (i = 0; i < dlzka_cesty; i++) {
//	//	printf("%d %d\n", cesta[i * 2], cesta[i * 2 + 1]);
//	//	if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'H')
//	//		cas += 2;
//	//	else
//	//		cas += 1;
//	//	if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'D' && cas > t)
//	//		printf("Nestihol si zabit draka!\n");
//	//	if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'N')
//	//		printf("Prechod cez nepriechodnu prekazku!\n");
//	//	if (i > 0 && abs(cesta[i * 2 + 1] - cesta[(i - 1) * 2 + 1]) + abs(cesta[i * 2] - cesta[(i - 1) * 2]) > 1)
//	//		printf("Neplatny posun Popolvara!\n");
//	//}
//	//printf("%d\n", cas);
////	free(cesta);
//	//for (i = 0; i < n; i++) {
//	//	free(mapa[i]);
//	//}
////	free(mapa);
//	
//	return 0;
//}