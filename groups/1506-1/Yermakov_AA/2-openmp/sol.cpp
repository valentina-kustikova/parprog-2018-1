#include <omp.h>
#include <iostream>
#include <cstdio>
#include <math.h>
#include <vector>
#include<iterator>
#include <algorithm>
using namespace std;

void QuickSort(double* arr, int size)
{
	int i = 0, j = size - 1;
	double pivot = arr[size / 2];

	do {
		while (arr[i] < pivot) i++;
		while (arr[j] > pivot) j--;

		if (i <= j) {
			double tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;

			i++;
			j--;
		}
	} while (i <= j);

	if (j > 0)
		QuickSort(arr, j + 1);
	if (i < size)
		QuickSort(&arr[i], size - i);
}

int Bin_Search_ind(const std::vector<double> arr, int left, int right, double key)
{
	int midd = 0;
	while (1)
	{
		midd = (left + right) / 2;

		if (key < arr[midd])
			right = midd - 1;
		else if (key > arr[midd])
			left = midd + 1;
		else
			return midd;

		if (left > right) {
			if (left - 1 >= 0)
				return left - 1;
			else
				return left;
		}
	}
}

vector<double> MergeAndSort(const std::vector<double> vec1, const std::vector<double> vec2) //функци€ сортировки и сли€ни€ двух векторов
{
	int i = 0, j = 0;
	int size1 = vec1.size(), size2 = vec2.size();
	vector<double> write_to;
	write_to.resize(size1 + size2);

	while (i < size1 && j < size2) {
		if (vec1[i] <= vec2[j]) {
			write_to[i + j] = vec1[i];
			i++;
		}
		else {
			write_to[i + j] = vec2[j];
			j++;
		}
	}

	while (i < size1) {
		write_to[size2 + i] = vec1[i];
		i++;
	}
	while (j < size2) {
		write_to[size1 + j] = vec2[j];
		j++;
	}
	return write_to;
}

void ShareAndMerge(std::vector<double> vec1, std::vector<double> vec2, double* write_to)
{

	int left1 = 0, left2 = 0;
	int last1 = vec1.size() - 1, last2 = vec2.size() - 1;
	int mid1 = (left1 + last1) / 2;//индекс среднего элемента первого вектора
	double key = vec1[mid1];//средний элемент первого вектора, относительно него выполн€ем бинарный посик во втором векторе
	int mid2 = Bin_Search_ind(vec2, left2, last2, key);

	vector<double> vec_temp1_1, vec_temp1_2;
	vec_temp1_1.insert(vec_temp1_1.begin(), vec1.begin(), vec1.begin() + mid1 + 1);
	vec_temp1_2.insert(vec_temp1_2.begin(), vec2.begin(), vec2.begin() + mid2 + 1);

	vector<double> vec_temp2_1, vec_temp2_2;
	vec_temp2_1.insert(vec_temp2_1.begin(), vec1.begin() + mid1 + 1, vec1.begin() + last1 + 1);
	vec_temp2_2.insert(vec_temp2_2.begin(), vec2.begin() + mid2 + 1, vec2.begin() + last2 + 1);

	vec1 = MergeAndSort(vec_temp1_1, vec_temp1_2);
	vec2 = MergeAndSort(vec_temp2_1, vec_temp2_2);
	vector<double> Merged;
	Merged.resize(vec1.size() + vec2.size());
	merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), Merged.begin());
	for (int i = 0; i < Merged.size(); i++)
		write_to[i] = Merged[i];
}

void printArray(double* arr, int size) {
	for (int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
	cout << "|" << endl;
}
