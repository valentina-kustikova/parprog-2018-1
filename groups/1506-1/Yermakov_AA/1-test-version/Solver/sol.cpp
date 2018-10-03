void quicksort(double *array, int size)
{
	int l = 0;
	int r = size-1;
	double temp = array[size / 2];

	do
	{
		while (array[l] < temp)
			l++;
		while (array[r] > temp)
			r--;

		if (l <= r)
		{
			double tmp = array[l];
			array[l] = array[r];
			array[r] = tmp;
			l++;
			r--;
		}
	} while (l <= r);

	if (r > 0)
		quicksort(array, r + 1);
	if (l < size)
		quicksort(&array[l], size - l);
}

