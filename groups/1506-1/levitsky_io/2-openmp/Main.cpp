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


void initData(BinaryInt *&dataArray, int size, int* mas)
{
	dataArray = new BinaryInt[size];
	for (int i = 0; i < size; i++)
	{
		dataArray[i] = BinaryInt(mas[i]);
	}
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
		//cout << data[i].d << " ";
		sortedData.pop();
	}
}

BinaryInt* merge(BinaryInt *&firstArray, BinaryInt *&secondArray, int sizeFirst, int sizeSecond)
{
	BinaryInt *sortedArray = new BinaryInt[sizeFirst + sizeSecond];
		int indexFirst = 0;
		int indexSecond = 0;
		int index = 0;
		while ((indexFirst < sizeFirst) && (indexSecond < sizeSecond))
		{
			BinaryInt elementFirst = firstArray[indexFirst];
			BinaryInt elementSecond = secondArray[indexSecond];
			if (elementFirst.d < elementSecond.d)
			{
				sortedArray[index] = elementFirst;
				indexFirst++;
			}
			else
			{
				sortedArray[index] = elementSecond;
				indexSecond++;
			}
			index++;
		}
		while (indexFirst < sizeFirst)
		{
			sortedArray[index] = (firstArray)[indexFirst];
			indexFirst++;
			index++;
		}
		while (indexSecond < sizeSecond)
		{
			sortedArray[index] = secondArray[indexSecond];
			indexSecond++;
			index++;
		}
		return sortedArray;
}

int main(int argc, char * argv[])
{
	int size;
	char* input = "ExArr.in";
	char* output = "SortedExArr.out";
	int thread = 4;
	omp_set_num_threads(thread);

	if (argc > 2)
	{
		input = argv[1];
		output = argv[2];
	}

	freopen(input, "rb", stdin);
	fread(&size, sizeof(int), 1, stdin);
	int* mas = new int[size];
	int* sorted = new int[size];
	fread(mas, sizeof(int), size, stdin);
	fclose(stdin);

	int piece = size / 4;

	//BinaryInt *nonParallel = nullptr;
	BinaryInt *parallel = new BinaryInt[size];
	BinaryInt *parallelCopy = nullptr;
	int *sendCounts = nullptr;
	int *displs = nullptr;
	double startTime = 0;
	double endTime = 0;
	double timeOfNonParallel = 0;
	double timeOfParallel = 0;
	int i;
	int j = 0;

	queue<BinaryInt> queueData;
	queue<BinaryInt> sortedData;

	startTime = clock();

#pragma omp parallel private (i, queueData, sortedData)
	{
		//parallel = new BinaryInt[size];
		int num = omp_get_thread_num();
		cout << "Helo epta from " << num << endl;

		//initData(parallel, size, mas);
#pragma omp for schedule(dynamic, piece)
		for (i = 0; i <size; i++)
		{
			parallel[i] = BinaryInt(mas[i]);
		}

		/////////////////////////////////
		//startTime = clock();
#pragma omp for schedule(dynamic, piece)
		for (i = 0; i < size; i++) {
			queueData.push(parallel[i]);
		}
		int u = 3; int o = 128;

		RadixSort(queueData, sortedData, u, o);
		setResult(sortedData, parallel);

#pragma omp barrier
		//parallelCopy = merge(parallelCopy, parallel, piece, size);

#pragma omp for schedule(dynamic, piece)
		for (i = 0; i < size; i++)
		{
			sorted[i] = parallel[i].d;
		}
		
	}
	endTime = clock();
	timeOfNonParallel = endTime - startTime;
	cout << "---" << timeOfNonParallel << "---" << endl;


	freopen(output, "wb", stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(sorted, sizeof(*sorted), size, stdout);
	fclose(stdout);

	delete[] mas;
	delete[] sorted;
	//delete[] nonParallel;
	delete[] parallel;
	delete[] parallelCopy;

	return 0;
}