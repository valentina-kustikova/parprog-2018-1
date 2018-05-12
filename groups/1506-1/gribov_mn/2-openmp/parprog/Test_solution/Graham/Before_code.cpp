
#define _CRT_SECURE_NO_WARNINGS
#include "Sol.h"
#include <cstdio>
//#include <time.h> 
#include <chrono>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	int num_threads = 1; 
	if (argc > 1)
	{
		num_threads = atoi(argv[1]);
		
	}
	omp_set_num_threads(num_threads);

	int amountOfPoints;
	freopen("matr.in", "rb", stdin); 
	freopen("matr.out", "wb", stdout);
	fread(&amountOfPoints, sizeof(amountOfPoints), 1, stdin);
	int linCountConv = 0;

	CPoint *mainArrOfPoints = new CPoint[amountOfPoints/num_threads];
	CPoint *resultArr = new CPoint[amountOfPoints];
	int *A = new int[2 * amountOfPoints];
	fread(A, sizeof(*A), 2 * amountOfPoints, stdin);
	int tid, i, resultCount = 0, sumCount = 0;
	/*for (int j=0; j<amountOfPoints; j++)
	{
		mainArrOfPoints[j].x = A[2*j];
		mainArrOfPoints[j].y = A[2*j + 1];
	}*/
	double time = 0.0;
	#pragma omp parallel private(tid,i,linCountConv,mainArrOfPoints) shared(A,resultArr,sumCount,resultCount,time,num_threads) 
	{
		tid = omp_get_thread_num();
		
		if (tid != 0)
		{
			i = 0;
			for (int j = amountOfPoints / num_threads * (tid - 1); j < amountOfPoints / num_threads * tid; j++)
			{
				mainArrOfPoints[i].x = A[2 * j];
				mainArrOfPoints[i].y = A[2 * j + 1];
				i++;
			}
			Conv(mainArrOfPoints, amountOfPoints/num_threads, linCountConv);
			int count2 = sumCount + linCountConv;
			int k = 0;
			for (int i = sumCount; i < count2; i++)
			{
				resultArr[i] = mainArrOfPoints[k];
				k++;
			}
			sumCount = count2;
		}
		else if (tid == 0)
		{
			mainArrOfPoints = new CPoint[amountOfPoints - amountOfPoints / num_threads*(num_threads-1)];
			i = 0;
			for (int j = amountOfPoints / num_threads * (num_threads - 1); j < amountOfPoints; j++)
			{
				mainArrOfPoints[i].x = A[2 * j];
				mainArrOfPoints[i].y = A[2 * j + 1];
				i++;
			}
			Conv(mainArrOfPoints, amountOfPoints - amountOfPoints / num_threads * (num_threads - 1), linCountConv);
			int count2 = sumCount + linCountConv;
			int k = 0;
			for (int i = sumCount; i < count2; i++)
			{
				resultArr[i] = mainArrOfPoints[k];
				k++;
			}
			sumCount = count2;						
		}
	}
	time = omp_get_wtime();
	Conv(resultArr, sumCount, resultCount);
	time = omp_get_wtime() - time;
	
	/*for (int i = 0; i < amountOfPoints-2; i=i+2)
	{
		mainArrOfPoints[i].x = A[i];
		mainArrOfPoints[i].y = A[i+1];
	}*/

	/*double time = omp_get_wtime();
	
	#pragma omp parallel
	{
		Conv(mainArrOfPoints, amountOfPoints, linCountConv);
	}
	time = omp_get_wtime() - time;*/
	
	//fwrite(&time, sizeof(time), 1, stdout);

	int *B = new int[2 * resultCount];
	for (int j = 0; j<resultCount; j++)
	{
		B[2*j] = resultArr[j].x;
		B[2*j + 1] = resultArr[j].y;
	}
	
	fwrite(&resultCount, sizeof(resultCount), 1, stdout);
	fwrite(B, sizeof(*B), 2 * resultCount, stdout);
	fwrite(&time, sizeof(time), 1, stdout);
	delete A,B, mainArrOfPoints, resultArr;
	fclose(stdin);
	fclose(stdout);
	
}