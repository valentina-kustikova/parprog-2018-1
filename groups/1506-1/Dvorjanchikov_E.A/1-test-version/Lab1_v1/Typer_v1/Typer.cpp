#include <stdio.h>  
#include<cstdlib>

int main(int argc, char* argv[])
{
	FILE* fp;
	errno_t err;
	if (argc >= 2)
	{
		err = fopen_s(&fp, argv[1], "rb");
	}
	else
	{
		err = fopen_s(&fp, "input", "rb");
	}
	int p;
	fread_s(&p, sizeof(int), sizeof(int), 1, fp);
	double * resmas = new double[p];
	fread_s(resmas, p * sizeof(double), sizeof(double), p, fp);
	fclose(fp);
	if (argc == 3)
	{
		err = fopen_s(&fp, argv[2], "w");
	}
	else
	{
		err = fopen_s(&fp, "output.txt", "w");
	}
	fprintf_s(fp, "Size of mas %i \n", p);
	for (int i = 0; i < p; i++)
	{
		fprintf(fp, "%lf \n", resmas[i]);
	}
	fclose(fp);
	delete(resmas);
	return 0;
}
