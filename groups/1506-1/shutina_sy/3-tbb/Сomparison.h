#pragma once

#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
using namespace tbb;

class Ñomparison
{
private:
	double* mas;
public:
	Ñomparison(double* p_mas) : mas(p_mas) {}

	void operator() (const blocked_range<int>& r) const {
		int begin = r.begin();
		int end = r.end();

		for (int i = begin; i < end; i++) {
			if (mas[2 * i] < mas[2 * i - 1]) {
				double tmp = mas[2 * i - 1];
				mas[2 * i - 1] = mas[2 * i];
				mas[2 * i] = tmp;
			}
		}
	}
};

