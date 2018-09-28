#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <string>

#include <omp.h>
using namespace std;

void sortHoarwithBatcher(int* a, int size);

int main(int argc, char* argv[])
{

	int sizeMass = 256;
	string nameFile;

	if (argc == 2)
	{
		nameFile = argv[1];
		//n = (int)(stod(argv[1]));// = stoi(argv[1]);
	}

	//for (int i = 1; i <= n; i++)
	//{
		//nameFile = to_string((long long)i);
		FILE* fTestMass = fopen((nameFile).c_str(), "rb");
		FILE* fResMass = fopen((nameFile + ".ans").c_str(), "wb");

		//
		fread(&sizeMass, sizeof(sizeMass), 1, fTestMass);
		int *data = new int[sizeMass];
		fread(data, sizeof(*data), sizeMass, fTestMass);

		auto timeWork = omp_get_wtime();
		sortHoarwithBatcher(data, sizeMass);
		timeWork = omp_get_wtime() - timeWork;

		//
		fwrite(&sizeMass, sizeof(sizeMass), 1, fResMass);
		fwrite(&timeWork, sizeof(timeWork), 1, fResMass);
		fwrite(data, sizeof(*data), sizeMass, fResMass);

		delete[]data;
		fclose(fTestMass);
		fclose(fResMass);
	//}
	return 0;

}
