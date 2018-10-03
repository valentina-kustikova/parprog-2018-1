#define _CRT_SECURE_NO_WARNINGS

#include <omp.h>
#include <cstdio>
#include <math.h>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;

void QuickSort(double* arr, int size);
int Bin_Search_ind(const std::vector<double> arr, int left, int right, double key);//бинарный поиск максимального элемента слева относительно key
void ShareAndMerge(std::vector<double> vec1, std::vector<double> vec2, double* write_to);

int main(int argc, char * argv[])
{
	if (argc != 4) {
		puts("Use 'omp_qs [threads] [input file] [output file]'");
		return 1;
	}

	int size, threads = atoi(argv[1]);
	double* arr;
	double fict_time;

	freopen(argv[2], "rb", stdin);
	fread(&fict_time, sizeof(fict_time), 1, stdin);
	fread(&size, sizeof(size), 1, stdin);

	arr = new double[size];

	fread(arr, sizeof(*arr), size, stdin);
	fclose(stdin);

	double time = omp_get_wtime();
	int step;
	std::vector<double>* tempArray = new std::vector<double>[threads]; 
	int *shift = new int[threads], *chunk = new int[threads]; //сдвиг относительно начала, хвост относительно сдвига

#pragma omp parallel shared(arr, step, shift, chunk, tempArray) num_threads(threads) 
	{
		int tid, thread_index; //ид текущего потока 
		tid = omp_get_thread_num();

		shift[tid] = tid * (size / threads); //начало с которого будем сортировать

		chunk[tid] = (tid == threads - 1) ? size - tid * (size / threads) : size / threads;

		QuickSort(arr + shift[tid], chunk[tid]);

#pragma omp barrier //ждешь все потоки

		step = 1; //шаг рекурсии
		while (step < threads) {
			thread_index = tid + step; //позволяет найти следующиий поток

			for (int i = 0; i < chunk[tid]; ++i) { //записываем каждый кусок исходного массива в нужный вектор
				tempArray[tid].push_back(*(arr + shift[tid] + i));
			}

#pragma omp barrier //ждем все потоки

			if ((tid % (step * 2)) == 0) { //в каждой паре берем левый 
				ShareAndMerge(tempArray[tid], tempArray[thread_index], &arr[shift[tid]]);
				chunk[tid] += chunk[thread_index];
				chunk[thread_index] = 0;
				tempArray[tid].clear(); tempArray[tid].shrink_to_fit(); //удаление памяти
				tempArray[thread_index].clear(); tempArray[thread_index].shrink_to_fit();
			}
#pragma omp barrier
#pragma omp single 
			{
				step *= 2;
			}
#pragma omp barrier
		}
	}
	delete[] tempArray;
	delete[] chunk;
	delete[] shift;

	time = omp_get_wtime() - time;

	printf("OpenMP time: %f\n", time);

	if (FILE *outFile = fopen(argv[3], "wb"))
	{
		fwrite(&time, sizeof(time), 1, outFile);
		fwrite(&size, sizeof(size), 1, outFile);
		fwrite(arr, sizeof(*arr), size, outFile);
		fclose(outFile);
	}

	return 0;

}

