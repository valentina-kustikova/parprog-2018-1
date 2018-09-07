#include <string>
#include <time.h>
#include <fstream>
#include <random>
#include <omp.h>

#include <algorithm>

using namespace std;

int main(int argc, char* argv[])
{
	//
	int n = 20;
	int sizeMass = 256;
	int maxVal = 1e3;
	string nameFile;

	if(argc >= 3)
	{
		n =(int)( stod(argv[1]));
		//n=stoi(argv[1]);
		sizeMass = (int)(stod(argv[2]));//=stoi(argv[2]);
	}
	if(argc == 4)
		maxVal = (int)(stod(argv[3]));// = stoi(argv[3]);

	//
	default_random_engine generator(time(NULL));
	uniform_int_distribution <int> distribution(0, maxVal);

	int *massTest=new int[sizeMass];
	for(int i=1;i<=n;i++)
	{
		//
		nameFile=to_string((long long)i);
		FILE* fTestMass = fopen(("./tests/"+nameFile).c_str(), "wb");
		FILE* fTestMassRes = fopen(("./tests/"+nameFile + ".testres").c_str(), "wb");
		//
		fwrite(&sizeMass, sizeof (sizeMass), 1, fTestMass);
		fwrite(&sizeMass, sizeof (sizeMass), 1, fTestMassRes);

		//
		for(int j=0;j<sizeMass;j++)
			massTest[j] = distribution(generator);
		fwrite(massTest, sizeof (sizeMass), sizeMass, fTestMass);
		//
		auto timeWork = omp_get_wtime();
		sort(massTest, massTest + sizeMass);
		timeWork = omp_get_wtime() - timeWork;
		//
		fwrite(&timeWork, sizeof(timeWork), 1, fTestMassRes);
		fwrite(massTest, sizeof (sizeMass), sizeMass, fTestMassRes);

		fclose(fTestMass);
		fclose(fTestMassRes);
	}

	delete[] massTest;
	return 0;
}
