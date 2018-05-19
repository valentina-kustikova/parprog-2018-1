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
	queue<BinaryInt> MqueueZero;
	queue<BinaryInt> MqueueOne;

			if (numOfByte != -1)
			{
				while (data.size() != 0)
				{
					if (data.front().d < 0)
					{
						if (!(numOfBitInByte & data.front().c[numOfByte]))
						{
							MqueueOne.push(data.front());
							data.pop();
						}
						else
						{
							MqueueZero.push(data.front());
							data.pop();
						}
					}
					else
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
				}
			}
			// recursive call must be outside of while loop above
			numOfByte = numOfBitInByte == 1 ? --numOfByte : numOfByte;
			numOfBitInByte = numOfBitInByte == 1 ? numOfBitInByte = 128 : numOfBitInByte = numOfBitInByte / 2;
			int numOfByteCopy2 = numOfByte;
			int numOfBitInByteCopy2 = numOfBitInByte;
			if (MqueueOne.size() > 1)
				RadixSort(MqueueOne, sortedData, numOfByteCopy2, numOfBitInByteCopy2);
			//#pragma omp section
			while (MqueueOne.size() != 0)
			{
				sortedData.push(MqueueOne.front());
				MqueueOne.pop();
			}

			if (MqueueZero.size() > 1)
				RadixSort(MqueueZero, sortedData, numOfByte, numOfBitInByte);

			//#pragma omp section

			while (MqueueZero.size() != 0)
			{
				sortedData.push(MqueueZero.front());
				MqueueZero.pop();
			}
			//////////////

			if (queueZero.size() > 1)
				RadixSort(queueZero, sortedData, numOfByte, numOfBitInByte);
			//#pragma omp section

			while (queueZero.size() != 0)
			{
				sortedData.push(queueZero.front());
				queueZero.pop();
			}

			if (queueOne.size() > 1)
				RadixSort(queueOne, sortedData, numOfByteCopy2, numOfBitInByteCopy2);
			//#pragma omp section

			while (queueOne.size() != 0)
			{
				sortedData.push(queueOne.front());
				queueOne.pop();
			}
}


void setResult(queue<BinaryInt> sortedData, BinaryInt *data)
{
	//int count = sortedData.size() * omp_get_thread_num();
	int left = sortedData.size()* omp_get_thread_num();
	int right = sortedData.size()* (omp_get_thread_num()+1);
	for (left; left < right; left++)
	{
		data[left] = sortedData.front();
		sortedData.pop();
	}
}

void merge(int* &Sorted, BinaryInt *&Array, int size)
{
		int left = size * omp_get_thread_num();
		int right = size * (omp_get_thread_num() + 1);
		int leftsize = left +size;
		int rightsize = right + size;
		int index = size * omp_get_thread_num();


		while ((left < leftsize) && (right<rightsize))
		{
			int elemfirst = Array[left].d;
			int elemsecond = Array[right].d;

			if (Array[left].d < Array[right].d)
			{
				Sorted[index] = elemfirst;
				left++;
				index++;
			}
			else
			{
				Sorted[index] = elemsecond;
				right++;
				index++;
			}
		}

		while (left < leftsize)
		{
			Sorted[index] = Array[left].d;
			left++;
			index++;
		}
		while (right < rightsize)
		{
			Sorted[index] = Array[right].d;
			right++;
			index++;
		}
		for (int i = size * omp_get_thread_num(); i < size * (omp_get_thread_num() + 1) + size; i++)
			Array[i].d = Sorted[i];
}

void sorting(queue<BinaryInt> queueData, queue<BinaryInt> sortedData, BinaryInt *parallel, int* mas, int size, int piece)
{
#pragma omp parallel private (queueData, sortedData)
	{
#pragma omp for
		for (int i = 0; i < size; i++) {
			queueData.push(parallel[i]);
		}

		int u = 3; int o = 128;
		RadixSort(queueData, sortedData, u, o);

		setResult(sortedData, parallel);

		int sendsize = piece;
		int counter = 2;

#pragma omp barrier
		while (sendsize < size)
		{
			if (omp_get_thread_num() % counter == 0)
			{
				merge(mas, parallel, sendsize);
			}
			sendsize *= 2;
			counter *= 2;
#pragma omp barrier
		}
	}
}

int main(int argc, char * argv[])
{
	int size;
	char* input = "ExArr.in";
	char* output = "SortedExArr.out";
	int thread = 4;

	if (argc > 3)
	{
		thread = atoi(argv[1]);
		input = argv[2];
		output = argv[3];
	}

	omp_set_num_threads(thread);

	freopen(input, "rb", stdin);
	fread(&size, sizeof(int), 1, stdin);
	int* mas = new int[size];
	fread(mas, sizeof(int), size, stdin);
	fclose(stdin);

	BinaryInt *parallel = new BinaryInt[size];
	double startTime = 0;
	double endTime = 0;
	double timeOfNonParallel = 0;

	queue<BinaryInt> queueData;
	queue<BinaryInt> sortedData;

	startTime = clock();

	initData(parallel, size, mas);

	int piece = size / thread;

	startTime = clock();
	sorting(queueData, sortedData, parallel, mas, size, piece);
	endTime = clock();

	timeOfNonParallel = endTime - startTime;
	cout << "---" << timeOfNonParallel << "---" << endl;
	
	freopen(output, "wb", stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(mas, sizeof(*mas), size, stdout);
	fclose(stdout);

	delete[] mas;
	delete[] parallel;

	return 0;
}