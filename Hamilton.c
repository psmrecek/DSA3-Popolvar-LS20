//#include <stdio.h>
//#include <stdlib.h>
//
//void NextVertex(int k, int** matrix, int matrixSize, int** Arr) {
////	printf("NextVertex\n");
//	do
//	{
////		printf("NextVertex\n");
//		int i;
//		(*Arr)[k] = ((*Arr)[k] + 1) % (matrixSize + 1);
////		printf("k %d Arr[k] %d\n", k, (*Arr)[k]);
//		if ((*Arr) == 0)
//		{
//			return;
//		}
//		if (matrix[(*Arr)[k - 1]][(*Arr)[k]] != 0)
//		{
//			for (i = 1; i < k - 1; i++)
//			{
//				if ((*Arr)[i] == (*Arr)[k]) {
//					break;
//				}
//			}
//			if (i == k)
//			{
//				if (k <= matrixSize && matrix[(*Arr)[matrixSize]][(*Arr)[1]] != 0)
//				{
//					return;
//				}
//			}
//			return;
//		}
//
//	} while (1);
//}
//
//void Hamiltonian(int k, int **matrix, int matrixSize, int **Arr) {
////	do
////	{
//		NextVertex(k, matrix, matrixSize, &*Arr);
////		printf("Hamilton\n");
//		if ((*Arr)[k] == 0)
//		{
//			return;
//		}
//		if (k == matrixSize)
//		{
//			for (int i = 0; i < matrixSize; i++)
//			{
//				printf("%d ", (*Arr)[i]);
//			}
//			printf("\n");
//		}
//		else
//		{
//			Hamiltonian(k + 1, matrix, matrixSize, &*Arr);
//		}
////	} while (1);
//}
//
//void exchange2(int* x, int* y)
//{
//	int temp;
//	temp = *x;
//	*x = *y;
//	*y = temp;
//}
//
//int k = 0;
//
//void permute(int* Arr, int start, int size, int ***Permutations)
//{
//	if (start == size) {
//		for (int i = 0; i <= size; i++)
//		{
//			(*Permutations)[k][i] = Arr[i];
//		}
//		k++;
//	}
//	else
//	{
//		for (int i = start; i <= size; i++)
//		{
//			exchange2((Arr + start), (Arr + i));
//			permute(Arr, start + 1, size, &*Permutations);
//			exchange2((Arr + start), (Arr + i));
//		}
//	}
//}
//
//int factorial(int k) {
//	int factor = 1;
//	for (int i = 1; i <= k; i++)
//	{
//		factor *= i;
//	}
//	return factor;
//}
//
//int main() {
//	int matrixSize = 4;
//	int** matrix = (int**)malloc(matrixSize * sizeof(int*));
//	for (int i = 0; i < (matrixSize); i++) {
//		matrix[i] = (int*)malloc(matrixSize * sizeof(int));
//	}
//
//	matrix[0][0] = 0;
//	matrix[0][1] = 15;
//	matrix[0][2] = 4;
//	matrix[0][3] = 9;
//
//	matrix[1][0] = 15;
//	matrix[1][1] = 0;
//	matrix[1][2] = 12;
//	matrix[1][3] = 21;
//
//	matrix[2][0] = 4;
//	matrix[2][1] = 12;
//	matrix[2][2] = 0;
//	matrix[2][3] = 10;
//
//	matrix[3][0] = 9;
//	matrix[3][1] = 21;
//	matrix[3][2] = 10;
//	matrix[3][3] = 0;
//
//	for (int i = 0; i < matrixSize; i++)
//	{
//		for (int j = 0; j < matrixSize; j++)
//		{
//			printf("%2d ", matrix[i][j]);
//		}
//		printf("\n");
//	}
//	printf("-------------\n");
//
//	int* Arr = malloc(matrixSize * sizeof(int));
//	for (int i = 0; i < matrixSize; i++)
//	{
//		Arr[i] = i;
//	}
//
////	Hamiltonian(1, matrix, matrixSize - 1, &Arr);
//
//	int factor = factorial(matrixSize);
//	int** Permutations = (int**)malloc(factor * sizeof(int*));
//	for (int i = 0; i < factor; i++) {
//		Permutations[i] = (int*)malloc(matrixSize * sizeof(int));
////		memset(matrix[i], 0, matrixSize * sizeof(int));
//	}
//
//	k = 0;
//	permute(Arr, 0, matrixSize - 1, &Permutations);
//
//	printf("-------------\n");
//	for (int i = 0; i < factor; i++)
//	{
//		for (int j = 0; j < matrixSize; j++)
//		{
//			printf("%d ", Permutations[i][j]);
//		}
//		printf("\n");
//	}
//
//	return 0;
//}