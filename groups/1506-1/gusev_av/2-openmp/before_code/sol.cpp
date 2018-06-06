void QuickSort(double* arr, int size)
{
	int left = 0;
	int right = size - 1;
	double middle = arr[size / 2];
	do {
		while (arr[left] < middle)
			left++;
		while (arr[right] > middle)
			right--;

		if (left <= right) {
			double tmp = arr[left];
			arr[left] = arr[right];
			arr[right] = tmp;
			left++;
			right--;
		}
	} while (left <= right);
#pragma omp parallel
	{
#pragma omp sections nowait
		{
#pragma omp section
			if (right > 0)
				QuickSort(arr, right + 1);
#pragma omp section
			if (left < size)
				QuickSort(&arr[left], size - left);
		}
	}
}
void Merge(double *arr1, double *arr2, int arr1_size, int arr2_size, double* res)
{
	int i = 0, j = 0; 
	while (i < arr1_size && j < arr2_size)
	{
		if (arr1[i] <= arr2[j])
		{
			res[i + j] = arr1[i];
			i++;
		}
		else
		{
			res[i + j] = arr2[j];
			j++;
		}
	}
	while (i < arr1_size)
	{
		res[i + arr2_size] = arr1[i];
		i++;
	}
	while (j < arr2_size)
	{
		res[j + arr1_size] = arr2[j];
		j++;
	}
}