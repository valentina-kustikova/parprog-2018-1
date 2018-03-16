// typer.cpp получает на вход имя текстового файла (через аргумент командой строки, например), 
// в котором записаны входные данные в текстовом виде, и преобразует их в бинарный вид.

#include <cstdio> 
#include <omp.h> 
#include <random> 
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

int main(int argc, char * argv[])
{
	setlocale(0, "RUS");
	ifstream fin;
	char* name = new char[20];
	if (argc > 1) {
		name = argv[1];
		cout << name << endl;
		fin.open(name);
	}
	else {
		fin.open("matr.txt");
	}
	if (!fin.is_open())
	{
		cout << "Файл не найден!" << endl;
		return 0;
	}
	//считываем размер матрицы
	int N;
	fin >> N;
	cout << N << endl;
	double* A = new double[N*N];
	double* B = new double[N*N];
	//считываем две входные матрицы
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			fin >> A[i*N + j];
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			fin >> B[i*N + j];
		}
	}
	//если матрицы небольшие - выводим на экран
	if (N < 11) {
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				cout << A[i*N + j] << " ";
			}
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				cout << B[i*N + j] << " ";
			}
			cout << endl;
		}
	}
	FILE *stream;
	freopen_s(&stream, "matr.in", "wb", stdout);

	// записываем в бинарном виде размерность матриц   
	fwrite(&N, sizeof(N), 1, stdout);

	// записываем матрицы в бинарном виде в файл    
	fwrite(A, sizeof(*A), N*N, stdout);
	fwrite(B, sizeof(*B), N*N, stdout);

	fin.close();
	fclose(stream);
	return 0;
}