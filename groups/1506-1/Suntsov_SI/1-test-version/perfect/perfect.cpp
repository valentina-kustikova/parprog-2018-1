
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <fstream>
#include <Windows.h>
#include <string>




using namespace std;

void MatrMulti(double* A, double* B, double* C, int N, int realSize)
{
	for (int i = 0; i < realSize; i++)
		for (int j = 0; j < realSize; j++)
		{
			for (int k = 0; k < N; k++)
			{
				C[i*N + j] += A[i*N + k] * B[k*N + j];
			}
		}

}


void String_to_Char(string str, char* ch)
{
	for (int i = 0; i < str.length(); i++)
		ch[i] = str[i];
}

int main(int argc, char* argv[])
{

	char* fileName = "mat.in";
	char* answerName = "answer.ans";
	FILE* matr_in;
	FILE *perfect;
	int realSize = 0;

	if (argc > 1) {
		fileName = argv[1];
		string str = string(argv[1]) + string(".out");
		answerName = new char[str.length()];
		String_to_Char(str, answerName);
	}
	freopen_s(&matr_in, fileName, "rb", stdin);

	fread(&realSize, sizeof(int), 1, stdin);
	int N = 0;
	//получаем размер матрицы, который кратный степени 2
	N = (int)(log2(realSize));
	int k = pow(2, N);
	if (realSize == k) { N = realSize; ; }
	else
	{
		N++;
		N = pow(2, N);
	}

	//считываем исходные матрицы 
	double *A = new double[N*N];
	double *B = new double[N*N];
	double *C = new double[N*N];

	for (int i = 0; i<N; i++) {
		for (int j = 0; j<N; j++) {

			A[i*N + j] = 0;
			B[i*N + j] = 0;
			C[i*N + j] = 0;

		}

	}


	for (int i = 0; i<realSize; i++) {
		for (int j = 0; j<realSize; j++) {
			fread(&A[i*N + j], sizeof(double), 1, stdin);
		}

	}

	for (int i = 0; i < realSize; i++) {
		for (int j = 0; j < realSize; j++) {

			fread(&B[i*N + j], sizeof(double), 1, stdin);
		}
	}


	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	MatrMulti(A, B, C, N, realSize);
	end = std::chrono::system_clock::now();

	double time = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();



	freopen_s(&perfect, "answer.ans", "wb", stdout);

	fwrite(C, sizeof(*C), N*N, perfect);

	fwrite(&time, sizeof(time), 1, perfect);

	return 0;
}