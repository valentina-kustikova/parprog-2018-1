#include <cstdio> 
#include <cmath> 
#include <string>
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

/*
AC = Accepted
WA = Wrong Answer
*/

void String_to_Char(string str, char* ch);



int main(int argc, char * argv[])
{
	ofstream fout;	// файл дл€ записи в него результата
	fout.open("result.txt");

	char* fileIN = "matr.in";
	char* fileOUT = "matr.out";
	char* answer = "answer.ans";

	if (argc > 1)
	{
		fileIN = argv[1];
		string str = string(argv[1]) + string(".out");
		fileOUT = new char[str.length()];
		String_to_Char(str, fileOUT);
		str = string(argv[1]) + string(".ans");
		answer = new char[str.length()];
		String_to_Char(str, answer);
	}
	//ќткрываем файл входных данных, файл выходных данных и ответ 
	FILE * bui;
	fopen_s(&bui, fileIN, "rb");
	FILE * buo;
	fopen_s(&buo, fileOUT, "rb");
	FILE * perfect;
	fopen_s(&perfect, answer, "rb");

	int N = 0;
	int realSize;
	//—читываем размер матрицы
	fread(&realSize, sizeof(int), 1, bui);
	//получаем размер матрицы, который кратный степени 2
	N = (int)(log2(realSize));
	int k = pow(2, N);
	if (realSize == k) { N = realSize; ; }
	else
	{
		N++;
		N = pow(2, N);
	}

	double **ans = new double*[N];
	for (int count = 0; count < N; count++)
		ans[count] = new double[N];

	double **res = new double*[N];
	for (int count = 0; count < N; count++)
		res[count] = new double[N];



	for (int i = 0; i < N; i++) {
		fread(ans[i], sizeof(double), N, buo);
	}


	for (int i = 0; i < N; i++) {
		fread(res[i], sizeof(double), N, perfect);
	}


	bool flag = true;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {

			cout << res[i][j] << " ";
		}
		cout << endl;
	}


	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {

			cout << ans[i][j] << " ";
		}
		cout << endl;
	}


	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {

			if (res[i][j] != ans[i][j]) flag = false;
		}
	}
	if (flag = true) {

		fout << "AC. Numbers are equal." << endl;
	}
	else {
		fout << "WA. Output is not correct." << endl;
	}


	fclose(buo);
	fclose(bui);
	fclose(perfect);
	fout.close();
	return 0;
}


void String_to_Char(string str, char* ch)
{
	for (int i = 0; i < str.length(); i++)
		ch[i] = str[i];
}


int Order(int num) {
	int order = 0;
	while (num) {
		num /= 10;
		order++;
	}
	return order;
}