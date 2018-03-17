#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>

using namespace std;

int comp1(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
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
		if (read) bur = fopen("result.txt", "r"); else bur = fopen("result.txt", "w");
	}
	~result() { fclose(bur); }
	void write_type(ext_cls t) { fwrite(&t, sizeof(t), 1, bur); }
	// —ообщить тестирующей системе, что решение получило один из вердиктов verdict
	void write_verdict(verdict v)
	{
		write_type(ext_cls::VERDICT); fwrite(&v, sizeof(v), 1, bur);
	}
	// Ќаписать сообщение от checker'a пользователю.
	// Ќапример, что решение верное, или неверное.
	// »спользовать только латинские буквы и знаки препинани€
	void write_message(string str)
	{
		write_type(ext_cls::MESSAGE); int l = str.size(); fwrite(&l, sizeof(l), 1, bur);
		fwrite(&str[0], sizeof(str[0]), l, bur);
	}
	// —ообщить тестирующей системе врем€ работы программы участника,
	// вычисленное с помощью before_code
	// x имеет размерность 100 нс = 10 ^ (-7) сек
	void write_time(long long x)
	{
		write_type(ext_cls::TIME); fwrite(&x, sizeof(x), 1, bur);
	}
	// —ообщить тестирующей системе, пам€ть затребованную программой участника
	void write_memory(unsigned long long x)
	{
		write_type(ext_cls::MEMORY); fwrite(&x, sizeof(x), 1, bur);
	}
} checker_result;

void show(int* a, int n){
	for (int i = 0; i < n; i++){
		cout << a[i] << "  ";
	}
	cout << endl;
}

int check(int* a, int* b, int n){
	int k = 0;
	for (int i = 0; i < n; i++){
		if (a[i] != b[i]){
			k++;
			break;
		}
	}
	return k;
}

int main(){

	int n;
	int* sorted;
	int* mixed;
	// read from abinary file	 
	FILE *in1 = fopen("sorted_array.in", "rb");
	if (!in1) {
		return 0;
	}
	//read length of array
	fread(&n, sizeof(size_t), 1, in1);
	//initializing 
	sorted = new int[n];
	// read array
	//	fseek(in1, 0 + sizeof(int), SEEK_SET);
	fread(sorted, sizeof(int), n, in1);
	fclose(in1);
	//-----------------------------------------------------------------------------
	// read from abinary file	 
	FILE *in2 = fopen("array.in", "rb");
	if (!in2) {
		return 0;
	}
	//read length of array
	fread(&n, sizeof(size_t), 1, in2);
	//initializing 
	mixed = new int[n];
	// read array
	//fseek(in2, 0+sizeof(int), SEEK_SET);
	fread(mixed, sizeof(int), n, in2);
	fclose(in2);
	//-----------------------------------------------------------------------------
	qsort(mixed, n, sizeof(int), comp1);

	//show(sorted, n);
	//show(mixed, n);
	
	int m = check(sorted, mixed, n);
	if (m == 0) {
		checker_result.write_message("AC. Arrays are equal.");
		checker_result.write_verdict(verdict::AC);
	}
	else{
		checker_result.write_message("WA. Output is not correct.");
		checker_result.write_verdict(verdict::WA);
	}


	return 0;
}