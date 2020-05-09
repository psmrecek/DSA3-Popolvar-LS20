//#include <stdio.h>
//#include <time.h>
//
//int main() {
//
//	int n = 15;
//	int m = 15;
//	int t = 100;
//
//	printf("%d %d %d\n", n, m, t);
//
//	srand(time(0));
//	int lower = 0, upper = 3;
//	int num;
//
//	for (int i = 0; i < n; i++)
//	{
//		for (int j = 0; j < m; j++)
//		{
//			if (i == 0 && j == 0)
//			{
//				printf("C");
//				continue;
//			}
//			num = (rand() % (upper - lower + 1)) + lower;
//			if (num == upper - 1) printf("H");
//			else if (num == upper) printf("N");
//			else printf("C");
//		}
//		printf("\n");
//	}
//	return 0;
//}