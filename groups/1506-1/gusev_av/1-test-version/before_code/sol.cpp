void QuickSort(int* arr, int size)
{
	int left = 0;
	int right = size - 1;
	int middle = arr[size / 2];
	do {
		while (arr[left] < middle)
			left++;
		while (arr[right] > middle)
			right--;

		if (left <= right) {
			int tmp = arr[left];
			arr[left] = arr[right];
			arr[right] = tmp;
			left++;
			right--;
		}
	} while (left <= right);
	if (right > 0)
		QuickSort(arr, right + 1);
	if (left < size)
		QuickSort(&arr[left], size - left);
}