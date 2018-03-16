#include <stdio.h>  
#include<cstdlib>
#include <ctime>
int compareup(const int *a, const int *b)
{
	return *a - *b;
}
int comparedown(const int *a, const int *b)
{
	return *b - *a;
}
int main(int argc, char* argv[])
{
	FILE* fp;
	errno_t err;
	srand(time(0));
	err = fopen_s(&fp, "input.bin", "wb");
	int k = atoi(argv[1]);
	fwrite(&k, sizeof(int), 1, fp);
	int * mas = new int[k];
	for (int i = 0; i < k; i++)
	{
		mas[i] = rand() % 200;
	}
	if (atoi(argv[2]) == 2)
		std::qsort(mas, k, sizeof(int), (int(*) (const void *, const void *))compareup);
	if (atoi(argv[2]) == 3)
		std::qsort(mas, k, sizeof(int), (int(*) (const void *, const void *))comparedown);
	fwrite(mas, sizeof(int), k, fp);
	delete mas;
	fclose(fp);
	return 0;
}