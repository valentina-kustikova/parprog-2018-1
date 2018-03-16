#include <stdio.h>  
#include<cstdlib>

int main()
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, "input.bin", "rb");
	int p;
	fread_s(&p, sizeof(int), sizeof(int), 1, fp);
	int * resmas = new int[p];
	fread_s(resmas, p * sizeof(int), sizeof(int), p, fp);
	fclose(fp);
	err = fopen_s(&fp, "output.txt", "w");
	fprintf_s(fp, "Size of mas %i \n", p);
	for (int i = 0; i < p; i++)
	{
		fprintf(fp, "%i \n", resmas[i]);
	}
	err = fopen_s(&fp, "output.bin", "rb");
	fread_s(&p, sizeof(int), sizeof(int), 1, fp);
	fread_s(resmas, p * sizeof(int), sizeof(int), p, fp);
	fclose(fp);
	err = fopen_s(&fp, "output2.txt", "w");
	fprintf_s(fp, "Size of mas %i \n", p);
	for (int i = 0; i < p; i++)
	{
		fprintf(fp, "%i \n", resmas[i]);
	}
	return 0;
}
