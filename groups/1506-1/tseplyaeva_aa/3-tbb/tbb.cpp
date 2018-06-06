#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/tbb.h"
#include <string>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdint.h>
#include <string>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include "omp.h"

using namespace std;
using namespace tbb;

void show(double* a, int st, int n){
	for (int i = st; i < n; i++){
		cout << a[i] << "  ";
	}
	cout << endl;
}

void shellsort(double*a, int st, int n){

	int size_b = n - st;
	double* b = new double[size_b];

	for (int i = 0; i < size_b; i++){
		b[i] = a[i + st];
	}

	vector<int> seq;
	int k = 0;
	int i = 0;
	int lvl = 0;

	while (k == 0){
		int p = 0;
		int q = 0;
		int powq = 1;
		for (p = lvl; p >= 0; p--){
			i = pow(2, p)*powq;
			powq *= 3;
			if (i <= (n / 2)){ seq.push_back(i); }
			else{
				k++;
				break;
			}
		}
		lvl++;
	}
	int m = seq.size();
	reverse(seq.begin(), seq.end());

	double x;
	int gap;
	int j;
	for (k = 0; k < m; k++) { // seq
		gap = seq[k];
		for (int i = gap; i < size_b; ++i) {
			x = b[i];
			for (j = i - gap; (x < b[j]) && (j >= 0); j = j - gap)
				b[j + gap] = b[j];
			b[j + gap] = x;

		}
	}

	for (int i = 0; i < size_b; i++){
		a[i + st] = b[i];
	}

}

//merging two parts from left to rigth
void simple_merge(double* a, int left, int rigth, int hm){
	vector<double> abc;

	for (int i = left; i < rigth; i++){
		abc.push_back(a[i]);
	}
	
	vector<double> one;
	vector<double> two;

	for (int i = 0; i <hm; i++){
		one.push_back(a[i + left]);
	}

	for (int i = hm; i < abc.size(); i++){
		two.push_back(a[i + left]);
	}

	abc.clear();

	unsigned left_it = 0, right_it = 0;

	while (left_it < one.size() && right_it < two.size()){
		if (one[left_it] < two[right_it]){
			abc.push_back(one[left_it]);
			left_it++;
		}
		else{
			abc.push_back(two[right_it]);
			right_it++;
		}
	}

	while (left_it < one.size()){
		abc.push_back(one[left_it]);
		left_it++;
	}

	while (right_it < two.size()){
		abc.push_back(two[right_it]);
		right_it++;
	}

	for (int i = 0; i < abc.size(); i++){
		a[i + left] = abc[i];
	}

	one.clear(); two.clear(); abc.clear();
}

void merge(double* a, int n, int parts, int thread_count1, int my_con){

	int glob_parts = parts / 2;
	int left, rigth, midl, midr;

	task_scheduler_init init(task_scheduler_init::deferred);
	init.initialize(thread_count1);
	
	parallel_for(tbb::blocked_range<int>(0, thread_count1),
		[&parts, &left, &rigth, &thread_count1, &n, &a, &glob_parts, &my_con](const blocked_range<int> &r){

		//sorting by parts
		for (int i = 0; i < thread_count1; ++i) {

			left = i *((n - n%parts) / glob_parts);

			if (i == (thread_count1 - 1)) {
				rigth = n;
			}
			else{
				rigth = left + ((n - n%parts) / glob_parts);
			}

			simple_merge(a, left, rigth, my_con);
		}

	});

}

void tbbShellSort(double* a, int n, int thread_count){
	//инициализация откладывается до яного вызова initialize (опр-ть кол-во потоков)
	task_scheduler_init init(task_scheduler_init::deferred);
	init.initialize(thread_count);

	int part = n / thread_count; // размер одной части
	int left, rigth;

	parallel_for(tbb::blocked_range<int>(0, thread_count),
		[&part, &left, &rigth, &thread_count, &n, &a](const blocked_range<int> &r){

		for (int i = 0; i < thread_count; i++){

			left = i * part;
			if (i == (thread_count - 1)) {
				rigth = n;
			}
			else{

				rigth = left + part;
			}

			shellsort(a, left, rigth);
			show(a, left, rigth);
		}

	});

	int parts;
	//amount of arrys parts to merge
	parts = thread_count;

	int m = (int)(round((double)log(thread_count) / log(2.0))); //thread level depth
	int my_con = n / parts;
	// go through levels
	for (int i = 0; i < m; i++){
		thread_count /= 2;

		merge(a, n, parts, thread_count, my_con);
		my_con *= 2;
		parts /= 2;
	}

}

int main(int argc, char* argv[]){

	int n;
	double* a;
	FILE *in;
	int thread_count = 0;
	// read from abinary file	
	if (argc > 3){
		in = fopen(argv[1], "rb"); // array.in
		if (!in) {
			return 0;
		}
		freopen(argv[2], "wb", stdout); // TBBsorted_array.in
		thread_count = atoi(argv[3]);
	}
	else{
		in = fopen("array.in", "rb"); // array.in
		if (!in) {
			return 0;
		}
		freopen("tbb.in", "wb", stdout); // tbbsorted_array.in
		thread_count = 2;
	}

	//read length of array
	fread(&n, sizeof(size_t), 1, in);
	//initializing 
	a = new double[n];
	// read array
	fread(a, sizeof(double), n, in);
	fclose(in);
	
	double start = omp_get_wtime();
	tbbShellSort(a, n, thread_count);
	double end = omp_get_wtime();
	double seconds = (end - start);

	// time at the end of file 
	// file for a sorted array
	freopen("tbb.in", "wb", stdout);
	fwrite(&n, sizeof(n), 1, stdout);
	fwrite(a, sizeof(*a), n, stdout);
	fwrite(&seconds, sizeof(seconds), 1, stdout);


	delete[] a;
	return 0;
}