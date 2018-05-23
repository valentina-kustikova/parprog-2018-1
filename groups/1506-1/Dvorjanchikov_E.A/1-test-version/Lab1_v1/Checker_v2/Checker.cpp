#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <string>
#include <iostream>
#include <algorithm>
#define eps 0.0001
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
		if (read) bur = fopen("result.txt", "a+");
		else
			bur = fopen("result.txt", "a+");
	}
	~result()
	{
		fclose(bur);
	}
	void write_message(string str)
	{
		int l = str.size();
		char c = '/';
		fwrite(&str[0], sizeof(str[0]), l, bur);
		fprintf_s(bur, "\n");
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
	double *res_arr = new double[res_size];
	fread(res_arr, sizeof(double), res_size, buo);
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
		//checker_result.write_verdict(verdict::AC);
	}
	else
	{
		checker_result.write_message("WA. Array isn't sorted corrently.");
		//checker_result.write_verdict(verdict::WA);
	}
	delete(arr);
	delete(res_arr);
	fclose(buo);
	fclose(bui);
	return 0;
}