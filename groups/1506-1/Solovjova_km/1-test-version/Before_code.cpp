
//before_code.cpp � ������ ������ � ����� ����� � ���������
//����������
//��������� �� ����� C++ ������ ������������� ������� �� ��������� ���������� :
//(double *functionValues, double x1, double y1, double x2, double y2, int N);
//������ ������� ������
//������� �������� � ���������� ��������� ���������� :
// double *functionValues - ������ �������� �������
// double x1 - ����� ������� �
// double x2 - ������ ������� �
// double y2 - ������ ������� Y
// double x2 - ������ ������� �
//������ �������� ������
//����������� ��������� - ��� ����� ���� double

#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <omp.h>
#include <random>
#include<iostream>
#include <fstream>
#include<conio.h>
#include<string>
#include<cmath>
#include <ctime>
#include <chrono>
using namespace std;


double MonteCarlo(double *functionValues, double x1, double y1, double x2, double y2, int N);



int main(int argc, char * argv[])
{
	//������ ������� - ��� 2 �����, ������ ������� - ��� ������� �����
	char *nameFile = new char[250];
	char *nameFile2 = new char[250];
	//if (argc > 1)
	//{
	//	//nameFile = argv[argc - 1];
	//	//std::cout << nameFile << std::endl;
	//	//nameFile2 = argv[argc - 2];
	//	//std::cout << nameFile2 << std::endl;
	//}
	//else
	//{
	//	std::cout << "Input nameFile" << std::endl;
	//	std::cin.getline(nameFile, 250);
	//	std::cout << "Input nameFile2" << std::endl;
	//	std::cin.getline(nameFile2, 250);
	//}

	//������� ������

	double x1;
	double y1;
	double x2;
	double y2;
	int N;
	double* functionValues;

	//��������� ���� ��������� ������
	freopen("test", "rb", stdin);

	//������ ������ �� �����
	fread(&N, sizeof(N), 1, stdin);
	fread(&x1, sizeof(x1), 1, stdin);
	fread(&x2, sizeof(x2), 1, stdin);
	fread(&y1, sizeof(y1), 1, stdin);
	fread(&y2, sizeof(y2), 1, stdin);

	 //������ ��������
	functionValues = new double[N];
	fread(functionValues, sizeof(*functionValues), N, stdin);
	fclose(stdin);
	//omp_set_num_threads(1);

	//�������� �����
	//double start = omp_get_wtime();
	auto begin = chrono::high_resolution_clock::now();
	double res_ser = MonteCarlo(functionValues, x1, y1, x2, y2, N);
	//double end = omp_get_wtime();
	auto end = chrono::high_resolution_clock::now();
	double time_ser = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

	//���������� ������ � ����
	freopen("ser.ans", "wb", stdout);
	fwrite(&time_ser, sizeof(time_ser), 1, stdout);
	fwrite(&res_ser, sizeof(res_ser), 1, stdout);
	//std::cout << "time = " << time_ser;
	//std::cout << "res = " << res_ser;
	fclose(stdout);
	
	//std::cout << "x1 =" << x1 << " x2= " << x2 << " y1= " << y1 << " y2= " << y2;
	//std::cout << "time = " << time;
	//std::cout << "res = " << time;
	delete[]functionValues;
	delete[]nameFile;
	delete[]nameFile2;

	return 0;
}