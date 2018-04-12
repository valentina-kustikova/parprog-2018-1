#include <iostream>
#include "mkl_spblas.h"

struct crsMatrix
{
	int Size; // Размер матрицы
	int Size_Z; // Кол-во ненулевых

	double* Value; // Массив значений
	int* Col; // Массив номеров столбцов
	int* Row_Index;	// Массив индексов строк
};

/*
	// Checker может устанавливать вот эти три вердикта:
	AC = Accepted = Решение выдаёт корректный результат на данном тесте
	WA = Wrong Answer = Решение выдаёт некорректный результат на данном тесте
	PE = Presentation Error = Ошибка формата выходных данных

	// Остальные вердикты checker не может устанавливать

	NO = No verdict = Вердикт отсутствует
	CE = Compilation Error = Ошибка компиляции
	ML = Memory Limit Exceeded = Превышено ограничение по памяти
	TL = Time Limit Exceeded = Превышено ограничение по времени работы
	RE = Runtime Error = Ошибка времени исполнения программы
	IL = Idle Limit Exceeded = Превышено время простоя (бездействия) программы
	DE = Deadly Error = Ошибка тестирующей системы
*/

void InitMatr(int N, int NZ, crsMatrix &matr)
{
	matr.Size = N;
	matr.Size_Z = NZ;
	matr.Value = new double[NZ];
	matr.Col = new int[NZ];
	matr.Row_Index = new int[N + 1];
}

void FreeMatr(crsMatrix &matr)
{
	delete[] matr.Col;
	delete[] matr.Value;
	delete[] matr.Row_Index;
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
		if (read) fopen_s(&bur,"result.txt", "r"); else fopen_s(&bur, "result.txt", "w");
	}
	~result() { fclose(bur); }
	void write_type(ext_cls t) { fwrite(&t, sizeof(t), 1, bur); }

	// Сообщить тестирующей системе, что решение получило один из вердиктов verdict
	void write_verdict(verdict v)
	{
		write_type(ext_cls::VERDICT); fwrite(&v, sizeof(v), 1, bur);
	}

	// Написать сообщение от checker'a пользователю.
	// Например, что решение верное, или неверное.
	// Использовать только латинские буквы и знаки препинания

	void write_message(std::string str)
	{
		write_type(ext_cls::MESSAGE); int l = str.size(); fwrite(&l, sizeof(l), 1, bur);
		fwrite(&str[0], sizeof(str[0]), l, bur);
	}

	// Сообщить тестирующей системе время работы программы участника,
	// вычисленное с помощью before_code
	// x имеет размерность 100 нс = 10 ^ (-7) сек
	void write_time(long long x)
	{
		write_type(ext_cls::TIME); fwrite(&x, sizeof(x), 1, bur);
	}
	// Сообщить тестирующей системе, память затребованную программой участника
	void write_memory(unsigned long long x)
	{
		write_type(ext_cls::MEMORY); fwrite(&x, sizeof(x), 1, bur);
	}
} checker_result;
////////////////////////////////////////////////////////////////////////////////////////////


int SparseDiff(crsMatrix A, crsMatrix B,double& diff)
{

	int Size = A.Size;
	// Будем вычислять C = A - B, используя MKL
	// Структуры данных в стиле MKL

	double *c = 0; // Значения
	int *jc = 0; // Номера столбцов (нумерация с единицы)
	int *ic; // Индексы первых элементов строк
			 // (нумерация с единицы)
			 // Настроим параметры для вызова функции MKL
			 // Переиндексируем матрицы A и B с единицы
	int i, j;
	for (i = 0; i < A.Size_Z; i++)
		A.Col[i]++;
	for (i = 0; i < B.Size_Z; i++)
		B.Col[i]++;
	for (j = 0; j <= A.Size; j++)
	{
		A.Row_Index[j]++;
		B.Row_Index[j]++;
	}
	// Используется функция, вычисляющая C = A + beta*op(B)
	char trans = 'N'; 

	int request;
	
	int sort = 0;
	// beta = -1 -> C = A + (-1.0) * B;

	double beta = -1.0;

	int nzmax = -1;

	// Служебная информация
	int info;
	// Выделим память для индекса в матрице C
	ic = new int[Size + 1];
	// Сосчитаем количество ненулевых элементов в матрице C
	request = 1;

	std::cout << "5" << std::endl;
	mkl_dcsradd(&trans, &request, &sort, &Size, &Size,
		A.Value, A.Col, A.Row_Index, &beta,
		B.Value, B.Col, B.Row_Index,
		c, jc, ic, &nzmax, &info);
	std::cout << "6" << std::endl;
	int nzc = ic[Size] - 1;
	c = new double[nzc];
	jc = new int[nzc];
	// Сосчитаем C = A - B
	request = 2;
	mkl_dcsradd(&trans, &request, &sort, &Size, &Size,
		A.Value, A.Col, A.Row_Index, &beta,
		B.Value, B.Col, B.Row_Index,
		c, jc, ic,
		&nzmax, &info);

	// Сосчитаем max|Cij|
	diff = 0.0;
	for (i = 0; i < nzc; i++)
	{
		double var = fabs(c[i]);
		if (var > diff)
		{
			diff = var;
			return 0;
		}
			
	}

	// Приведем к исходному виду матрицы A и B
	for (i = 0; i < A.Size_Z; i++)
		A.Col[i]--;
	for (i = 0; i < B.Size_Z; i++)
		B.Col[i]--;
	for (j = 0; j <= Size; j++)
	{
		A.Row_Index[j]--;
		B.Row_Index[j]--;
	}
	// Освободим память
	delete[] c;
	delete[] ic;
	delete[] jc;

	return 1;
}


void ReadStandartMult(char* file, crsMatrix& C)
{
	FILE* matr;
	int N = 1;
	int non_zero = 1;

	freopen_s(&matr, file, "rb", stdin);
	fread(&N, sizeof(N), 1, stdin);
	fread(&non_zero, sizeof(N), 1, stdin);


	InitMatr(N, non_zero, C);

	fread(C.Value, sizeof(*C.Value), non_zero, stdin);
	fread(C.Col, sizeof(*C.Col), non_zero, stdin);
	fread(C.Row_Index, sizeof(*C.Row_Index), N + 1, stdin);
	fclose(matr);

}



void ReadParticipantMult(char* file, crsMatrix& C, double &time)
{
	FILE* matr;
	int N = 1;
	int non_zero = 1;

	freopen_s(&matr, file, "rb", stdin);
	fread(&N, sizeof(N), 1, stdin);
	fread(&non_zero, sizeof(N), 1, stdin);

	InitMatr(N, non_zero, C);

	fread(C.Value, sizeof(*C.Value), non_zero, stdin);
	fread(C.Col, sizeof(*C.Col), non_zero, stdin);
	fread(C.Row_Index, sizeof(*C.Row_Index), N + 1, stdin);
	fread(&time, sizeof(N), 1, stdin);

	fclose(matr);

}

int Checker(char* file_p, char* file_s, double& time)
{
	crsMatrix C1, C2;
	double diff = 0.0;

	ReadParticipantMult(file_p, C1, time);
	ReadStandartMult(file_s,C2);

	SparseDiff(C1,C2, diff);
	if (diff > 0)
		return 0;


	FreeMatr(C1);
	FreeMatr(C2);
	return 1;
}


int main(int argc, char* argv[])
{
	double res_time;
	if (argc != 3)
	{
		std::cout << "Invalid input parameters\n" << std::endl;
		checker_result.write_message("PE. Presentation Error.");
		checker_result.write_verdict(verdict::PE);
		return 0;
	}
	
	if (Checker(argv[1], argv[2], res_time))
	{
		checker_result.write_message("AC. Numbers are equal.");
		checker_result.write_verdict(verdict::AC);		std::cout << "GOOD" << std::endl;
	}
	else
	{
		checker_result.write_message("WA. Output is not correct.");
		checker_result.write_verdict(verdict::WA);		std::cout << "BAD" << std::endl;	}

	// Записываем время в правильной размерности (интервалы по 100 нс = 10 ^ (-7) сек).
	checker_result.write_time(res_time);
	return 0; 
}