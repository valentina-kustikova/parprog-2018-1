#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <string>
#include <iostream>
using namespace std;

enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };
class result
{
private:
	FILE * bur;
public:
	enum ext_cls
	{ NO = 1, VERDICT, MESSAGE, TIME, MEMORY };
	result(bool read = false)
	{
		if (read) bur = fopen("result.txt", "r"); 
		else
			bur = fopen("result.txt", "w");
	}
	~result() 
	{ 
		fclose(bur);
	}
	void write_type(ext_cls t)
	{ 
		fwrite(&t, sizeof(t), 1, bur);
	}
	void write_verdict(verdict v)
	{
		write_type(ext_cls::VERDICT);
		fwrite(&v, sizeof(v), 1, bur);
	}
	void write_message(string str)
	{
		write_type(ext_cls::MESSAGE);
		int l = str.size();
		fwrite(&l, sizeof(l), 1, bur);
		fwrite(&str[0], sizeof(str[0]), l, bur);
	}
	void write_time(long long x)
	{
		write_type(ext_cls::TIME);
		fwrite(&x, sizeof(x), 1, bur);
	}
	void write_memory(unsigned long long x)
	{
		write_type(ext_cls::MEMORY);
		fwrite(&x, sizeof(x), 1, bur);
	}
} checker_result;

int compare(const double *a, const double *b)
{
	return *a - *b;
}

int main(int argc, char * argv[])
{
	if (argc != 3)
	{
		std::cout << "'please, enter checker [input.bin] [res.bin] '" << std::endl;
		return 1;
	}
	FILE *bui = fopen(argv[1], "rb");
	FILE * buo = fopen(argv[2], "rb");
	int size;
	fread(&size, sizeof(int), 1, bui);
	double *arr = new double[size];
		fread(arr, sizeof(*arr), size, bui);
		qsort(arr, size, sizeof(double), (int(*) (const void *, const void *))compare);
		int res_size;
	bool flag = false;
	double time;
	fread(&time, sizeof(time), 1, buo); 
	fread(&res_size, sizeof(res_size), 1, buo);	
	double *res = new double[res_size];
	fread(res, sizeof(*res), res_size, buo);
	for (int i = 0; i < size; i++)
	{
		if (abs(res[i] - arr[i]) > 0.01)
		{
			flag = true;
			break;
		}
	}

	if (flag == false) {

		checker_result.write_message("AC. Array is sorted correctly.");
		checker_result.write_verdict(verdict::AC);
	}
	else
	{
		checker_result.write_message("WA. Array isn't sorted corrently.");
		checker_result.write_verdict(verdict::WA);
	}

	checker_result.write_time((time * 1e7));
	fclose(buo);
	fclose(bui);
	delete arr;
	return 0;
}