#include <iostream>
#include <ctime>
#include <random> 
#include <chrono>


struct crsMatrix
{
	int Size; // ������ �������
	int Size_Z; // ���-�� ���������

	double* Value; // ������ ��������
	int* Col; // ������ ������� ��������
	int* Row_Index;	// ������ �������� �����
};

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

crsMatrix Transpose(crsMatrix B)
{
	int Size = B.Size;
	int NZSize = B.Size_Z;
	crsMatrix B_T;
	InitMatr(Size, NZSize, B_T);

	for (int i = 0; i < Size + 1; i++)
		B_T.Row_Index[i] = 0;

    // RowIndex[i] = RowIndex[i - 1] + ����������� ��������� � ������� i - 1�.
	for (int i = 0; i < NZSize; i++)
		B_T.Row_Index[B.Col[i] + 1]++;

	int S = 0;
	int tmp;

	for (int i = 1; i <= Size; i++)
	{
		tmp = B_T.Row_Index[i];
		B_T.Row_Index[i] = S;
		S += tmp;
	}

	for (int i = 0; i < Size; i++)
	{
		int start = B.Row_Index[i];
		int end = B.Row_Index[i + 1];
		int Column = i;								// i-�� ������ � B ���� i-�� ������� � B_T
		double Value;
		int Rcolumn, Iindex;

		for (int j = start; j < end; j++)
		{
			Value = B.Value[j];
			Rcolumn = B.Col[j];					    // ������� � B
			Iindex = B_T.Row_Index[Rcolumn + 1];	// ������ ������ B_T, ������� ����� ��������� Value
			B_T.Value[Iindex] = Value;
			B_T.Col[Iindex] = Column;
			B_T.Row_Index[Rcolumn + 1]++;			// � ����� B_T.rows[j] ����� ��������� ������ ������ j-�� ������ 
		}
	}

	return B_T;
}

crsMatrix Multiplication(crsMatrix A, crsMatrix B, double& time)
{
	std::chrono::time_point<std::chrono::system_clock> start, end;

	if (A.Size != B.Size)
		return crsMatrix();

	int Size = A.Size;

	std::vector<int> columns;
	std::vector<double> values;
	std::vector<int> rIndex;

	start = std::chrono::system_clock::now();

	int *temp = new int[Size];
	rIndex.push_back(0);

	int NZ = 0;

	for (int i = 0; i < Size; i++)
	{
		
		for (int i = 0; i < Size; i++)
			temp[i] = -1;

		int ind1 = A.Row_Index[i], ind2 = A.Row_Index[i + 1];
			
		for (int j = ind1; j < ind2; j++)
		{
			int col = A.Col[j];
			temp[col] = j;
		}

		// �������� ������ ������ i ������� A
		// ������ ���������� �������� �� �� ������ �� ����� ������� BT
		for (int j = 0; j < Size; j++)
		{
			double sum = 0;
		
			int ind3 = B.Row_Index[j], ind4 = B.Row_Index[j + 1];
			for (int k = ind3; k < ind4; k++)
			{
				int Bcol = B.Col[k];
				int Aind = temp[Bcol];
				if (Aind != -1)
					sum += A.Value[Aind] * B.Value[k];
			}
			if (fabs(sum) != 0)
			{
				columns.push_back(j);
				values.push_back(sum);	
				NZ++;
			}
		}

		rIndex.push_back(NZ);
	}
	
	crsMatrix C;
	InitMatr(Size, NZ, C);

	for (int j = 0; j < NZ; j++)
	{
		C.Col[j] = columns[j];
		C.Value[j] = values[j];
	}

	for (int i = 0; i <= Size; i++)
		C.Row_Index[i] = rIndex[i];
	
	end = std::chrono::system_clock::now();

	time = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000;
	std::cout << "Time: " << time << std::endl;

	delete[] temp;
	return C;
}


void Sequence(char* file_in, char* file_out)
{
	FILE* matr_in, *matr_res;
	crsMatrix A, B, BT, C;
	double time = -1;

	errno_t f = freopen_s(&matr_in, file_in, "rb", stdin);
	if (f != 0)
	{
		std::cout << "Could not open file \n" << std::endl;
		return;
	}

	int N = 2, Nz = 1;

	fread(&N, sizeof(N), 1, stdin);
	fread(&Nz, sizeof(Nz), 1, stdin);
	int size_nonzero = N * Nz;

	InitMatr(N, size_nonzero, A);
	InitMatr(N, size_nonzero, B);

	fread(A.Value, sizeof(*A.Value), size_nonzero, stdin);
	fread(A.Col, sizeof(*A.Col), size_nonzero, stdin);
	fread(A.Row_Index, sizeof(*A.Row_Index), N + 1, stdin);

	fread(B.Value, sizeof(*B.Value), size_nonzero, stdin);
	fread(B.Col, sizeof(*B.Col), size_nonzero, stdin);
	fread(B.Row_Index, sizeof(*B.Row_Index), N + 1, stdin);

	fclose(matr_in);

	BT = Transpose(B);
	C = Multiplication(A, BT,time);
	
	// ����� ��������� ���������� ������ ��� �� ���������
	int size_nonzero_C = C.Size_Z;

	
	freopen_s(&matr_res, file_out, "wb", stdout);

	fwrite(&N, sizeof(N), 1, stdout);
	fwrite(&size_nonzero_C, sizeof(size_nonzero_C), 1, stdout);
	fwrite(C.Value, sizeof(*C.Value), size_nonzero_C, stdout);
	fwrite(C.Col, sizeof(*C.Col), size_nonzero_C, stdout);
	fwrite(C.Row_Index, sizeof(*C.Row_Index), N + 1, stdout);	fwrite(&time, sizeof(time), 1, stdout);

	fclose(matr_res);

	FreeMatr(A);
	FreeMatr(B);
	FreeMatr(BT);
	FreeMatr(C);
}

int main(int argc, char *argv[])
{
	char* f1;
	char* f2;

	if (argc != 3)
	{
		std::cout << "Invalid input parameters\n" << std::endl;
		std::cout << "The default values are used:\n  matr.in -> matr.ans" << std::endl;

		f1 = "matr.in";
		f2 = "matr.ans";

	}
	else
	{
		f1 = argv[1];
		f2 = argv[2];
	}

	Sequence(f1,f2);

	return 0;
}

