#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>

#include <omp.h>
using namespace std;
//
////////////////////////////////////////////////////////////////////////////////////////////
/*
// Checker :
AC = Accepted =
WA = Wrong Answer =
PE = Presentation Error =
//  checker
NO = No verdict =
CE = Compilation Error =
ML = Memory Limit Exceeded =
TL = Time Limit Exceeded =
RE = Runtime Error =
IL = Idle Limit Exceeded =
DE = Deadly Error =
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
		if (read) bur = fopen("result.txt", "r");
		else bur = fopen("result.txt", "w");
	}
	~result()
	{
		fclose(bur);
	}
	void write_type(ext_cls t)
	{
		fwrite(&t, sizeof(t), 1, bur);
	}
	//  verdict
	void write_verdict(verdict v)
	{
		write_type(ext_cls::VERDICT);
		fwrite(&v, sizeof(v), 1, bur);
	}
	//  checker'a
	//
	//
	void write_message(string str)
	{
		write_type(ext_cls::MESSAGE);
		int l = str.size();
		fwrite(&l, sizeof(l), 1, bur);
		fwrite(&str[0], sizeof(str[0]), l, bur);
	}
	//
	//  before_code
	// x  100  = 10 ^ (-7)
	void write_time(long long x)
	{
		write_type(ext_cls::TIME);
		fwrite(&x, sizeof(x), 1, bur);
	}
	//
	{
		write_type(ext_cls::MEMORY);
		fwrite(&x, sizeof(x), 1, bur);
	}
} checker_result;
////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;

using ulong = unsigned long;
int main(int argc, char** argv)
{
	//
	int n = 20;
	string nameFile;
	bool result = true;
	bool resAll = true;

	if (argc == 2)
		n = (int)(stod(argv[1])); //= stoi(argv[1]);

	for (int i = 1; i <= n; i++)
	{
		//
		result = true;
		nameFile = to_string((long long)i);
		FILE* fTestMassRes = fopen(("./tests/" + nameFile + ".testres").c_str(), "rb");
		FILE* fResMass = fopen(("./tests/" + nameFile + ".ans").c_str(), "rb");

		int sizeMassTest, sizeMassRes;
		fread(&sizeMassTest, sizeof(sizeMassTest), 1, fTestMassRes);
		fread(&sizeMassRes, sizeof(sizeMassRes), 1, fResMass);

		double timeWorkTest, timeWorkRes;
		fread(&timeWorkTest, sizeof(timeWorkTest), 1, fTestMassRes);
		fread(&timeWorkRes, sizeof(timeWorkRes), 1, fResMass);

		if (sizeMassTest != sizeMassRes)
			result = false;
		else
		{
			int *dataTest = new int[sizeMassTest];
			int *dataRes = new int[sizeMassRes];

			for (int i = 0; i < sizeMassRes; i++)
			{
				fread(&dataTest[i], sizeof(dataTest[i]), 1, fTestMassRes);
				fread(&dataRes[i], sizeof(dataRes[i]), 1, fResMass);
				if (dataTest[i] != dataRes[i])
				{
					result = false;
					resAll = false;
					break;
				}
			}

			delete[]dataTest;
			delete[]dataRes;
		}

		if (result)
		{
			checker_result.write_message("AC. Output is correct.");
			checker_result.write_verdict(verdict::AC);
		}
		else
		{
			checker_result.write_message("WA. Output is not correct.");
			checker_result.write_verdict(verdict::WA);
		}

		checker_result.write_time(timeWorkRes * 1e7);
		fclose(fTestMassRes);
		fclose(fResMass);
	}
	cout << resAll << endl;
	return 0;
}
