#include <stdio.h>
#include <stdlib.h>
#define INF 2147483647

typedef struct vertex {
	int x, y;				// indexy do reprezentacie grafu
	int parentX, parentY;	// indexy na rodica
	int lenght;				// dlzka cesty
}VERTEX;

typedef struct princess {
	int x, y;
	VERTEX *heap;
	int **mapHeap;
	int heapSize;
}PRINCESS;

int k;

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
		printf("[%d %d] %d | Poradie %d = %d mapHeap\n", heap[i].x, heap[i].y, heap[i].lenght, i, (*mapHeap)[heap[i].x][heap[i].y]);
	printf("\n");
}

void printExtractMin(VERTEX** heap, int* heapSize, int*** mapHeap) {
	VERTEX min = extractMin(&*heap, &*heapSize, &(*mapHeap));
	printf("[%d %d] %d | HeapSize %d = %d Poloha\n", min.x, min.y, min.lenght, *heapSize, (*mapHeap)[min.x][min.y]);
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

void addToArray(int** des, int* src, int desCount, int srcCount) {
	for (int i = 0; i < srcCount; i++)
	{
		(*des)[i + desCount] = src[i];
	}
}

void copyHeap(VERTEX *sHeap, VERTEX **dHeap, int **sMapHeap, int ***dMapHeap, int sHeapSize, int *dHeapSize, int n, int m) {
	int capacity = n * m;
	*dHeapSize = sHeapSize;
	VERTEX* finalHeap = malloc(capacity * sizeof(VERTEX));
	for (int i = 0; i < n * m; i++)
	{
		finalHeap[i] = sHeap[i];
	}
	int** finalMapHeap = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++) {
		finalMapHeap[i] = (int*)malloc(m * sizeof(int));
		for (int j = 0; j < m; j++)
		{
			finalMapHeap[i][j] = sMapHeap[i][j];
		}
	}
	*dHeap = finalHeap;
	*dMapHeap = finalMapHeap;
}

//int pathCounter(VERTEX *heap, int **mapHeap, int startX, int startY, int endX, int endY) {
//	int count = 0;
//	VERTEX temp = heap[mapHeap[endX][endY]];
//	while (temp.x != startX || temp.y != startY)
//	{
//		count++;
//		if (temp.parentX < 0 || temp.parentY < 0)
//		{
//			printf("K princeznej neexistuje cesta!\n");
//			printf("[%d %d] [%d %d]\n", startX, startY, endX, endY);
//			return 0;
//		}
//		else
//		{
//			temp = heap[mapHeap[temp.parentX][temp.parentY]];
//		}
//	}
//	return count;
//}

void exchange2(int* x, int* y)
{
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

int k = 0;

void permute(int* Arr, int start, int size, int*** Permutations)
{
	if (start == size) {
		for (int i = 0; i <= size; i++)
		{
			(*Permutations)[k][i] = Arr[i];
		}
		k++;
	}
	else
	{
		for (int i = start; i <= size; i++)
		{
			exchange2((Arr + start), (Arr + i));
			permute(Arr, start + 1, size, &*Permutations);
			exchange2((Arr + start), (Arr + i));
		}
	}
}

int factorial(int k) {
	int factor = 1;
	for (int i = 1; i <= k; i++)
	{
		factor *= i;
	}
	return factor;
}

int* pathCreator(VERTEX *heap, int **mapHeap, int **path, int dlzka_cesty, int startX, int startY, int endX, int endY) {
	int count = 0;
	VERTEX temp = heap[mapHeap[endX][endY]];
	int* newPath = malloc(10000 * sizeof(int));
	while (temp.x != startX || temp.y != startY)
	{
		newPath[count++] = temp.x;
		newPath[count++] = temp.y;
		if (temp.parentX < 0 || temp.parentY < 0)
		{
			printf("K princeznej neexistuje cesta!\n");
			printf("[%d %d] [%d %d]\n", startX, startY, endX, endY);
			return 0;
		}
		else
		{
			temp = heap[mapHeap[temp.parentX][temp.parentY]];
		}
	}

	reverse(&newPath, count);
	addToArray(&*path, newPath, dlzka_cesty * 2, count);
	dlzka_cesty += count / 2;
	return dlzka_cesty;
}

int* zachran_princezne(char** mapa, int n, int m, int t, int* dlzka_cesty) {
	// Zadanim pozadovana funkcia

	*dlzka_cesty = 0;

	int capacity = n * m;
	int index = 0;

	VERTEX* heap = malloc(capacity * sizeof(VERTEX));
	int heapSize = n * m;

	int** mapHeap = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++)
		mapHeap[i] = (int*)malloc(m * sizeof(int));

	int* princesses = malloc(5 * 2 * sizeof(int));
	int princessCounter = 0;

	int dragonX, dragonY;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++) {
				heap[index].x = i;
				heap[index].y = j;
				heap[index].lenght = INF;
				heap[index].parentX = -1;
				heap[index].parentY = -1;
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

		//VERTEX* dragonHeap = malloc(capacity * sizeof(VERTEX));
		//int dragonHeapSize = n * m;
		//for (int i = 0; i < dragonHeapSize; i++)
		//{
		//	dragonHeap[i] = heap[i];
		//}
		//dragonHeapSize = heapSize;

		//int** dragonMapHeap = (int**)malloc(n * sizeof(int*));
		//for (int i = 0; i < n; i++) {
		//	dragonMapHeap[i] = (int*)malloc(m * sizeof(int));
		//	for (int j = 0; j < m; j++)
		//	{
		//		dragonMapHeap[i][j] = mapHeap[i][j];
		//	}
		//}

		VERTEX* originalHeap;
		int originalHeapSize;
		int** originalMapHeap;
		copyHeap(heap, &originalHeap, mapHeap, &originalMapHeap, heapSize, &originalHeapSize, n, m);

		VERTEX* dragonHeap;
		int dragonHeapSize;
		int** dragonMapHeap;
		copyHeap(heap, &dragonHeap, mapHeap, &dragonMapHeap, heapSize, &dragonHeapSize, n, m);

		decrease(&heap, mapHeap[0][0], verticesLen(0, 0, mapa), &mapHeap);
		djikstra(&heap, &heapSize, &mapHeap, n, m, mapa);

		int* path = malloc(10000 * sizeof(int));						// Dlzku treba ale nastavovat dynamicky
		VERTEX temp = heap[mapHeap[dragonX][dragonY]];
		int count = 0;
		while (1)
		{
			path[count++] = temp.x;
			path[count++] = temp.y;

			if (temp.parentX == -1 || temp.parentY == -1)
			{
				break;
			}
			else
			{
				temp = heap[mapHeap[temp.parentX][temp.parentY]];
			}	
		}
		reverse(&path, count);
		*dlzka_cesty = count / 2;

		if (*dlzka_cesty == 1)
		{
			printf("K drakovi neexistuje cesta!\n");
			*dlzka_cesty = 0;
			return path;
		}


		decrease(&dragonHeap, dragonMapHeap[dragonX][dragonY], 1, &dragonMapHeap);
		djikstra(&dragonHeap, &dragonHeapSize, &dragonMapHeap, n, m, mapa);

		if (princessCounter == 1) {
			//int* dragonPath = malloc(1000 * sizeof(int));						// Dlzku treba ale nastavovat dynamicky
			//int count2 = 0;
			//VERTEX temp2 = dragonHeap[dragonMapHeap[princesses[0]][princesses[1]]];
			//while (temp2.x != dragonX || temp2.y != dragonY)
			//{
			//	//printf("[%d %d]\n", temp.y, temp.x);
			//	dragonPath[count2++] = temp2.x;
			//	dragonPath[count2++] = temp2.y;

			//	if (temp2.parentX < 0 || temp2.parentY < 0)
			//	{
			//		printf("K princeznej neexistuje cesta!\n");
			//		*dlzka_cesty = 0;
			//		return path;
			//	}
			//	else 
			//	{
			//		temp2 = dragonHeap[dragonMapHeap[temp2.parentX][temp2.parentY]];
			//	}
			//}
			//reverse(&dragonPath, count2);
			//addToArray(&path, dragonPath, count, count2);
			//*dlzka_cesty += count2 / 2;

			*dlzka_cesty = pathCreator(dragonHeap, dragonMapHeap, &path, *dlzka_cesty, dragonX, dragonY, princesses[0], princesses[1]);
		}
		else
		{
			PRINCESS* pr = malloc(princessCounter * sizeof(PRINCESS));
			for (int i = 0; i < princessCounter; i++)
			{
				pr[i].x = princesses[i * 2];
				pr[i].y = princesses[i * 2 + 1];
				copyHeap(originalHeap, &(pr[i].heap), originalMapHeap, &(pr[i].mapHeap), originalHeapSize, &(pr[i].heapSize), n, m);
				decrease(&(pr[i].heap), pr[i].mapHeap[princesses[i * 2]][princesses[i * 2 + 1]], 1, &(pr[i].mapHeap));
				djikstra(&(pr[i].heap), &(pr[i].heapSize), &(pr[i].mapHeap), n, m, mapa);
			}

			int matrixSize = princessCounter + 1;
			int** matrix = (int**)malloc(matrixSize * sizeof(int*));
			for (int i = 0; i < (princessCounter + 1); i++) {
				matrix[i] = (int*)malloc(matrixSize * sizeof(int));
				memset(matrix[i], 0, matrixSize * sizeof(int));
			}

			for (int i = 0; i < princessCounter; i++)
			{
				matrix[0][i + 1] = dragonHeap[dragonMapHeap[princesses[i * 2]][princesses[i * 2 + 1]]].lenght;
				matrix[i + 1][0] = matrix[0][i + 1];
			}

			for (int i = 0; i < princessCounter; i++)
			{
				for (int j = i + 1; j < princessCounter; j++)
				{
					matrix[i + 1][j + 1] = pr[i].heap[pr[i].mapHeap[princesses[j * 2]][princesses[j * 2 + 1]]].lenght;
					matrix[j + 1][i + 1] = matrix[i + 1][j + 1];
				}
			}

			//for (int i = 0; i < matrixSize; i++)
			//{
			//	for (int j = 0; j < matrixSize; j++)
			//	{
			//		printf("%2d ", matrix[i][j]);
			//	}
			//	printf("\n");
			//}
			//printf("\n");

			int newSize = matrixSize - 1;
			int* Arr = malloc(newSize * sizeof(int));
			for (int i = 0; i < newSize; i++)
			{
				Arr[i] = i + 1;
			}


			int factor = factorial(newSize);
			int** Permutations = (int**)malloc(factor * sizeof(int*));
			for (int i = 0; i < factor; i++) {
				Permutations[i] = (int*)malloc(newSize * sizeof(int));
				//		memset(matrix[i], 0, newSize * sizeof(int));
			}

			k = 0;
			permute(Arr, 0, newSize - 1, &Permutations);

			//printf("-------------\n");
			//for (int i = 0; i < factor; i++)
			//{
			//	for (int j = 0; j < newSize; j++)
			//	{
			//		printf("%d ", Permutations[i][j]);
			//	}
			//	printf("\n");
			//}

			//printf("-------------\n");

			int min = INF;
			int* configuration = malloc(matrixSize * sizeof(int));

			int* tempArray = malloc(matrixSize * sizeof(int));
			for (int i = 0; i < factor; i++)
			{
				tempArray[0] = 0;
				for (int j = 1; j < matrixSize; j++)
				{
					tempArray[j] = Permutations[i][j - 1];
				}
				
				int l = 1;
				int tempMin = 0;
				for (int k = 0; k < matrixSize - 1; k++)
				{
					tempMin += matrix[tempArray[k]][tempArray[l]];
//					printf("%d ", matrix[tempArray[k]][tempArray[l]]);
					l++;
				}
				if (min > tempMin)
				{
					min = tempMin;
					for (int i = 0; i < matrixSize; i++)
					{
						configuration[i] = tempArray[i];
					}
				}
			}
			
			int princessIndex = configuration[1] - 1;
			int prinX = princesses[princessIndex * 2];
			int prinY = princesses[princessIndex * 2 + 1];
//			printf("%d %d\n", prinX, prinY);
			*dlzka_cesty = pathCreator(dragonHeap, dragonMapHeap, &path, *dlzka_cesty, dragonX, dragonY, prinX, prinY);

			for (int i = 2; i < matrixSize; i++)
			{
				int oldPrincessIndex = configuration[i - 1] - 1;
				princessIndex = configuration[i] - 1;

				prinX = princesses[princessIndex * 2];
				prinY = princesses[princessIndex * 2 + 1];

//				printf("%d %d\n", pr[oldPrincessIndex].x, pr[oldPrincessIndex].y);
//				printf("%d %d\n", prinX, prinY);
				*dlzka_cesty = pathCreator(pr[oldPrincessIndex].heap, pr[oldPrincessIndex].mapHeap, &path, *dlzka_cesty, pr[oldPrincessIndex].x, pr[oldPrincessIndex].y, prinX, prinY);

//				printf("%d %d\n", prinX, prinY);
			}
//			printf("-------------\n");
		}

//		printf("%d\n", princessCounter);
		return path;
}

int main()
{
	char** mapa;
	int i, test, dlzka_cesty, cas, * cesta;
	int n = 0, m = 0, t = 0;
	FILE* f;

	//n = 10;
	//m = 10;
	//t = 12;
	//mapa = (char**)malloc(n * sizeof(char*));
	//mapa[0] = "CCHCNHCCHN";
	//mapa[1] = "NNCCCHHCCC";
	//mapa[2] = "DNCCNNHHHC";
	//mapa[3] = "CHHHCCCCCC";
	//mapa[4] = "CCCCCNHHHH";
	//mapa[5] = "PCHCCCNNNN";
	//mapa[6] = "NNNNNHCCCC";
	//mapa[7] = "CCCCCPCCCC";
	//mapa[8] = "CCCNNHHHHH";
	//mapa[9] = "HHHPCCCCCC";
	//cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			
//	f = fopen("jednaPrincezna.txt", "r");
//	f = fopen("kritickaMapa.txt", "r");
//	f = fopen("test.txt", "r");

	//f = fopen("vstup1.txt", "r");
	//f = fopen("vstup2.txt", "r");
	f = fopen("vstup3.txt", "r");
	if (f)
		fscanf(f, "%d %d %d", &n, &m, &t);
	else
		return 0;
	mapa = (char**)malloc(n * sizeof(char*));
	for (i = 0; i < n; i++) {
		mapa[i] = (char*)malloc(m * sizeof(char));
		for (int j = 0; j < m; j++) {
			char policko = fgetc(f);
			if (policko == '\n') policko = fgetc(f);
			mapa[i][j] = policko;
		}
	}
	fclose(f);
	cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);

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