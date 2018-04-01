#include <stdio.h>  
#include<cstdlib>
#include <algorithm>
#define eps 0.00001

double compare(const int *a, const int *b)
{
	return *a - *b;
}
void Show(double* mas, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%lf \n", mas[i]);
	}
	printf("------------------------------------------------------------ \n");
}
int main()
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, "input", "rb");
	int p;
	fread_s(&p, sizeof(int), sizeof(int), 1, fp);
	double * resmas = new double[p];
	fread_s(resmas, p * sizeof(double), sizeof(double), p, fp);
	fclose(fp);
	err = fopen_s(&fp, "output", "rb");
	int k;
	fread_s(&k, sizeof(int), sizeof(int), 1, fp);
	double * copymas = new double[k];
	fread_s(copymas, k * sizeof(double), sizeof(double), k, fp);
	fclose(fp);
	fopen_s(&fp, "status.txt", "w");
	if (k != p)
	{
		fprintf_s(fp, "Error 1 \n");
		return 1;
	}
	std::sort(resmas,resmas+p);
	bool flag = true;
	for (int i = 0; i < p; i++)
	{
		if (abs(copymas[i] - resmas[i])>eps)
			flag = false;
	}
	if (flag)
	{
		fprintf_s(fp, "Allright \n");
	}
	else
	{
		fprintf_s(fp, "Error 2 \n");
	}
	delete resmas;
	delete copymas;
	fclose(fp);
	return 2;
}