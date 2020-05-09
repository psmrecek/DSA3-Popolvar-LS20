#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define INF 2147483647

typedef struct vertex {		// struktura vrcholu v halde
	int x, y;				// indexy do reprezentacie grafu
	int parentX, parentY;	// indexy na rodica
	int lenght;				// dlzka cesty
}VERTEX;

typedef struct princess {	// struktura konkretnej princeznej
	int x, y;				// indexy princeznej na mape
	VERTEX *heap;			// smernik na binarnu haldu konkretnej princeznej
	int **mapHeap;			// smernik na mapu binarnej haldy konkretnej princeznej
	int heapSize;			// velkost binarnej haldy konkretnej princeznej
}PRINCESS;

int numberOfPermutations;	// globalna premenna urcujuca pocet permutacii ciest k princeznam

// Funkcie pre binarnu haldu

int getParent(int i) {		// vrati index rodica prvku v binarnej halde
	return (i - 1) / 2;
}

int getLeft(int i) {		// vrati index laveho potomka prvku v binarnej halde
	return i * 2 + 1;
}

int getRight(int i) {		// vrati index praveho potomka v binarnej halde
	return i * 2 + 2;
}

int getMin(VERTEX** heap) {	// vrati minimalny prvok v binarnej halde (pracujem s haldou vrcholou, ktora je zoradena podla dlzky cesty k vrcholu)
	return (*heap)[0].lenght;
}

void exchange(VERTEX* a, VERTEX* b, int*** mapHeap) {
							// vymena 2 vrcholov v halde cez smerniky
	int vertex1 = (*mapHeap)[(*a).x][(*a).y];
	(*mapHeap)[(*a).x][(*a).y] = (*mapHeap)[(*b).x][(*b).y];
	(*mapHeap)[(*b).x][(*b).y] = vertex1;

	VERTEX temp = *a;
	*a = *b;
	*b = temp;
}

void fixHeapUp(VERTEX** heap, int fix, int*** mapHeap) {
							// oprava binarnej haldy po vlozeni prvku do haldy
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
							// vlozenie prvku do haldy
	(*heap)[(*heapSize)++].lenght = key;
	fixHeapUp(&*heap, *heapSize - 1, &(*mapHeap));
}

void decrease(VERTEX** heap, int i, int key, int*** mapHeap) {
							// zmensenie kluca konkretneho prvku v halde a nasledna oprava haldy
	(*heap)[i].lenght = key;
	fixHeapUp(*&heap, i, &(*mapHeap));
}

void heapify(VERTEX** heap, int heapSize, int fix, int*** mapHeap) {
							// funkcia heapify pre binarnu haldu vrcholov
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
							// odstranenie minimalneho - tj prveho prvku z binarnej haldy
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
							// pomocna funkcia na vypis aktualne celej haldy
	for (int i = 0; i < heapSize; i++)
		printf("[%d %d] %d | Poradie %d = %d mapHeap\n", heap[i].x, heap[i].y, heap[i].lenght, i, (*mapHeap)[heap[i].x][heap[i].y]);
	printf("\n");
}

void printExtractMin(VERTEX** heap, int* heapSize, int*** mapHeap) {
							// pomocna funkcia na vypis haldy po odstraneni minimalneho prvku
	VERTEX min = extractMin(&*heap, &*heapSize, &(*mapHeap));
	printf("[%d %d] %d | HeapSize %d = %d Poloha\n", min.x, min.y, min.lenght, *heapSize, (*mapHeap)[min.x][min.y]);
	printHeap(*heap, *heapSize, &(*mapHeap));
}

void delete(VERTEX** heap, int* heapSize, int i, int*** mapHeap) {
							// odstranenie konkretneho prvku z haldy - tomuto prvku nastavi najmensia mozna hodnota a potom sa vymaze najmensi prvok
	decrease(&*heap, i, -INF, &(*mapHeap));
	VERTEX temp = extractMin(&*heap, &*heapSize, &(*mapHeap));
}

// Funkcie pre Popolvara

void reverse(int** path, int lenght) {
							// obratenie pola s cestou
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

void transformMapToHeap(VERTEX** heap, int* heapSize, int*** mapHeap, int n, int m, char** mapa, int* dragonX, int* dragonY, int** princesses, int* princessCounter) {
							// pretransformovanie grafu z mapy do binarnej haldy
	int index = 0;
	for (int i = 0; i < n; i++)								// cela mapa sa prepise do binarnej haldy vrcholov a inicializuju sa informacie o vrchole
	{
		for (int j = 0; j < m; j++) {
			(*heap)[index].x = i;
			(*heap)[index].y = j;
			(*heap)[index].lenght = INF;
			(*heap)[index].parentX = -1;
			(*heap)[index].parentY = -1;
			(*mapHeap)[i][j] = index;
			if (mapa[i][j] == 'D')
			{
				*dragonX = i;
				*dragonY = j;
			}
			if (mapa[i][j] == 'P')
			{
				(*princesses)[(*princessCounter)++] = i;
				(*princesses)[(*princessCounter)++] = j;
			}
			index++;
		}
	}
	(*princessCounter) /= 2;

	for (int i = 0; i < n; i++)								// Vymazanie nepriechodnych prekazok z haldy
	{
		for (int j = 0; j < m; j++)
		{
			if (mapa[i][j] == 'N') {
				delete(&(*heap), &(*heapSize), (*mapHeap)[i][j], &(*mapHeap));
			}
		}
	}
}

void pathToDragonCreator(int** path, char** mapa, int dragonX, int dragonY, VERTEX* heap, int heapSize, int** mapHeap, int* dlzka_cesty) {
							// najdenie cesty k drakovi spatnym prehladavanim z haldy
	int count = 0;
	VERTEX temp = heap[mapHeap[dragonX][dragonY]];

	while (1)
	{
		(*path)[count++] = temp.x;
		(*path)[count++] = temp.y;

		if (temp.parentX == -1 || temp.parentY == -1)
		{
			if (!(temp.x == 0 && temp.y == 0))
			{
				*dlzka_cesty = 0;
				return;
			}
			break;
		}
		else
		{
			temp = heap[mapHeap[temp.parentX][temp.parentY]];
		}
	}
	reverse(&(*path), count);
	*dlzka_cesty = count / 2;
}

void top(VERTEX min, int* x, int* y) {
							// posun na mape hore
	*x = min.x - 1;
	*y = min.y;
}

void right(VERTEX min, int* x, int* y) {
							// posun na mape doprava
	*x = min.x;
	*y = min.y + 1;
}

void bottom(VERTEX min, int* x, int* y) {
							// posun na mape dolu
	*x = min.x + 1;
	*y = min.y;
}

void left(VERTEX min, int* x, int* y) {
							// posun na mape dolava
	*x = min.x;
	*y = min.y - 1;
}

int validXY(int x, int y, int n, int m, int heapSize, int** mapHeap) {
							// funkcia kontrolujuca platnost novej suradnice, tj ze sa nenachadza policko mimo mapy a zaroven je v halde
	if (x < n && x >= 0 && y < m && y >= 0 && mapHeap[x][y] <= heapSize)
		return 1;
	return 0;
}

int verticesLen(int x, int y, char** mapa) {
							// funkcia na vratenie casovej dlzky policka
	char map = mapa[x][y];
	if (map == 'N')
		return INF;
	else if (map == 'H')
		return 2;
	else
		return 1;
}

void vertextNewLenghtDjikstra(VERTEX** heap, int* heapSize, int*** mapHeap, int n, int m, char** mapa, int newX, int newY, VERTEX* min) {
							// nastavenie novej dlzky pre vrchol v Djikstrovom algoritme
	if (validXY(newX, newY, n, m, (*heapSize), (*mapHeap))) {
		int id = (*mapHeap)[newX][newY];
		int newLenght = (*min).lenght + verticesLen(newX, newY, mapa);
		if ((*heap)[id].lenght > newLenght)
		{
			(*heap)[id].parentX = (*min).x;
			(*heap)[id].parentY = (*min).y;
			decrease(&(*heap), id, newLenght, &(*mapHeap));
		}
	}
}

void djikstra(VERTEX** heap, int* heapSize, int*** mapHeap, int n, int m, char **mapa) {
							// Djikstrov algoritmus
	while ((*heapSize) != 0) {

		VERTEX min = extractMin(&(*heap), &(*heapSize), &(*mapHeap));
		int index = (*mapHeap)[min.x][min.y];
		int tx, ty, rx, ry, bx, by, lx, ly;

		top(min, &tx, &ty);
		right(min, &rx, &ry);
		bottom(min, &bx, &by);
		left(min, &lx, &ly);

		vertextNewLenghtDjikstra(&(*heap), &(*heapSize), &(*mapHeap), n, m, mapa, tx, ty, &min);
		vertextNewLenghtDjikstra(&(*heap), &(*heapSize), &(*mapHeap), n, m, mapa, rx, ry, &min);
		vertextNewLenghtDjikstra(&(*heap), &(*heapSize), &(*mapHeap), n, m, mapa, bx, by, &min);
		vertextNewLenghtDjikstra(&(*heap), &(*heapSize), &(*mapHeap), n, m, mapa, lx, ly, &min);
	}
}

void addToArray(int** des, int* src, int desCount, int srcCount) {
							// pridanie zdrojoveho pola do cieloveho pola - spojenie 2 poli suradnic
	for (int i = 0; i < srcCount; i++)
		(*des)[i + desCount] = src[i];
}

void copyHeap(VERTEX *sHeap, VERTEX **dHeap, int **sMapHeap, int ***dMapHeap, int sHeapSize, int *dHeapSize, int n, int m) {
							// iterativne kopirovanie haldy bez nutnosti jej opatovneho vytvarania
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

void exchange2(int* x, int* y){
							// vymena 2 integerov cez smerniky
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

void permute(int* Arr, int start, int size, int*** Permutations){
							// rekurzivna funkcia vytvarajuca pole permutacii
	if (start == size) {
		for (int i = 0; i <= size; i++)
		{
			(*Permutations)[numberOfPermutations][i] = Arr[i];
		}
		numberOfPermutations++;
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
							// iterativne ratanie hodnoty faktorialu
	int factor = 1;
	for (int i = 1; i <= k; i++)
	{
		factor *= i;
	}
	return factor;
}

int* princessPathCreator(VERTEX *heap, int **mapHeap, int **path, int dlzka_cesty, int startX, int startY, int endX, int endY) {
							// funkcia na vytvorenie cesty medzi 2 princeznami pomocou binarnej hlady
	int count = 0;
	VERTEX temp = heap[mapHeap[endX][endY]];
	int* newPath = malloc(10000 * sizeof(int));
	while (temp.x != startX || temp.y != startY)			// spatne vracanie sa haldou
	{
		newPath[count++] = temp.x;
		newPath[count++] = temp.y;
		if (temp.parentX < 0 || temp.parentY < 0)
		{
			printf("K princeznej neexistuje cesta!\n");
			//printf("[%d %d] [%d %d]\n", startX, startY, endX, endY);
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

void findBestConfiguration(int matrixSize, int princessCounter, VERTEX* dragonHeap, int** dragonMapHeap, int* princesses, PRINCESS* pr, int** configuration) {
							// najdenie najlepsieho poradia navsivenia princezien

	int** matrix = (int**)malloc(matrixSize * sizeof(int*));			// vytvorenie prazdnej matice susednosti
	for (int i = 0; i < (princessCounter + 1); i++) {
		matrix[i] = (int*)malloc(matrixSize * sizeof(int));
		memset(matrix[i], 0, matrixSize * sizeof(int));
	}

	for (int i = 0; i < princessCounter; i++)							// nastavenie hodnot - dlzok ciest v matici susednosti medzi drakom a princeznami
	{
		matrix[0][i + 1] = dragonHeap[dragonMapHeap[princesses[i * 2]][princesses[i * 2 + 1]]].lenght;
		matrix[i + 1][0] = matrix[0][i + 1];
	}

	for (int i = 0; i < princessCounter; i++)							// nastavenie hodnot - dlzok ciest v matici susednosti medzi princeznami navzajom
	{
		for (int j = i + 1; j < princessCounter; j++)
		{
			matrix[i + 1][j + 1] = pr[i].heap[pr[i].mapHeap[princesses[j * 2]][princesses[j * 2 + 1]]].lenght;
			matrix[j + 1][i + 1] = matrix[i + 1][j + 1];
		}
	}

	int newSize = matrixSize - 1;										// vytvorenie pomocneho pola s indexami princezien
	int* Arr = malloc(newSize * sizeof(int));
	for (int i = 0; i < newSize; i++)
	{
		Arr[i] = i + 1;
	}

	int factor = factorial(newSize);									// vytvorenie pola permutacii
	int** Permutations = (int**)malloc(factor * sizeof(int*));
	for (int i = 0; i < factor; i++) {
		Permutations[i] = (int*)malloc(newSize * sizeof(int));
	}

	numberOfPermutations = 0;											// vynulovanie globalnej premennej urcujucej pocet permutacii
	permute(Arr, 0, newSize - 1, &Permutations);						// vytvorenie pola permutacii z povodneho pola princezien

	int min = INF;

	int* tempArray = malloc(matrixSize * sizeof(int));

	for (int i = 0; i < factor; i++)									// najdenie najlacnejsej permutacie v poli permutacii
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
			l++;
		}
		if (min > tempMin)
		{
			min = tempMin;
			for (int i = 0; i < matrixSize; i++)
			{
				(*configuration)[i] = tempArray[i];
			}
		}
	}
}

int* zachran_princezne(char** mapa, int n, int m, int t, int* dlzka_cesty) {
							// Zadanim pozadovana funkcia

	*dlzka_cesty = 0;										// Nastavenie dlzky cesty na 0 a nasledne alokovanie pomocnych premennych a poli

	VERTEX* heap = malloc(n * m * sizeof(VERTEX));
	int heapSize = n * m;

	int** mapHeap = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++)
		mapHeap[i] = (int*)malloc(m * sizeof(int));

	int* princesses = malloc(5 * 2 * sizeof(int));
	int princessCounter = 0;

	int dragonX = -1, dragonY = -1;

	int* path = malloc(30000 * sizeof(int));

	transformMapToHeap(&heap, &heapSize, &mapHeap, n, m, mapa, &dragonX, &dragonY, &princesses, &princessCounter);
															// Transformacia grafu z mapy to haldy

	if (mapa[0][0] == 'N')
	{
		printf("Popolvar sa nedostal na mapu, pretoze zacina nepriechodnou prekazkou!\n");
		*dlzka_cesty = 0;
		return NULL;
	}

	if (dragonX == -1 || dragonY == -1)
	{
		printf("Na mape sa nenachadza drak!\n");
		*dlzka_cesty = 0;
		return NULL;
	}

	VERTEX* originalHeap;									// Zalohovanie vytvorenej haldy pre setrenie casu potrebneho na vytvaranie novej
	int originalHeapSize;
	int** originalMapHeap;
	copyHeap(heap, &originalHeap, mapHeap, &originalMapHeap, heapSize, &originalHeapSize, n, m);

	VERTEX* dragonHeap;										// Vytvorenie haldy pre draka
	int dragonHeapSize;
	int** dragonMapHeap;
	copyHeap(heap, &dragonHeap, mapHeap, &dragonMapHeap, heapSize, &dragonHeapSize, n, m);

	decrease(&heap, mapHeap[0][0], verticesLen(0, 0, mapa), &mapHeap);	// Nastavenie policka (0, 0) ako zaciatok mapy 
	djikstra(&heap, &heapSize, &mapHeap, n, m, mapa);					// Djikstrov algoritmus na najdenie najkratsich ciest z policka (0, 0)
	
	pathToDragonCreator(&path, mapa, dragonX, dragonY, heap, heapSize, mapHeap, &*dlzka_cesty);
																		// Vytvorenie a najdenie cesty z policka (0, 0) k drakovi
	if (*dlzka_cesty == 0)
	{
		printf("K drakovi neexistuje cesta!\n");
		*dlzka_cesty = 0;
		return NULL;
	}

	if (heap[mapHeap[dragonX][dragonY]].lenght > t)
	{
		printf("Drak sa zobudil a zjedol princezne!\n");
		*dlzka_cesty = 0;
		return NULL;
	}

	decrease(&dragonHeap, dragonMapHeap[dragonX][dragonY], 1, &dragonMapHeap);	// Nastavenie policka s drakom ako zaciatok cesty
	djikstra(&dragonHeap, &dragonHeapSize, &dragonMapHeap, n, m, mapa);			// Spustenie djikstru z policka draka

	if (princessCounter == 1) 
	{	// Ak je pocet princezien 1, vytvori priamo cestu od draka k princeznej a funkcia konci
		*dlzka_cesty = princessPathCreator(dragonHeap, dragonMapHeap, &path, *dlzka_cesty, dragonX, dragonY, princesses[0], princesses[1]);
	}
	else 
	{	// Ak je princezien viac ako 1, vytvori pole princezien, kde kazda princezna ma svoju haldu s pociatocnym polickom rovnakym ako poloha princeznej na mape
		PRINCESS* pr = malloc(princessCounter * sizeof(PRINCESS));
		for (int i = 0; i < princessCounter; i++)
		{
			pr[i].x = princesses[i * 2];
			pr[i].y = princesses[i * 2 + 1];
			copyHeap(originalHeap, &(pr[i].heap), originalMapHeap, &(pr[i].mapHeap), originalHeapSize, &(pr[i].heapSize), n, m);
			decrease(&(pr[i].heap), pr[i].mapHeap[princesses[i * 2]][princesses[i * 2 + 1]], 1, &(pr[i].mapHeap));
			djikstra(&(pr[i].heap), &(pr[i].heapSize), &(pr[i].mapHeap), n, m, mapa);
		}

		int matrixSize = princessCounter + 1;									// Alokovanie matice susednosti
		int* configuration = malloc(matrixSize * sizeof(int));					// Alokovanie pola hladania najlepsej konfiguracie

		findBestConfiguration(matrixSize, princessCounter, dragonHeap, dragonMapHeap, princesses, pr, &configuration);
																				// Najdenie najlepsej konfiguracie funkciou

		int princessIndex = configuration[1] - 1;								// Najdenie ktora princezna je v konfiguracii prva a jej suradnice
		int prinX = princesses[princessIndex * 2];
		int prinY = princesses[princessIndex * 2 + 1];

		*dlzka_cesty = princessPathCreator(dragonHeap, dragonMapHeap, &path, *dlzka_cesty, dragonX, dragonY, prinX, prinY);
																				// Najdenie cesty medzi drakom a prvou princeznou
		if (*dlzka_cesty == 0)
		{
			return NULL;
		}

		for (int i = 2; i < matrixSize; i++)									// najdenie cesty medzi minule navstivenou a nasledujucou princeznou
		{
			int oldPrincessIndex = configuration[i - 1] - 1;
			princessIndex = configuration[i] - 1;

			prinX = princesses[princessIndex * 2];
			prinY = princesses[princessIndex * 2 + 1];

			*dlzka_cesty = princessPathCreator(pr[oldPrincessIndex].heap, pr[oldPrincessIndex].mapHeap, &path, *dlzka_cesty, pr[oldPrincessIndex].x, pr[oldPrincessIndex].y, prinX, prinY);
			if (*dlzka_cesty == 0)
			{
				return NULL;
			}
		}
	}
	return path;
}

void mapaZoSuboru(char ***mapa, int n, int m, FILE* f) {
	(*mapa) = (char**)malloc(n * sizeof(char*));
	int i;
	for (i = 0; i < n; i++) {
		(*mapa)[i] = (char*)malloc(m * sizeof(char));
		for (int j = 0; j < m; j++) {
			char policko = fgetc(f);
			if (policko == '\n') policko = fgetc(f);
			(*mapa)[i][j] = policko;
		}
	}
}

int main()
{
	char** mapa;
	int i, test, dlzka_cesty, cas, * cesta;
	int n = 0, m = 0, t = 0;
	FILE* f;
	while (1) {
		printf("------------------------------\nZadajte cislo testu (0 ukonci program):\n");
		printf(" 1 - vzorovy test - nacitanie preddefinovanej mapy 10x10 P3 zo suboru\n 2 - minimalna mapa 2x1 P1\n 3 - minimalna mapa 1x2 P1\n"
			" 4 - mapa 15x8 P1\n 5 - mapa 30x22 P2\n 6 - mapa 25x35 P3\n 7 - mapa 50x75 P4\n"
			" 8 - mapa 42x70 P5\n 9 - maximalna mapa 100x100 P1\n10 - maximalna mapa 100x100 P3\n"
			"11 - maximalna mapa 100x100 P5\n12 - ina maximalna mapa 100x100 P5\n13 - mapa 6x15 P3 pre ukazku spravneho poradia zachrany\n"
			"14 - mapa 11x11 P5 s nedostatkom casu na zabitie draka\n15 - mapa 15x15 P5 - nedostupna princezna\n"
			"16 - mapa 9x9 P3 - nedostupny drak\n------------------------------\n");
		scanf("%d", &test);
		dlzka_cesty = 0;
		n = m = t = 0;
		switch (test) {
		case 0://ukonci program
			return 0;
		case 1://nacitanie mapy zo suboru
			f = fopen("1_vzorovyVstup.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 2: //pridajte vlastne testovacie vzorky
			f = fopen("2_2x1-1minimalnaMapa.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 3: //pridajte vlastne testovacie vzorky
			f = fopen("3_1x2-1minimalnaMapa.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 4: //pridajte vlastne testovacie vzorky
			f = fopen("4_15x8-1.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 5: //pridajte vlastne testovacie vzorky
			f = fopen("5_30x22-2.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 6: //pridajte vlastne testovacie vzorky
			f = fopen("6_25x35-3.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 7: //pridajte vlastne testovacie vzorky
			f = fopen("7_50x75-4.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 8: //pridajte vlastne testovacie vzorky
			f = fopen("8_42x70-5.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 9: //pridajte vlastne testovacie vzorky
			f = fopen("9_100x100-1.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 10: //pridajte vlastne testovacie vzorky
			f = fopen("10_100x100-3.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 11: //pridajte vlastne testovacie vzorky
			f = fopen("11_100x100-5.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 12: //pridajte vlastne testovacie vzorky
			f = fopen("12_100x100-5ina.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 13: //pridajte vlastne testovacie vzorky
			f = fopen("13_6x15-3spravnePoradie.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 14: //pridajte vlastne testovacie vzorky
			f = fopen("14_11x11-5nedostatokCasu.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 15: //pridajte vlastne testovacie vzorky
			f = fopen("15_15x15-5nedostupnaPrincezna.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 16: //pridajte vlastne testovacie vzorky
			f = fopen("16_9x9-3nedostupnyDrak.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapaZoSuboru(&mapa, n, m, f);
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		default:
			continue;
		}

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
	}
	return 0;
}

