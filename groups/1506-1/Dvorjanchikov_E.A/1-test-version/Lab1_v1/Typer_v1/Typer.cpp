#include <stdio.h>  
#include<cstdlib>

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
	err = fopen_s(&fp, "output.txt", "w");
	fprintf_s(fp, "Size of mas %i \n", p);
	for (int i = 0; i < p; i++)
	{
		fprintf(fp, "%lf \n", resmas[i]);
	}
	fclose(fp);
	return 0;
}
