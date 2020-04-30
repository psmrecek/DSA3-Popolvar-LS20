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

// Funkcie pre binarnu haldu

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

	int index = (*mapHeap)[(*heap)[0].x][(*heap)[0].y];
	(*mapHeap)[(*heap)[0].x][(*heap)[0].y] = (*mapHeap)[(*heap)[(*heapSize)].x][(*heap)[(*heapSize)].y];
	(*mapHeap)[(*heap)[(*heapSize)].x][(*heap)[(*heapSize)].y] = index;

	min = (*heap)[0];
	(*heap)[0] = (*heap)[(*heapSize)];
	(*heap)[(*heapSize)] = min;

	heapify(*&heap, *heapSize, 0, &(*mapHeap));
	return min;
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
	decrease(&*heap, i, -INF, &(*mapHeap));
	extractMin(&*heap, &*heapSize, &(*mapHeap));
}

// Funkcie pre Popolvara

void top(VERTEX min, int* x, int* y) {
	*x = min.x - 1;
	*y = min.y;
}

void right(VERTEX min, int* x, int* y) {
	*x = min.x;
	*y = min.y + 1;
}

void bottom(VERTEX min, int* x, int* y) {
	*x = min.x + 1;
	*y = min.y;
}

void left(VERTEX min, int* x, int* y) {
	*x = min.x;
	*y = min.y - 1;
}

int validXY(int x, int y, int n, int m, int heapSize, int** mapHeap) {
	if (x < n && x >= 0 && y < m && y >= 0 && mapHeap[x][y] <= heapSize)
		return 1;
	return 0;
}

int verticesLen(int x, int y, char** mapa) {
	int set;
	char map = mapa[x][y];
	if (map == 'N')
		return INF;
	else if (map == 'H')
		return 2;
	else
		return 1;
}

void reverse(int **path, int lenght) {
	lenght--;
	int temp, start = 0;
	while (start < lenght) {
		temp = (*path)[start];
		(*path)[start] = (*path)[lenght];
		(*path)[lenght] = temp;
		start++;
		lenght--;
	}
}

void createGraph(){
}

void djikstra(VERTEX** heap, int* heapSize, int*** mapHeap, int n, int m, char **mapa) {
	while ((*heapSize) != 0) {
		VERTEX min = extractMin(&(*heap), &(*heapSize), &(*mapHeap));
		int index = (*mapHeap)[min.x][min.y];
		int tx, ty, rx, ry, bx, by, lx, ly;
		top(min, &tx, &ty);
		right(min, &rx, &ry);
		bottom(min, &bx, &by);
		left(min, &lx, &ly);

		//			printf("[%d %d] ma platnych susedov ", min.x, min.y);

		if (validXY(tx, ty, n, m, (*heapSize), (*mapHeap))) {
			//				printf("t[%d %d] ", tx, ty);
			int id = (*mapHeap)[tx][ty];
			int newLenght = min.lenght + verticesLen(tx, ty, mapa);
			if ((*heap)[id].lenght > newLenght)
			{
				//heap[id].lenght = newLenght;
				(*heap)[id].parentX = min.x;
				(*heap)[id].parentY = min.y;
				decrease(&(*heap), id, newLenght, &(*mapHeap));
			}
		}
		if (validXY(rx, ry, n, m, (*heapSize), (*mapHeap))) {
			//				printf("r[%d %d] ", rx, ry);
			int id = (*mapHeap)[rx][ry];
			int newLenght = min.lenght + verticesLen(rx, ry, mapa);
			if ((*heap)[id].lenght > newLenght)
			{
				//heap[id].lenght = newLenght;
				(*heap)[id].parentX = min.x;
				(*heap)[id].parentY = min.y;
				decrease(&(*heap), id, newLenght, &(*mapHeap));
			}
		}
		if (validXY(bx, by, n, m, (*heapSize), (*mapHeap))) {
			//				printf("b[%d %d] ", bx, by);
			int id = (*mapHeap)[bx][by];
			int newLenght = min.lenght + verticesLen(bx, by, mapa);
			if ((*heap)[id].lenght > newLenght)
			{
				//heap[id].lenght = newLenght;
				(*heap)[id].parentX = min.x;
				(*heap)[id].parentY = min.y;
				decrease(&(*heap), id, newLenght, &(*mapHeap));
			}
		}
		if (validXY(lx, ly, n, m, (*heapSize), (*mapHeap))) {
			//				printf("l[%d %d] ", lx, ly);
			int id = (*mapHeap)[lx][ly];
			int newLenght = min.lenght + verticesLen(lx, ly, mapa);
			if ((*heap)[id].lenght > newLenght)
			{
				//heap[id].lenght = newLenght;
				(*heap)[id].parentX = min.x;
				(*heap)[id].parentY = min.y;
				decrease(&(*heap), id, newLenght, &(*mapHeap));
			}
		}
		//printf("\n");
	}
}

int* zachran_princezne(char** mapa, int n, int m, int t, int* dlzka_cesty) {
	// Zadanim pozadovana funkcia
	int* path = malloc(1000*sizeof(int));						// Dlzku treba ale nastavovat dynamicky
	*dlzka_cesty = 0;

	int capacity = n * m;
	int index = 0;

	VERTEX* heap = malloc(capacity * sizeof(VERTEX));
	int heapSize = n * m;
	int** mapHeap = (int**)malloc(n * sizeof(int*));

	int* princesses = malloc(5 * 2 * sizeof(int));
	int princessCounter = 0;
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
				if (mapa[i][j] == 'P')
				{
					princesses[princessCounter++] = i;
					princesses[princessCounter++] = j;
				}
				index++;
			}
		}
		princessCounter /= 2;
		
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				if (mapa[i][j] == 'N') {
					delete(&heap, &heapSize, mapHeap[i][j], &mapHeap);
				}
			}
		}
		
		decrease(&heap, mapHeap[0][0], verticesLen(0, 0, mapa), &mapHeap);
		djikstra(&heap, &heapSize, &mapHeap, n, m, mapa);


		VERTEX temp = heap[mapHeap[dragonX][dragonY]];
		int count = 0;
		while (temp.x != 0 || temp.y != 0)
		{
			path[count++] = temp.x;
			path[count++] = temp.y;

			temp = heap[mapHeap[temp.parentX][temp.parentY]];
		}
		path[count++] = 0;
		path[count++] = 0;
		*dlzka_cesty = count / 2;

		//for (int i = 0; i < n; i++)
		//{
		//	//for (int j = 0; j < m; j++)
		//	//{
		//	//	printf("%3c", mapa[i][j]);
		//	//}
		//	printf("%s\n", mapa[i]);
		//}

		reverse(&path, count);
		return path;
}

int main()
{
	char** mapa;
	int i, test, dlzka_cesty, cas, * cesta;
	int n = 0, m = 0, t = 0;
	FILE* f;

	n = 10;
	m = 10;
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
			
	//f = fopen("vstup3.txt", "r");
	//if (f)
	//	fscanf(f, "%d %d %d", &n, &m, &t);
	//else
	//	return 0;
	//mapa = (char**)malloc(n * sizeof(char*));
	//for (i = 0; i < n; i++) {
	//	mapa[i] = (char*)malloc(m * sizeof(char));
	//	for (int j = 0; j < m; j++) {
	//		char policko = fgetc(f);
	//		if (policko == '\n') policko = fgetc(f);
	//		mapa[i][j] = policko;
	//	}
	//}
	//fclose(f);
	//cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);


	cas = 0;
	for (i = 0; i < dlzka_cesty; i++) {
		printf("%d %d\n", cesta[i * 2], cesta[i * 2 + 1]);
		if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'H')
			cas += 2;
		else
			cas += 1;
		if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'D' && cas > t)
			printf("Nestihol si zabit draka!\n");
		if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'N')
			printf("Prechod cez nepriechodnu prekazku!\n");
		if (i > 0 && abs(cesta[i * 2 + 1] - cesta[(i - 1) * 2 + 1]) + abs(cesta[i * 2] - cesta[(i - 1) * 2]) > 1)
			printf("Neplatny posun Popolvara!\n");
	}
	printf("%d\n", cas);
	free(cesta);
	for (i = 0; i < n; i++) {
		free(mapa[i]);
	}
	free(mapa);
	return 0;
}