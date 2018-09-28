#include "solution2.h"
#include "solution.h"
#include <vector>
#include <algorithm>
#include "omp.h"

using namespace std;

struct comparator
{
	int id1;
	int id2;
	omp_lock_t start, finish, sync;
	comparator(int id1, int id2)
	{
		this->id1 = id1;
		this->id2 = id2;
	}
};

void mergeMass(vector<comparator>& vMass, int iStart, int iFinish, int tmpRS, int realSize)
{
	int count = tmpRS * 2;
	if ((iFinish - iStart) > count)
	{
		mergeMass(vMass, iStart, iFinish, count, realSize);
		mergeMass(vMass, iStart + tmpRS, iFinish, count, realSize);
		for (int i = iStart + tmpRS; i + tmpRS < min(iFinish, realSize); i += count)
			vMass.push_back({ i, i + tmpRS });
	}
	else
	{
		if (iStart + tmpRS < realSize)
			vMass.push_back({ iStart, iStart + tmpRS });
	}
}

void sortMass(vector<comparator>& vMass, int iStart, int iFinish, int realSize)
{
	if ((iFinish - iStart) > 1)
	{
		int halfSizeMass = iStart + (iFinish - iStart) / 2;
		sortMass(vMass, iStart, halfSizeMass, realSize);
		sortMass(vMass, halfSizeMass, iFinish, realSize);
		mergeMass(vMass, iStart, iFinish, 1, realSize);
	}
}
vector<comparator> generateComparators(int realSize)
{
	vector<comparator> result;
	int deep = 0;
	int temp = realSize;
	while (temp > 1)
	{
		temp /= 2;
		deep++;
	}
	int size = (realSize == (1 << deep)) ? realSize : (2 << deep);
	result.reserve(deep * (deep - 1) * (size / 4) + size - 1);
	sortMass(result, 0, size, realSize);
	return result;
}

template<typename Tmass> void sort(vector<Tmass>& vMass, int numThreads)
{
	size_t division = vMass.size() / numThreads;
	size_t remainder = vMass.size() % numThreads;
	if (remainder > 0)
	{
		division++;
		remainder = numThreads - remainder;
	}
	Tmass* data = vMass.data();
	vector<comparator> vComparators = generateComparators(numThreads);
#pragma omp parallel num_threads(numThreads) shared(data, numThreads, vComparators)
	{
		size_t id = omp_get_thread_num();
		for (auto& itemComparator : vComparators)
		{
			if (id == itemComparator.id1)
			{
				omp_init_lock(&itemComparator.start);
				omp_init_lock(&itemComparator.sync);
				omp_set_lock(&itemComparator.start);
				omp_set_lock(&itemComparator.sync);
			}
			else if (id == itemComparator.id2)
			{
				omp_init_lock(&itemComparator.finish);
				omp_set_lock(&itemComparator.finish);
			}
		}

#pragma omp barrier
		size_t iStart = id * division;
		size_t iFinish = (id + 1) * division;
		if (id == (numThreads - 1))
			iFinish -= remainder;
		vector<Tmass> vTempMemory(iFinish - iStart);
		sort(data + iStart, data + iFinish);

		size_t length = 0;
		for (auto& itemComparator : vComparators)
		{
			size_t start = itemComparator.id1;
			size_t finish = itemComparator.id2;
			if (id == start)
			{
				omp_unset_lock(&itemComparator.start);
				omp_set_lock(&itemComparator.finish);
				omp_unset_lock(&itemComparator.sync);
				length = (finish == (numThreads - 1)) ? (division - remainder) : division;
				size_t iRes = iStart, iCurr = division * finish, iTmp = 0;
				for (; (iTmp < iFinish - iStart) && (iCurr < (length + division * finish)); iTmp++)
				{
					Tmass result = data[iRes];
					Tmass current = data[iCurr];
					if (result < current)
					{
						vTempMemory[iTmp] = result;
						iRes++;
					}
					else
					{
						vTempMemory[iTmp] = current;
						iCurr++;
					}
				}

				for (; iTmp < iFinish - iStart; iTmp++, iRes++)
					vTempMemory[iTmp] = data[iRes];

				omp_set_lock(&itemComparator.start);
				omp_unset_lock(&itemComparator.finish);

				for (size_t i = 0; i < iFinish - iStart; i++)
					data[iStart + i] = vTempMemory[i];

			}
			else if (id == finish)
			{
				omp_set_lock(&itemComparator.start);
				omp_unset_lock(&itemComparator.finish);
				omp_set_lock(&itemComparator.sync);
				length = division;
				size_t iRes = iFinish - 1, iCurr = division * start + length, iTmp = iFinish - iStart;
				for (; (iTmp >= 1); iTmp--)
				{
					Tmass result = data[iRes];
					Tmass current = data[iCurr - 1];
					if (result > current)
					{
						vTempMemory[iTmp - 1] = result;
						iRes--;
					}
					else
					{
						vTempMemory[iTmp - 1] = current;
						iCurr--;
					}
				}
				omp_unset_lock(&itemComparator.start);
				omp_set_lock(&itemComparator.finish);
				for (size_t i = 0; i < iFinish - iStart; i++)
					data[iStart + i] = vTempMemory[i];
			}
		}
	}

	for (auto& itemComparator : vComparators)
	{
		omp_destroy_lock(&itemComparator.start);
		omp_destroy_lock(&itemComparator.finish);
		omp_destroy_lock(&itemComparator.sync);
	}
}

void sortHoarwithBatcher2(int* mass, int size, int numThreads)
{
	if (numThreads <= 1)
	{
		sortHoarwithBatcher(mass, size);
		return;
	}
	vector<int> tmpVectorMass;
	for (int i = 0; i < size; i++)
		tmpVectorMass.push_back(mass[i]);
	sort(tmpVectorMass, numThreads);
	for (int i = 0; i < size; i++)
		mass[i] = tmpVectorMass[i];
}
