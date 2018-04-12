#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <string>
#include <iostream>
#include <algorithm>
#define eps 0.00001
using namespace std;
/*
// Checker can set these three verdicts:
AC = Accepted = gives a correct answer to this test
WA = Wrong Answer = gives wrong answer to this test
PE = Presentation Error
// Other verdicts checker can not set
NO = No verdict
CE = Compilation Error
ML = Memory Limit Exceeded
TL = Time Limit Exceeded
RE = Runtime Error
IL = Idle Limit Exceeded
DE = Deadly Error = error of testing system 
*/
enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };
class result
{
private:
	FILE * bur;
public:
	enum ext_cls
	{
		NO = 1, VERDICT, MESSAGE, TIME, MEMORY
	};
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

int compare(const int *a, const int *b)
{
	return *a - *b;
}

int main(int argc, char * argv[])
{
	FILE *bui;
	FILE * buo;
	if (argc >= 2)
	{
		bui = fopen(argv[1], "rb");
	}
	else
	{
		bui = fopen("input", "rb");
	}
	if (argc == 3)
	{
		buo = fopen(argv[2], "rb");
	}
	else
	{
		buo = fopen("output", "rb");
	}
	int size;
	fread(&size, sizeof(int), 1, bui);
	double *arr = new double[size];
	fread(arr, sizeof(double), size, bui);
	sort(arr, arr+size);
	int res_size;
	fread(&res_size, sizeof(int), 1, buo);
	double *res_arr = new double[size];
	fread(res_arr, sizeof(double), size, buo);
	bool flag = false;
	double time;
	int cur = 0;
	for (int i = 0; i < size; i++)
	{
		if (abs(res_arr[i] - arr[i])>eps)
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
	delete(arr);
	delete(res_arr);
	fclose(buo);
	fclose(bui);
	return 0;
}