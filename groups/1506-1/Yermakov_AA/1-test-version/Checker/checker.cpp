#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>

using namespace std;

enum verdict { NO = 1, AC, WA, CE, ML, TL, RE, IL, PE, DE };

class result {
private:
	FILE * bur;

public:
	enum ext_cls { NO = 1, VERDICT, MESSAGE, TIME, MEMORY };

	result(bool read = false) {
		if (read)
			bur = fopen("result.txt", "r");
		else
			bur = fopen("result.txt", "w");
	}

	~result() {
		fclose(bur);
	}

	void write_type(ext_cls t) {
		fwrite(&t, sizeof(t), 1, bur);
	}

	void write_verdict(verdict v) {
		write_type(ext_cls::VERDICT);
		fwrite(&v, sizeof(v), 1, bur);
	}

	void write_message(string str) {
		write_type(ext_cls::MESSAGE);
		int l = str.size();
		fwrite(&l, sizeof(l), 1, bur);
		fwrite(&str[0], sizeof(str[0]), l, bur);
	}

	void write_time(long long x) {
		write_type(ext_cls::TIME);
		fwrite(&x, sizeof(x), 1, bur);
	}

	void write_memory(unsigned long long x) {
		write_type(ext_cls::MEMORY);
		fwrite(&x, sizeof(x), 1, bur);
	}
} checker_result;

int main(int argc, char * argv[])
{
	if (argc != 2) {
		std::cout << "Error! You should input 'checker file.txt(output file)'" << std::endl;
		return 1;
	}

	FILE * buo = fopen(argv[1], "rb");

	int res_size = 0;
	double res_time;

	fread(&res_time, sizeof(res_time), 1, buo);
	fread(&res_size, sizeof(res_size), 1, buo);

	bool error = false;
	double prev = 0, cur = 0;

	fread(&prev, sizeof(prev), 1, buo);

	for (int i = 1; i < res_size; ++i) {
		fread(&cur, sizeof(cur), 1, buo);
		if (cur < prev) {
			error = true;
			break;
		}
		prev = cur;
	}
	if (error == false) {
		checker_result.write_message("AC. Sorting was correct.");
		checker_result.write_verdict(verdict::AC);
	}
	else {
		checker_result.write_message("WA. Sorting was incorrect.");
		checker_result.write_verdict(verdict::WA);
	}
	checker_result.write_time(static_cast<long long>(res_time * 1e7));

	fclose(buo);
	return 0;
}