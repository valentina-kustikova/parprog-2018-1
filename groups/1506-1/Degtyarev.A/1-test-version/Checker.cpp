#include <iostream>

struct crsMatrix
{
	int Size; // Размер матрицы
	int Size_Z; // Кол-во ненулевых

	double* Value; // Массив значений
	int* Col; // Массив номеров столбцов
	int* Row_Index;	// Массив индексов строк

	crsMatrix() {}

	crsMatrix(int N, int NZ)
	{
		Size = N;
		Size_Z = NZ;
		Value = new double[NZ];
		Col = new int[NZ];
		Row_Index = new int[N + 1];
	}
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


void Copying_dense(double* matr, crsMatrix A, int size)
{
	int i1, i2;
	for (int i = 0; i < size; i++)
	{
		i1 = A.Row_Index[i];
		i2 = A.Row_Index[i + 1] - 1;
		for (int j = i1; j <= i2; j++)
			matr[i*size + A.Col[j]] = A.Value[j];
	}
}

double* ReadStandartMult(char* file)
{
	FILE* matr;
	int N = 1;

	freopen_s(&matr, file, "rb", stdin);
	fread(&N, sizeof(N), 1, stdin);

	double* ans = new double[N*N];
	for (int i = 0; i < N; i++)
		ans[i] = 0;
	fread(ans, sizeof(*ans), N*N, stdin);
	
	fclose(matr);

	return ans;
}

double* ReadParticipantMult(char* file, int &size, double &time)
{
	FILE* matr;
	int N = 1, Nz = 1;

	freopen_s(&matr, file, "rb", stdin);
	fread(&N, sizeof(N), 1, stdin);
	fread(&Nz, sizeof(Nz), 1, stdin);
	int size_nonzero = Nz;
	size = N;
	double* res = new double[N*N];
	for (int i = 0; i < N*N; i++)
		res[i] = 0;

	crsMatrix C(N, size_nonzero);

	fread(C.Value, sizeof(*C.Value), size_nonzero, stdin);
	fread(C.Col, sizeof(*C.Col), size_nonzero, stdin);
	fread(C.Row_Index, sizeof(*C.Row_Index), N + 1, stdin);
	fread(&time, sizeof(time), 1, stdin);
	fclose(matr);

	Copying_dense(res,C,N);

	return res;
}

int Checker(char* file_p, char* file_s, double& time)
{
	int N;

	double* ans;
	double* res;

	res = ReadParticipantMult(file_p,N,time);
	ans = ReadStandartMult(file_s);

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (ans[i*N + j] - res[i*N + j] != 0)
				return 0;
		}
	}

	delete[] ans;
	delete[] res;

	return 1;
}


int main(int argc, char* argv[])
{

	//   номер теста						1	2  3	4    5    6    7    8    9     10
	//   размер матрицы						2 100 1000 2000 3000 4000 5000	1000 2000  4000  
	//	 количество ненулевых элементов		1  10  100  100  100  100  100  250	 250   250
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