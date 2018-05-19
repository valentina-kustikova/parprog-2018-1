#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>

using namespace std;

int comp1(const void * a, const void * b)
{
	int res;
	if (*(double*)a < *(double*)b)
		res = -1;
	else if (*(double*)a - *(double*)b < 1e-5)
		res = 0;
	else
		res = 1;
	return res;
}

enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };

class result
{
private:
	FILE * bur;
public:
	enum ext_cls { NO = 1, VERDICT, MESSAGE, TIME, MEMORY };

	result(bool read = false)
	{
		if (read) bur = fopen("result.txt", "rb"); else bur = fopen("result.txt", "wb");
	}
	~result() { fclose(bur); }

	void write_type(ext_cls t) { 
		fprintf(bur, "%d ",t);
	}
	// —ообщить тестирующей системе, что решение получило один из вердиктов verdict
	void write_verdict(verdict v)
	{
		write_type(ext_cls::VERDICT); fprintf(bur, "%d ", v);
	}

	// Ќаписать сообщение от checker'a пользователю.
	// Ќапример, что решение верное, или неверное.
	// »спользовать только латинские буквы и знаки препинани€
	void write_message(string str)
	{
		write_type(ext_cls::MESSAGE); int l = str.size(); fprintf(bur, "%d ", l);
		fwrite(&str[0], sizeof(str[0]), l, bur);
	}

	// —ообщить тестирующей системе врем€ работы программы участника,
	// вычисленное с помощью before_code
	// x имеет размерность 100 нс = 10 ^ (-7) сек

	void write_time(long long x)
	{
		write_type(ext_cls::TIME); fprintf(bur, "%d ", x);
	}
	// —ообщить тестирующей системе, пам€ть затребованную программой участника

	void write_memory(unsigned long long x)
	{
		write_type(ext_cls::MEMORY); fprintf(bur, "%d ", x);
	}

} checker_result;

void show(double* a, int n){
	for (int i = 0; i < n; i++){
		cout << a[i] << "  ";
	}
	cout << endl;
}

int check(double* a, double* b, int n){

	int ans = 0;
	double k = 0.0;
	for (int i = 0; i < n; i++){
		k += (a[i] - b[i])*(a[i] - b[i]);
		}
	if (k < 1e-6){

		ans = 0;
	}
	else{
		ans = 1;
	}

	return ans;
}

int main(int argc, char* argv[]){
	
	int n;
	double* sorted;
	double* mixed;
	FILE *in1;
	FILE *in2;

	if (argc > 2){
		// read from abinary file	 
		in1 = fopen(argv[1], "rb"); // sorted_array.in
		if (!in1) {
			return 0;
		}
		in2 = fopen(argv[2], "rb");//array.in
		if (!in2) {
			return 0;
		}
	}
	else{
		in1 = fopen("sorted_array.in", "rb"); // sorted_array.in
		if (!in1) {
			return 0;
		}

		in2 = fopen("array.in", "rb");//array.in
		if (!in2) {
			return 0;
		}

	}



	//read length of array
	fread(&n, sizeof(int), 1, in1);
	//initializing 
	sorted = new double[n];
	// read array
	fread(sorted, sizeof(double), n, in1);
	double time_res = 0;
	fread(&time_res, sizeof(double), 1, in1);
	fclose(in1);
	//-----------------------------------------------------------------------------
	// read from abinary file	 
	
	//read length of array
	fread(&n, sizeof(int), 1, in2);
	//initializing 
	mixed = new double[n];
	// read array
	//fseek(in2, 0+sizeof(int), SEEK_SET);
	fread(mixed, sizeof(double), n, in2);
	fclose(in2);
	//-----------------------------------------------------------------------------
	qsort(mixed, n, sizeof(double), comp1);

	
	int m = check(sorted, mixed, n);
	if (m == 0) {
		checker_result.write_message("AC. Arrays are equal.");
		checker_result.write_verdict(verdict::AC);
	}
	else{

		checker_result.write_message("WA. Output is not correct.");
		checker_result.write_verdict(verdict::WA);
	}

	checker_result.write_time(time_res*1e7);

	delete[] sorted;
	delete[] mixed;
	return 0;
}