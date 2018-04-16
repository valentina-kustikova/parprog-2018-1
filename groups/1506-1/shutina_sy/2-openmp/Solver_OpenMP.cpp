#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <omp.h>
#include <math.h>
#include <vector>
using namespace std;

void radixPass(int num_byte, long n, double *source, double *dest, int *count) {
	// временные переменные
	double *sp;
	int s, c, i, *cp;
	unsigned char *bp;

	// шаг 3: кол-во элементов <i
	s = 0; 	// временная переменная, хранящая сумму на данный момент
	cp = count;
	for (i = 256; i > 0; --i, ++cp) {
		c = *cp;
		*cp = s;
		s += c;
	}

	// шаг 4: окончательная растановка
	bp = (unsigned char *)source + num_byte;
	sp = source;
	for (i = n; i > 0; --i, bp += sizeof(double), ++sp) {
		cp = count + *bp;
		dest[*cp] = *sp;
		++(*cp);
	}
}

// Функция для последнего прохода при поразрядной сортировке чисел с плавающей точкой
void floatRadixLastPass(int num_byte, int n, double *source, double *dest, int *count) {
	double *sp;
	int s, c, i, *cp;
	unsigned char *bp;

	int numNeg = 0; // количество отрицательных чисел
	for (i = 128; i < 256; i++)
		numNeg += count[i];

	// первые 128 элементов count относятся к положительным числам.
	// отсчитываем номер первого числа, начиная от numNeg 
	s = numNeg;
	cp = count;
	for (i = 0; i < 128; ++i, ++cp) {
		c = *cp;
		*cp = s;
		s += c;
	}

	// изменения, касающиеся обратного расположения отрицательных чисел.
	s = count[255] = 0;                // отрицательные числа располагаются от начала массива
	cp = count + 254;
	for (i = 254; i >= 128; --i, --cp) {
		*cp += s;
		s = *cp;
	}

	bp = (unsigned char *)source + num_byte;
	sp = source;
	for (i = n; i > 0; --i, bp += sizeof(double), ++sp) {
		cp = count + *bp;
		if (*bp < 128)
			dest[(*cp)++] = *sp;
		else
			dest[--(*cp)] = *sp;
	}
}

// Создать счетчики.
// data-сортируемый массив, counters-массив для счетчиков, N-число элементов в data
void createCounters(double *data, int *counters, int n) {
	// i-й массив count расположен, начиная с адреса counters+256*i
	memset(counters, 0, 256 * sizeof(double) * sizeof(int));

	unsigned char *bp = (unsigned char*)data;
	unsigned char *dataEnd = (unsigned char*)(data + n);
	int i;

	while (bp != dataEnd) {
		// увеличиваем количество байт со значением *bp
		// i - текущий массив счетчиков
		for (i = 0; i < sizeof(double); i++)
			counters[256 * i + *bp++]++;
	}
}

// поразрядная сортировка чисел с плавающей точкой
void doubleRadixSort(double* in, int n) {
	double *out = new double[n];
	unsigned char i;

	int *counters = new int[sizeof(double) * 256], *count;
	createCounters(in, counters, n);

	for (i = 0; i < sizeof(double) - 1; i++) {
		count = counters + 256 * i; // count - массив счетчиков для i-го разряда

		if (count[0] == n) continue; //**

		radixPass(i, n, in, out, count);
		swap(in, out);
	}
	count = counters + 256 * i;
	floatRadixLastPass(i, n, in, out, count);

	delete in;
	in = out;
	delete counters;
}

int main(int argc, char* argv[]) {
	int nThreads;

	if (argc > 3) {
		nThreads = atoi(argv[1]);
		freopen(argv[2], "rb", stdin);
		freopen(argv[3], "wb", stdout);
	}
	else {
		if (argc > 1) {
			nThreads = atoi(argv[1]);
			freopen("array.in", "rb", stdin);
			freopen("array.out", "wb", stdout);
		}
		else return 1;
	}
	omp_set_num_threads(nThreads);
	int n;
	fread(&n, sizeof(n), 1, stdin);

	double* arr = new double[n];
	fread(arr, sizeof(*arr), n, stdin);

	double start_time = omp_get_wtime();

	int step;
	std::vector<double>* tempArray = new std::vector<double>[nThreads];
	int *shift = new int[nThreads], *chunk = new int[nThreads];

#pragma omp parallel shared(arr, step, shift, chunk, tempArray) 
	{
		int tid = 0, thread_index = 0;
		tid = omp_get_thread_num();//возвращает номер потока

		shift[tid] = tid*(n / nThreads);
		chunk[tid] = (tid == nThreads - 1) ? n - tid * (n / nThreads) : n / nThreads;
		doubleRadixSort(arr + shift[tid], chunk[tid]);
		printf("%d ", arr[0]);
#pragma omp barrier


	}

	delete[] tempArray;
	delete[] chunk;
	delete[] shift;

	double time = omp_get_wtime() - start_time;

	fwrite(&n, sizeof(n), 1, stdout);
	fwrite(arr, sizeof(*arr), n, stdout);
	fwrite(&time, sizeof(time), 1, stdout);

	delete[] arr;

	return 0;
}

