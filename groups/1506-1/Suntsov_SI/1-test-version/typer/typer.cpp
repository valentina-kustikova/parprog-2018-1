#include <cstdio> 
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

int main(int argc, char * argv[])
{

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
	//считываем размер матрицы
	int N;
	fin >> N;
	cout << N << endl;
	double* A = new double[N*N];
	double* B = new double[N*N];
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
	FILE *in;
	freopen_s(&in, "mat.in", "wb", stdout);


	fwrite(&N, sizeof(N), 1, stdout);

	fwrite(A, sizeof(*A), N*N, stdout);
	fwrite(B, sizeof(*B), N*N, stdout);

	fin.close();
	fclose(in);
	return 0;
}