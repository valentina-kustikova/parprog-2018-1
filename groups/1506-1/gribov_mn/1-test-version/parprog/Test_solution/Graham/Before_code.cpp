#include "Sol.h"
#include <chrono>

int main(int argc, char * argv[])
{
	int num_threads = 1; 
	if (argc > 1) 
		num_threads = atoi(argv[1]);
	
	CPoint *mainArrOfPoints = 0;

	int amountOfPoints;
	freopen("matr.in", "rb", stdin); 
	freopen("matr.out", "wb", stdout);
	fread(&amountOfPoints, sizeof(amountOfPoints), 1, stdin);
	int linCountConv = 0;

	mainArrOfPoints = new CPoint[amountOfPoints];
	int *A = new int[2 * amountOfPoints];
	fread(A, sizeof(*A), 2 * amountOfPoints, stdin);
	for (int i = 0; i < amountOfPoints-2; i=i+2)
	{
		mainArrOfPoints[i].x = A[i];
		mainArrOfPoints[i].y = A[i+1];
	}

	chrono::steady_clock::time_point start = chrono::steady_clock::steady_clock::now();
	Conv(mainArrOfPoints, amountOfPoints, linCountConv);
	chrono::steady_clock::time_point end = chrono::steady_clock::steady_clock::now();
	int time = chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	fwrite(&time, sizeof(time), 1, stdout);
	int *B = new int[2 * linCountConv];
	for (int i = 0; i < linCountConv-2; i=i+2)
	{
		B[i] = mainArrOfPoints[i].x;
		B[i + 1] = mainArrOfPoints[i].y;
	}
	fwrite(B, sizeof(*B), 2 * linCountConv, stdout);
	
}