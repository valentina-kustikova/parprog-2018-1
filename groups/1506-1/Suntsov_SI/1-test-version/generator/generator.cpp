
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <fstream>
#include <Windows.h>
#include <string>

using namespace std;


int n_tests[] = { 1, 4, 6, 8, 9, 100, 80, 700, 50, 1021, 1024 };

void String_to_Char(string str, char* ch)
{
	for (int i = 0; i < str.length(); i++)
		ch[i] = str[i];
}


int main(int argc, char* argv[])
{

	char* fileName = "matr.in";
	char* answerName = "answer.ans";
	char* answerName1;
	FILE* matr_in;
	FILE *perfect;
	int n = 64;
	ofstream fout("cppstudio.txt");


	default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
	uniform_real_distribution<double> distribution(1, 4);
	freopen_s(&matr_in, "matr.in", "wb", stdout);
	int realSize = 64;
	if (argc > 1)
	{
		realSize = n_tests[atoi(argv[1])];
		//задаем им€ дл€ файла с результатом
		fileName = argv[1];
		string str = string(argv[1]) + ".ans";
		//cout << str;
		/*answerName1 = new char[str.length()];
		String_to_Char(str, answerName1);*/
	}
	// «аписываем в бинарном виде размерность матриц

	fwrite(&realSize, sizeof(realSize), 1, stdout);
	//получаем размер матрицы, который кратный степени 2
	cout << realSize;
	//n = (int)(log2(realSize));
	//int k = pow(2, n);
	//if (realSize == k) { n = realSize; ; }
	//else
	//{
	//	n++;
	//	n= pow(2, n);
	//}
	//инициализируем матрицы
	double **A = new double*[realSize];
	for (int i = 0; i < realSize; i++)
		A[i] = new double[realSize];

	double **B = new double*[realSize];
	for (int i = 0; i < realSize; i++)
		B[i] = new double[realSize];
	//создаем временный массив дл€ строки матрицы

	double * cur = new double[realSize];
	double tmp = 0; // временный элемент дл€ хранени€ случайного числа


					//«аполн€ем и записываем матрицу ј
	for (int i = 0; i < realSize; i++) {
		for (int j = 0; j < realSize; j++) {
			tmp = distribution(generator);
			cur[j] = tmp;
			A[i][j] = tmp;
			//A1[i][j] = tmp;

		}
		fwrite(cur, sizeof(*cur), n, stdout);
	}

	//«аполн€ем и записываем матрицу ¬
	for (int i = 0; i < realSize; i++) {
		for (int j = 0; j < realSize; j++) {
			tmp = distribution(generator);
			cur[j] = tmp;
			B[i][j] = tmp;
			//B1[i][j] = tmp;

		}
		fwrite(cur, sizeof(*cur), realSize, stdout);
	}

	for (int i = 0; i < realSize; i++) {
		for (int j = 0; j < realSize; j++) { cout << A[i][j] << " "; }
		cout << endl;
	}


	//‘ормирование заведомо правильного результата вынес в отдельный проект
	/*std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	MatrMulti(A, B, C, n);
	end = std::chrono::system_clock::now();

	double time = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();



	freopen_s(&perfect, "answer.ans", "wb", stdout);

	for (int i = 0; i < n; i++){
	fwrite(C[i], sizeof(double), n, perfect);
	}
	fwrite(&time, sizeof(time), 1, perfect);
	fclose(perfect);
	*/

	fclose(matr_in);

	return 0;

}