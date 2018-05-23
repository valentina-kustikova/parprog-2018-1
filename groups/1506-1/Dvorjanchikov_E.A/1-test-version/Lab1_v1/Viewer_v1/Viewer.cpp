#include <stdio.h>  
#include<cstdlib>

int main()
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, "output.txt", "r");
	fseek(fp, 12, SEEK_SET);
	int k;
	fscanf_s(fp, "%i", &k);
	int* mas = new int[k];
	for (int i = 0; i < k; i++)
	{
		fscanf_s(fp, "%i", &mas[i]);
	}
	fclose(fp);
	err = fopen_s(&fp, "input", "wb");
	fwrite(&k, sizeof(int), 1, fp);
	fwrite(mas, sizeof(int), k, fp);
	fclose(fp);
	delete(mas);
	return 0;
}

