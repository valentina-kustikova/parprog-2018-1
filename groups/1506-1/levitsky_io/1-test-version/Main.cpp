#define _CRT_SECURE_NO_WARNINGS
#pragma once;
#include <cstdio>
#include <random>
#include <chrono>
#include "iostream"
#include <string>
#include <ctime>
#include <queue>
#include <omp.h>


using namespace std;

int procSize;
int procRank;

enum sortingType { INCREASE };

sortingType orderOfSorting;

union BinaryInt
{
	int d;
	unsigned char c[sizeof(double)];

	BinaryInt()
	{
		d = 0;
	}
	BinaryInt(int doub)
	{
		d = doub;
	}
};

void welcomeWords(int size)
{
	orderOfSorting = INCREASE;
}

void initData(BinaryInt *&dataArray, int size, int* mas)
{
	dataArray = new BinaryInt[size];
	srand(time(NULL));
	rand();
	for (int i = 0; i < size; i++)
	{
		dataArray[i] = BinaryInt(mas[i]);
	}
}


void outputMessage(double time1, double time2, bool isRight)
{
	cout << "\n--- RESULTS ---\n\nTime of non-parallel algorythm: " << time1 <<
		" ms\nTime of parallel algorythm:     " << time2 << " ms\n" <<
		"Speedup: " << time1 / time2 << "\n" <<
		"Results are the same: " << isRight << "\n";
}

void RadixSort(queue<BinaryInt> &data, queue<BinaryInt> &sortedData, int numOfByte, int numOfBitInByte)
{
	queue<BinaryInt> queueZero;
	queue<BinaryInt> queueOne;

	if (numOfByte != -1)
	{
		while (data.size() != 0)
		{
			if (!(numOfBitInByte & data.front().c[numOfByte]))
			{
				queueZero.push(data.front());
				data.pop();
			}
			else
			{
				queueOne.push(data.front());
				data.pop();
			}
		}
		// recursive call must be outside of while loop above
		numOfByte = numOfBitInByte == 1 ? --numOfByte : numOfByte;
		numOfBitInByte = numOfBitInByte == 1 ? numOfBitInByte = 128 : numOfBitInByte = numOfBitInByte / 2;
		int numOfByteCopy2 = numOfByte;
		int numOfBitInByteCopy2 = numOfBitInByte;

		if (queueZero.size() > 1)
			RadixSort(queueZero, sortedData, numOfByte, numOfBitInByte);
		while (queueZero.size() != 0)
		{
			sortedData.push(queueZero.front());
			queueZero.pop();
		}

		if (queueOne.size() > 1)
			RadixSort(queueOne, sortedData, numOfByteCopy2, numOfBitInByteCopy2);
		while (queueOne.size() != 0)
		{
			sortedData.push(queueOne.front());
			queueOne.pop();
		}
	}
}


void setResult(queue<BinaryInt> sortedData, BinaryInt *data)
{
	int count = sortedData.size();
	for (int i = 0; i < count; i++)
	{
		data[i] = sortedData.front();
		sortedData.pop();
	}
}


int main(int argc, char * argv[])
{
	int size;
	char* input = "ExArr.in";
	char* output = "SortedExArr.out";

	if (argc > 2)
	{
		input = argv[1];
		output = argv[2];
	}

	freopen(input, "rb", stdin);
	fread(&size, sizeof(size), 1, stdin);
	int* mas = new int[size];
	int* sorted = new int[size];
	fread(mas, sizeof(mas), size, stdin);
	fclose(stdin);


	BinaryInt *nonParallel = nullptr;
	BinaryInt *parallel = nullptr;
	BinaryInt *parallelCopy = nullptr;
	int *sendCounts = nullptr;
	int *displs = nullptr;
	double startTime = 0;
	double endTime = 0;
	double timeOfNonParallel = 0;
	double timeOfParallel = 0;
	welcomeWords(size);
	initData(nonParallel, size, mas);


	/* ---------- START OF NON PARALLEL ALGORITHM ------------------ */
	startTime = clock();

	queue<BinaryInt> queueData;
	queue<BinaryInt> sortedData;
	for (int i = 0; i < size; i++) {
		queueData.push(nonParallel[i]);
	}
	int u = 3; int o = 128;
	RadixSort(queueData, sortedData, u, o);
	setResult(sortedData, nonParallel);

	endTime = clock();
	timeOfNonParallel = endTime - startTime;

	for (int i = 0; i < size; i++)
	{
		sorted[i] = nonParallel[i].d;
	}
	freopen(output, "wb", stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(sorted, sizeof(*sorted), size, stdout);
	fclose(stdout);

	delete[] mas;
	delete[] sorted;
	delete[] nonParallel;
	delete[] parallel;
	delete[] parallelCopy;

	return 0;
}