#include <ctime>
#include <random> 
#include <iostream>

struct crsMatrix
{
	int Size; // Размер матрицы
	int Size_Z; // Кол-во ненулевых

	double* Value; // Массив значений
	int* Col; // Массив номеров столбцов
	int* Row_Index;	// Массив индексов строк
};

void InitMatr(int N, int NZ,crsMatrix &matr)
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

double gen_double()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> distr(-1e4, 1e4); 

	return distr(gen);
}

crsMatrix Generate_CRS(int size, int cntInRow, crsMatrix& matr)
{
	bool flag = false;
	int notNull = cntInRow * size;

	InitMatr(size, notNull, matr);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < cntInRow; j++)
		{
			do
			{
				matr.Col[i * cntInRow + j] = rand() % size;
				flag = false;
				for (int k = 0; k < j; k++)				// если cntInRow ненулевых элемента, то они в разных столбцах одной строки
				{
					if (matr.Col[i * cntInRow + j] == matr.Col[i * cntInRow + k])
						flag = true;
				}
			} while (flag);
		}

		// Сортируем номера столцов в строке i
		for (int j = 0; j < cntInRow - 1; j++)	// если cntInRow ненулевых элемента, то сортировка для них
		{
			for (int k = 0; k < cntInRow - 1; k++)
				if (matr.Col[i * cntInRow + k] > matr.Col[i * cntInRow + k + 1])
				{
					int tmp = matr.Col[i * cntInRow + k];
					matr.Col[i * cntInRow + k] = matr.Col[i * cntInRow + k + 1];
					matr.Col[i * cntInRow + k + 1] = tmp;
				}
		}

	}

	// Заполняем массив значений
	for (int i = 0; i < cntInRow * size; i++)
		matr.Value[i] = gen_double();

	// Заполняем массив индексов строк
	int tmp = 0;
	for (int i = 0; i <= size; i++)
	{
		matr.Row_Index[i] = tmp;
		tmp += cntInRow;
	}
	return matr;
}

// 2,100,1000,2000,3000,2000,2500,1000,2500,
// 1,10,100,100,100,1000,500,250,1000,




int main(int argc, char* argv[])
{
	FILE *matr;
	int N = 2, Nz = 1;

	if (argc != 4)
	{
		std::cout << "Invalid input parameters\n" << std::endl;
		return 0;
	}
	char* f = argv[1];
	N = atoi(argv[2]);
	Nz = atoi(argv[3]);


	if ((N < Nz) || (N <= 0) || (Nz <= 0) || (N == 1))
	{
		std::cout << "Incorrect arguments of main\n" << std::endl;
		return 0;
	}

	int size_nonzero = N * Nz;

	crsMatrix A, B;

	A = Generate_CRS(N, Nz,A);	B = Generate_CRS(N, Nz,B);

	freopen_s(&matr, f, "wb", stdout);

	fwrite(&N, sizeof(N), 1, stdout);
	fwrite(&Nz, sizeof(Nz), 1, stdout);
	
	fwrite(A.Value, sizeof(*A.Value), size_nonzero, stdout);
	fwrite(A.Col, sizeof(*A.Col), size_nonzero, stdout);
	fwrite(A.Row_Index, sizeof(*A.Row_Index), N + 1, stdout);
	
	fwrite(B.Value, sizeof(*B.Value), size_nonzero, stdout);
	fwrite(B.Col, sizeof(*B.Col), size_nonzero, stdout);
	fwrite(B.Row_Index, sizeof(*B.Row_Index), N + 1, stdout);
		fclose(matr);

	FreeMatr(A);
	FreeMatr(B);

	return 0;
}