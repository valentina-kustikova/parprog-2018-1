#define _CRT_SECURE_NO_WARNINGS
#include"cstdio"
#include "iostream"
#include <ctime>
#include <algorithm>
#include <fstream>

using namespace std;






enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };
class result
{
private:
	FILE * bur;
public:
	enum ext_cls { NO = 1, VERDICT, MESSAGE, TIME, MEMORY };
	result(bool read = false)
	{
		if (read) 
			bur = fopen("result.txt", "rb"); 
		else 
			bur = fopen("result.txt", "wb");
	}
	~result() { fclose(bur); }
	void write_type(ext_cls t) 
	{
		//fwrite(&t, sizeof(t), 1, bur);
		fprintf(bur, "%d ", t);
	}
	// —ообщить тестирующей системе, что решение получило один из вердиктов verdict
	void write_verdict(verdict v)
	{
		write_type(ext_cls::VERDICT); 
		//fwrite(&v, sizeof(v), 1, bur);
		fprintf(bur, "%d ", v);
	}
	// Ќаписать сообщение от checker'a пользователю.
	// Ќапример, что решение верное, или неверное.
	// »спользовать только латинские буквы и знаки препинани€
	void write_message(string str)
	{
		write_type(ext_cls::MESSAGE); 
		int l = str.size(); 
		//fwrite(&l, sizeof(l), 1, bur);
		//fwrite(&str[0], sizeof(str[0]), l, bur);
		fprintf(bur, "%d ", l);
		fprintf(bur, "%s ", &str[0]);

	}
	// —ообщить тестирующей системе врем€ работы программы участника,
	// вычисленное с помощью before_code
	// x имеет размерность 100 нс = 10 ^ (-7) сек
	void write_time(long long x)
	{
		write_type(ext_cls::TIME); 
		//fwrite(&x, sizeof(x), 1, bur);
		fprintf(bur, "%d ", x);
	}
	// —ообщить тестирующей системе, пам€ть затребованную программой участника
	void write_memory(unsigned long long x)
	{
		write_type(ext_cls::MEMORY); 
		//fwrite(&x, sizeof(x), 1, bur);
		fprintf(bur, "%d ", x);

	}
} checker_result;








int main(int argc, char* argv[])
{
	char* name = "SortedExArr.out";
	char* answ = "ExIdealAnsw.out";


	if (argc > 2)
	{
		name = argv[1];
		answ = argv[2];
	}

	FILE* bui = fopen(name, "rb");
	FILE* buo = fopen(answ, "rb");


	int size;
	int size2;
	bool OK = true;

	fread(&size, sizeof(int), 1, bui);
	int* mas = new int[size];
	int* sorted = new int[size];

	fread(mas, sizeof(int), size, bui);
	fclose(bui);

	fread(&size2, sizeof(int), 1, buo);
	fread(sorted, sizeof(int), size, buo);
	fclose(buo);


	for (int i = 0; i < size; i++)
	{
		if (mas[i] != sorted[i])
		{
			OK = false;
			break;
		}
	}

	if (OK == true)
	{
		checker_result.write_message("AC.Arrays are equal.");
		checker_result.write_verdict(verdict::AC);
	}
	else
	{
			checker_result.write_message("WA. Output is not correct.");
			checker_result.write_verdict(verdict::WA);
	}


	delete[] sorted;
	delete[] mas;

	return 0;
}