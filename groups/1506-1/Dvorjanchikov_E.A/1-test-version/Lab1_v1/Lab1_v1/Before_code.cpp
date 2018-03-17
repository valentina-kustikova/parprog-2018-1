#include <iostream>
#include <ctime>
int arr[9] = { 1,4,10,23,57,132,301,701,1750 };
int count_divene = 1;
int mas_size;
int shift;
int** mas_pointer;
int* shifts;
int* sizes_mas;
int rank;
int pointer_k;
int Rank(int tmp)
{
	int res = 0;
	while (tmp != 1)
	{
		tmp = tmp / 2;
		res++;
	}
	return res;
}
void Show(int* mas, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%i \n", mas[i]);
	}
	printf("------------------------------------------------------------ \n");
}
void Dive(int* mas, int shift, int k, int step)
{
	if (step > 0)
	{
		if (mas[k*(step - 1) + shift] < mas[k*step + shift])
		{
			return;
		}
		else
		{
			int tmp = mas[k*step + shift];
			mas[k*step + shift] = mas[k*(step - 1) + shift];
			mas[k*(step - 1) + shift] = tmp;
			Dive(mas, shift, k, step - 1);
		}
	}
}
void Flood(int* mas, int shift, int k, int n)
{
	int step = (int)((n - shift - 1) / k) + 1;
	for (int i = 0; i < step; i++)
	{
		Dive(mas, shift, k, i);
	}
}
void Deluge(int*mas, int k, int n)
{
	for (int i = 0; i < k; i++)
	{
		Flood(mas, i, k, n);
	}
}
void ShellSort(int *mas, int n)
{
	for (int i = 9; i >= 0; i--)
	{
		Deluge(mas, arr[i], n);
	}
}
int* Search(int* mas, int n, int tmp, int& size)
{
	int mid;
	int left = 0;
	int right = n - 1;
	while (right - left > 1)
	{
		mid = (right + left) / 2;
		if (mas[mid] == tmp)
		{
			break;
		}
		if (mas[mid] < tmp)
		{
			left = mid;
		}
		else
		{
			right = mid;
		}
	}
	while ((mid != n - 1) && (mas[mid] < tmp))
	{
		mid++;
	}
	while ((mid != 0) && (mas[mid - 1] > tmp))
	{
		mid--;
	}
	while ((mid != 0) && (mas[mid - 1] == mas[mid]))
	{
		mid--;
	}
	if ((mid == n - 1) && (mas[mid] < tmp))
	{
		size = -1;
		return NULL;
	}
	if ((mid == 0) && (mas[mid] > tmp))
	{
		size = -2;
		return NULL;
	}

	size = mid;
	return mas + mid;
}
void Merge(int* f_mas, int* s_mas, int f_n, int s_n, int f_s, int s_s, int *res, int r_s)
{
	for (int i = 0; i < f_n + s_n; i++)
	{
		if (f_s == f_n)
		{
			res[r_s] = s_mas[s_s];
			s_s++;
			r_s++;
		}
		else
		{
			if (s_s == s_n)
			{
				res[r_s] = f_mas[f_s];
				f_s++;
				r_s++;
			}
			else
			{
				if (f_mas[f_s] < s_mas[s_s])
				{
					res[r_s] = f_mas[f_s];
					f_s++;
					r_s++;
				}
				else
				{
					res[r_s] = s_mas[s_s];
					s_s++;
					r_s++;
				}
			}
		}
	}
}
void Divede_Conquer(int* mas, int* f_mas, int* s_mas, int f_n, int s_n, int step, int*res, int r_s)
{
	int* f_counts = new int[step];
	int** f_points = new int*[step];
	int* s_counts = new int[step];
	int** s_points = new int*[step];
	int* dashs = new int[step];
	int counter = 1;
	bool flag = true;
	f_points[0] = f_mas;
	s_points[0] = s_mas;
	dashs[0] = 0;
	int f_kol = 0;
	int s_kol = 0;
	for (int i = 1; i < step; i++)
	{
		int f_p = (f_n / step)*i;
		int * f_pt = f_mas + f_p;
		int s_p;
		int* s_pt = Search(s_mas, s_n, *f_pt, s_p);
		if (s_pt != NULL)
		{
			f_counts[i - 1] = f_p - f_kol;
			s_counts[i - 1] = s_p - s_kol;
			f_points[i] = f_pt;
			s_points[i] = s_pt;
			f_kol += f_counts[i - 1];
			s_kol += s_counts[i - 1];
			dashs[i] = dashs[0] + f_kol + s_kol;
			counter++;
		}
		else
		{
			if (s_p == -1)
			{
				f_counts[i - 1] = f_p - f_kol;
				s_counts[i - 1] = s_n - s_kol;
				f_kol += f_counts[i - 1];
				s_kol = s_n;
				dashs[i] = dashs[0] + f_kol + s_kol;
				f_counts[i] = f_n - f_kol;
				s_counts[i] = 0;
				f_points[i] = f_pt;
				s_points[i] = s_points[i - 1];
				counter++;
				flag = false;
				break;
			}
			if (s_p == -2)
			{
				f_counts[i - 1] = f_p - f_kol;
				s_counts[i - 1] = 0;
				f_kol += f_counts[i - 1];
				dashs[i] = dashs[0] + f_kol;
				f_points[i] = f_pt;
				s_points[i] = s_points[i - 1];
				counter++;
			}
		}
	}
	if (flag)
	{
		f_counts[step - 1] = f_n - f_kol;
		s_counts[step - 1] = s_n - s_kol;
	}
	for (int i = 0; i < counter; i++)
	{
		Merge(f_points[i], s_points[i], f_counts[i], s_counts[i], 0, 0, res, r_s + dashs[i]);
	}
}
void Sort(int* mas)
{
	int* res_mas = new int[mas_size];
	for (int i = 0; i < count_divene; i++)
	{
		ShellSort(mas_pointer[i], sizes_mas[i]);
	}
	int step = 2;
	while (count_divene != 1)
	{
		count_divene /= 2;
		int dash = 0;
		pointer_k = 0;
		for (int i = 0; i < count_divene; i++)
		{
			Divede_Conquer(mas, mas_pointer[2 * i], mas_pointer[2 * i + 1], sizes_mas[2 * i], sizes_mas[2 * i + 1], step, res_mas, dash);
			shifts[i] = dash;
			dash += sizes_mas[2 * i] + sizes_mas[2 * i + 1];
			sizes_mas[i] = sizes_mas[2 * i] + sizes_mas[2 * i + 1];
		}
		step *= 2;
		for (int i = 0; i < mas_size; i++)
			mas[i] = res_mas[i];
		for (int i = 0; i < count_divene; i++)
		{
			mas_pointer[i] = mas + shifts[i];
		}
	}
}
int main()
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, "input", "rb");
	fread_s(&mas_size, sizeof(int), sizeof(int), 1, fp);
	int * mas = new int[mas_size];
	fread_s(mas, mas_size * sizeof(int), sizeof(int), mas_size, fp);
	fclose(fp);
	mas_pointer = new int*[count_divene];
	sizes_mas = new int[count_divene];
	shifts = new int[count_divene];
	rank = Rank(count_divene);
	int shift = mas_size / count_divene;
	for (int i = 0; i < count_divene; i++)
	{
		mas_pointer[i] = mas + i * shift;
	}
	for (int i = 0; i < count_divene - 1; i++)
	{
		sizes_mas[i] = shift;
	}
	sizes_mas[count_divene - 1] = mas_size - shift * (count_divene - 1);
	Sort(mas);
	err = fopen_s(&fp, "output", "wb");
	fwrite(&mas_size, sizeof(int), 1, fp);
	fwrite(mas, sizeof(int), mas_size, fp);
	fclose(fp);
	return 0;
}