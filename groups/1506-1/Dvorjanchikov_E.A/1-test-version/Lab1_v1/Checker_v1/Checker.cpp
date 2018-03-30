#include <stdio.h>  
#include<cstdlib>

double compare(const double *a, const double *b)
{
	return *a - *b;
}
int main()
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, "input", "rb");
	int p;
	fread_s(&p, sizeof(int), sizeof(int), 1, fp);
	int * resmas = new int[p];
	fread_s(resmas, p * sizeof(double), sizeof(double), p, fp);
	fclose(fp);
	err = fopen_s(&fp, "output", "rb");
	int k;
	fread_s(&k, sizeof(int), sizeof(int), 1, fp);
	double * copymas = new double[k];
	fread_s(copymas, k * sizeof(double), sizeof(double), k, fp);
	fclose(fp);
	fopen_s(&fp, "status.txt", "a");
	if (k != p)
	{
		fprintf_s(fp, "Error 1 \n");
		return 1;
	}
	std::qsort(resmas, p, sizeof(double), (int(*) (const void *, const void *))compare);
	bool flag = true;
	for (int i = 0; i < p; i++)
	{
		if (copymas[i] != resmas[i])
			flag = false;
	}
	if (flag)
	{
		fprintf_s(fp, "Allright \n");
		return 0;
	}
	fprintf_s(fp, "Error 2 \n");
	delete resmas;
	delete copymas;
	fclose(fp);
	return 2;
}