#include <omp.h>
#include <string>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <stdint.h>



using namespace std;

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

	//show(b, 0, size_b);
	
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
		 a[i + st]=b[i];
	}
	
}

//merging two parts from left to rigth
void simple_merge(double* a, int left, int rigth, int part){
	vector<double> abc;

	for (int i = left; i < rigth; i++){
		abc.push_back(a[i]);
	}

	//	cout << abc.size()<<"  ";
	//cout << left << rigth<<"   ";
	//cout << "a   ";
	//show(a, left, rigth);
	vector<double> one;
	vector<double> two;

	//for (int i = left; i < rigth/2+left; i++){
	//	one.push_back(a[i]);
	//}

	for (int i = 0; i < part; i++){
		one.push_back(a[i + left]);
	}

	for (int i = part; i < abc.size(); i++){
		two.push_back(a[i + left]);
	}

	/*cout << "one" << endl;
	for (int i = 0; i < one.size(); i++){
	cout << one[i]<<"  ";
	}
	cout << endl;
	cout << endl;
	cout << "two" ;

	for (int i = 0; i < two.size(); i++){
	cout << two[i] << "  ";
	}
	cout << endl;
	cout << endl;
	cout << endl;*/

	abc.clear();
	unsigned left_it = 0, right_it = 0;

	while (left_it < one.size() && right_it < two.size())
	{
		if (one[left_it] < two[right_it])
		{
			abc.push_back(one[left_it]);
			left_it++;
		}
		else
		{
			abc.push_back(two[right_it]);
			right_it++;
		}
	}

	while (left_it < one.size())
	{
		abc.push_back(one[left_it]);
		left_it++;
	}

	while (right_it < two.size())
	{
		abc.push_back(two[right_it]);
		right_it++;
	}

	//	for (int i = 0; i < abc.size(); i++){
	//	cout << abc[i];
	//}

	for (int i = 0; i < abc.size(); i++){
		a[i + left] = abc[i];
	}

	//cout << "result is:" << endl;
	//show(a, left, rigth);
	one.clear(); two.clear(); abc.clear();
}


void merge(double* a, int n, int parts, int thread_count1){

	omp_set_num_threads(thread_count1);
	int left, rigth;


#pragma omp parallel for schedule(static) shared(a, parts ,n ) \
  private(left, rigth)

	//sorting by parts
	for (int i = 0; i < thread_count1; ++i) {

		left = i * parts * 2;
		if (i == (thread_count1 - 1)) {

			rigth = n;
		}
		else{

			rigth = left + parts * 2;
		}


		//cout << "From omp:   "<<i<<"   " ;
		//show(a, left, rigth);
		//cout << endl;
		simple_merge(a, left, rigth, parts);

	}


}






void OmpShellSort(double* a, int n, int thread_count) {

	omp_set_num_threads(thread_count);
	int parts = n / thread_count;
	int left, rigth;

#pragma omp parallel for schedule(static) shared(parts, n) \
  private(left, rigth)

	//sorting by parts
	for (int i = 0; i < thread_count; ++i) {

		left = i * parts;
		if (i == (thread_count - 1)) {
			rigth = n;
		}
		else{

			rigth = left + parts;
		}

		//	cout << left << rigth << endl;
		shellsort(a, left, rigth);
		//	show(a, left, rigth);
		//	cout << endl;

	}



	//amount of arrys parts to merge
	//parts = thread_count;

	int m = (int)(round((double)log(thread_count) / log(2.0))); //thread level depth

	// go through levels
	for (int i = 0; i < m; i++){

		thread_count /= 2;

		//	cout << "parts: " << parts;
		//	cout << "TC: " << thread_count << "    " << endl;
		merge(a, n, parts, thread_count);

		parts *= 2;
	};


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
		freopen(argv[2], "wb", stdout); // OMPsorted_array.in
		thread_count = atoi(argv[3]);
	}
	else{
		in = fopen("array.in", "rb"); // array.in
		if (!in) {
			return 0;
		}
		freopen("omp.in", "wb", stdout); // OMPsorted_array.in
		thread_count = 2;
	}

	
	//cout << "TC =" << thread_count;
	


	//read length of array
	fread(&n, sizeof(size_t), 1, in);
	//initializing 
	a = new double[n];
	// read array
	//fseek(in, 0, SEEK_SET);
	fread(a, sizeof(double), n, in);
	fclose(in);
	//////////////////////////////////////////////////////
	//	for (int i = 0; i < n; i++){
	//		a[i] = rand() % 50;
	//	}


	//show(a, 0, n);
	double start = omp_get_wtime();

	OmpShellSort(a, n, thread_count);

	double end = omp_get_wtime();
	//show(a, 0, n);
	double seconds = (end - start);

	// time at the end of file 
	// file for a sorted array
	freopen("omp.in", "wb", stdout);
	fwrite(&n, sizeof(n), 1, stdout);
	fwrite(a, sizeof(*a), n, stdout);
	fwrite(&seconds, sizeof(seconds), 1, stdout);
	

	delete[] a;
	return 0;
}