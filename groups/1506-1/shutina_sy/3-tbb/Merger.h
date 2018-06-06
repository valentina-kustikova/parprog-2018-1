#pragma once

#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
using namespace tbb;

enum elemType {
	EVEN,
	ODD
};

class Merger : public task
{
private:
	elemType type;
	double* mas;
	int size1;
	int size2;
public:
	Merger(elemType p_type, double* p_mas, int p_size1, int p_size2) : type(p_type), mas(p_mas), size1(p_size1), size2(p_size2) {}
	task* execute() {
		double* tmp = new double[size1 + size2];
		int a, b, i;
		double* mas2;
		if (type == ODD)
		{
			for (int i = 1; i < size1; i += 2) {
				tmp[i] = mas[i];
			}

			mas2 = mas + size1;

			a = 1, b = 1, i = 1;
		}
		else {
			for (int i = 0; i < size1; i += 2) {
				tmp[i] = mas[i];
			}

			mas2 = mas + size1;

			a = 0, b = 0, i = 0;
		}

		while ((a < size1) && (b < size2)) {
			if (tmp[a] <= mas2[b]) {
				mas[i] = tmp[a];
				a += 2;
			}
			else {
				mas[i] = mas2[b];
				b += 2;
			}
			i += 2;
		}
		if (a >= size1)
			while (b < size2) {
				mas[i] = mas2[b];
				b += 2;
				i += 2;
			}
		else
			while (a < size1) {
				mas[i] = tmp[a];
				a += 2;
				i += 2;
			}
		return NULL;
	}
};

