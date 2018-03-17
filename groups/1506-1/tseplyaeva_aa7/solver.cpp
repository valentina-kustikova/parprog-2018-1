#include "conio.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <stdint.h>

#define ERROR_OPEN_FILE -3


#include <string>
using namespace std;



void show(int* a, int n){
	for (int i = 0; i < n; i++){
		cout << a[i] << "  ";
	}
	cout << endl;
}

void shellsort(int*a, int n){
	vector<int> seq;
	int k = 0;
	int i = 0;
	int lvl = 0;

	while (k == 0){
		int p = 0;
		int q = 0;
		for (p = lvl; p >= 0; p--){
			i = pow(2, p)*pow(3, q);
			q++;
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

	int x;
	int gap;
	int j;
	for (k = 0; k < m; k++) { // seq
		gap = seq[k];
		for (int i = gap; i < n; ++i) {
			x = a[i];
			for (j = i - gap; (x < a[j]) && (j >= 0); j = j - gap)
				a[j + gap] = a[j];
			a[j + gap] = x;
			//show(a,n);
		}
	}

}


int main(){

	int n;
	int* a;
	// read from abinary file	 
	FILE *in = fopen("array.in", "rb");
	if (!in) {
		return 0;
	}
	//read length of array
	fread(&n, sizeof(size_t), 1, in);
	//initializing 
	a = new int[n];
	// read array
	//fseek(in, 0, SEEK_SET);
	fread(a, sizeof(int32_t), n, in);
	fclose(in);
	////////////////////////////////////////////////////////
	//	for (int i = 0; i < n; i++){
	//		a[i] = rand() % 50;
	//	}


	//show(a, n);

	shellsort(a, n);
	//show(a, n);

	// file for a sorted array
	freopen("sorted_array.in", "wb", stdout);
	fwrite(&n, sizeof(n), 1, stdout);
	fwrite(a, sizeof(*a), n, stdout);



	return 0;
}