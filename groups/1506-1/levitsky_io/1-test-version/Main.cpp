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

void copyData(BinaryInt *src, BinaryInt **dest, int size)
{
	*dest = new BinaryInt[size];
	for (int i = 0; i < size; i++)
		(*dest)[i] = BinaryInt((src)[i].d);
}


void outputMessage(double time1, double time2, bool isRight)
{
	cout << "\n--- RESULTS ---\n\nTime of non-parallel algorythm: " << time1 <<
		" ms\nTime of parallel algorythm:     " << time2 << " ms\n" <<
		"Speedup: " << time1 / time2 << "\n" <<
		"Results are the same: " << isRight << "\n";
}

void RadixSort(queue<BinaryInt> data, queue<BinaryInt> sortedData, int numOfByte, int numOfBitInByte)
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

//BinaryInt* merge(BinaryInt **firstArray, BinaryInt **secondArray, int *sizeFirst, int *sizeSecond)
//{
//	BinaryInt *sortedArray = new BinaryInt[*sizeFirst + *sizeSecond];
//	int indexFirst = 0;
//	int indexSecond = 0;
//	int index = 0;
//	while ((indexFirst < *sizeFirst) && (indexSecond < *sizeSecond))
//	{
//		BinaryInt elementFirst = (*firstArray)[indexFirst];
//		BinaryInt elementSecond = (*secondArray)[indexSecond];
//		if (elementFirst.d < elementSecond.d)
//		{
//			sortedArray[index] = elementFirst;
//			indexFirst++;
//		}
//		else
//		{
//			sortedArray[index] = elementSecond;
//			indexSecond++;
//		}
//		index++;
//	}
//
//	while (indexFirst < *sizeFirst)
//	{
//		sortedArray[index] = (*firstArray)[indexFirst];
//		indexFirst++;
//		index++;
//	}
//
//	while (indexSecond < *sizeSecond)
//	{
//		sortedArray[index] = (*secondArray)[indexSecond];
//		indexSecond++;
//		index++;
//	}
//	return sortedArray;
//}

void setResult(queue<BinaryInt> sortedData, BinaryInt *&data)
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
	//cout.precision(15);
	int size;
	char* input;
	char* output;

	if (argc > 2)
	{
		//int num_threads = 1;
		input = argv[1];
		output = argv[2];
	}
	else
	{
		cout << "Enter file for sorting and outupt file" << endl;
		return 0;
	}

	/*if (argc > 2)
	{
		num_threads = atoi(argv[1]);
		char* input = argv[2];
		char* output = argv[3];
	}*/
	freopen(("tests\\", input), "rb", stdin);
	//freopen("Sort.out", "wb", stdout);

	//omp_set_num_threads(num_threads);

	fread(&size, sizeof(size), 1, stdin);
	int* mas = new int[size];
	int* sorted = new int[size];
	fread(mas, sizeof(mas), size, stdin);
	fclose(stdin);

	//cout << endl << endl;
	//cout << "mas:   ";
	//for (int i = 0; i < size; i++)
	//	cout << mas[i] << " ";
	//cout << endl << endl;

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
	//cout << "[TRACE] Data was initialized \n";
	copyData(nonParallel, &parallel, size);
	//cout << "[TRACE] Data was copied \n";
	//cout << "[INFO ] Size equals: " << size << "\n";

	//printArray(&nonParallel, &size);

	/* ---------- START OF NON PARALLEL ALGORITHM ------------------ */
	startTime = clock();
	//cout << "[TRACE] Start time of non-parallel algorythm: " << startTime << "\n";

	queue<BinaryInt> queueData;
	queue<BinaryInt> sortedData;
	for (int i = 0; i < size; i++) {
		queueData.push(nonParallel[i]);
	}
	int u = 3; int o = 128;
	RadixSort(queueData, sortedData, u, o);
	//cout << "[TRACE] Array was sorted by non-parallel algorythm \n";
	setResult(sortedData, nonParallel);

	endTime = clock();
	//cout << "[TRACE] End time of non-parallel algorythm: " << endTime << "\n";
	timeOfNonParallel = endTime - startTime;
	//cout << "[TRACE] Total time of non-parallel algorythm: " << timeOfNonParallel << " ms\n";

	//printArray(nonParallel, &size);
	for (int i = 0; i < size; i++)
	{
		sorted[i] = nonParallel[i].d;
		//cout << sorted[i] << " ";
	}
	freopen(("tests\\", output), "wb", stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	//fwrite(&timeOfNonParallel, sizeof(timeOfNonParallel), 1, stdout);
	fwrite(sorted, sizeof(*sorted), size, stdout);
	fclose(stdout);

	//delete[] input;
	//delete[] output;
	delete[] mas;
	delete[] sorted;
	delete[] nonParallel;
	delete[] parallel;
	delete[] parallelCopy;

	return 0;
}