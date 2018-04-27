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