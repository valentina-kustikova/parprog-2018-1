#include "solution3.h"
#include "solution.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include "tbb/parallel_for_each.h"
#include "tbb/task_scheduler_init.h"

using namespace std;

struct comparator
{
	int id1;
	int id2;
	int realLvl;
	comparator(int id1, int id2, int realLvl)
	{
		this->id1 = id1;
		this->id2 = id2;
		this->realLvl = realLvl;
	}
};

template <typename Tadd> void mergeMass(Tadd&& add, int iStart, int iFinish, int tmpRS, int realSize)
{
	int count = tmpRS * 2;
	if ((iFinish - iStart) > count)
	{
		mergeMass(add, iStart, iFinish, count, realSize);
		mergeMass(add, iStart + tmpRS, iFinish, count, realSize);
		for (int i = iStart + tmpRS; i + tmpRS < min(iFinish, realSize); i += count)
			add(i, i + tmpRS);
	}
	else
	{
		if (iStart + tmpRS < realSize)
			add(iStart, iStart + tmpRS);
	}
}

template <typename Tadd> void sortMass(Tadd&& add, int iStart, int iFinish, int realSize)
{
	if ((iFinish - iStart) > 1)
	{
		int halfSizeMass = iStart + (iFinish - iStart) / 2;
		sortMass(add, iStart, halfSizeMass, realSize);
		sortMass(add, halfSizeMass, iFinish, realSize);
		mergeMass(add, iStart, iFinish, 1, realSize);
	}
}

template <typename Tadd> void generateComparators(Tadd&& add, int realSize)
{
	int deep = 0;
	int temp = realSize;
	while (temp > 1)
	{
		temp /= 2;
		deep++;
	}

	int size = (realSize == (1 << deep)) ? realSize : (2 << deep);
	sortMass(add, 0, size, realSize);
}

template<typename Tmass> void sort(vector<Tmass>& vMass, int numThreads)
{
	tbb::task_scheduler_init init(numThreads);
	vector<int> maxLvl(numThreads, -1);
	vector<vector<comparator>> levels;
	generateComparators([&](int id1, int id2)
	{
		int lk1 = maxLvl[id1];
		int lk2 = maxLvl[id2];

		int realLvl = max(lk1, lk2) + 1;
		maxLvl[id1] = max(lk1, realLvl);
		maxLvl[id2] = max(lk2, realLvl);

		if (levels.size() <= realLvl)
			levels.emplace_back();
		levels[realLvl].emplace_back(id1, id2, realLvl);
		levels[realLvl].emplace_back(id2, id1, realLvl);

	}, numThreads);

	size_t division = vMass.size() / numThreads;
	size_t remainder = vMass.size() % numThreads;

	if (remainder > 0)
	{
		division++;
		remainder = numThreads - remainder;
	}

	Tmass* data = vMass.data();

	vector<vector<Tmass>> vTempMemory(numThreads);
	for (int i = 0; i < numThreads; i++)
		vTempMemory[i].resize(division);

	vector<int> index(numThreads);
	for (int i = 0; i < numThreads; i++)
		index[i] = i;

	tbb::parallel_for_each(index.begin(), index.end(), [&](int id)
	{
		size_t iStart = id * division;
		size_t iFinish = (id + 1) * division;
		if (id == (numThreads - 1))
			iFinish -= remainder;

		sort(data + iStart, data + iFinish);
	});


	auto step = [&](const comparator& comp)
	{
		size_t id = comp.id2;
		size_t iStart = id * division;
		size_t iFinish = (id + 1) * division;
		if (id == (numThreads - 1))
			iFinish -= remainder;
		Tmass* vTempMemory2 = vTempMemory[id].data();
		size_t length = 0;
		size_t iS = (comp.id1 < comp.id2) ? comp.id1 : comp.id2;
		size_t iF = (comp.id1 > comp.id2) ? comp.id1 : comp.id2;
		if (id == iS)
		{
			length = (iF == (numThreads - 1)) ? (division - remainder) : division;
			size_t iRes = iStart, iCurr = division * iF, iTmp = 0;
			for (; (iTmp < iFinish - iStart) && (iCurr < (length + division * iF)); iTmp++)
			{
				Tmass result = data[iRes];
				Tmass current = data[iCurr];
				if (result < current)
				{
					vTempMemory2[iTmp] = result;
					iRes++;
				}
				else
				{
					vTempMemory2[iTmp] = current;
					iCurr++;
				}
			}

			for (; iTmp < iFinish - iStart; iTmp++, iRes++)
				vTempMemory2[iTmp] = data[iRes];
		}
		else if (id == iF)
		{
			length = division;
			size_t iRes = iFinish - 1, iCurr = division * iS + length, iTmp = iFinish - iStart;
			for (; (iTmp >= 1); iTmp--)
			{
				Tmass result = data[iRes];
				Tmass current = data[iCurr - 1];
				if (result > current)
				{
					vTempMemory2[iTmp - 1] = result;
					iRes--;
				}
				else
				{
					vTempMemory2[iTmp - 1] = current;
					iCurr--;
				}
			}
		}
	};

	auto step2 = [&](const comparator& comp)
	{
		size_t id = comp.id1;
		size_t iStart = id * division;
		size_t iFinish = (id + 1) * division;
		if (id == (numThreads - 1))
			iFinish -= remainder;
		Tmass* vTempMemory2 = vTempMemory[id].data();

		for (size_t i = 0; i < iFinish - iStart; i++)
			data[iStart + i] = vTempMemory2[i];
	};

	for (auto& x : levels)
	{
		tbb::parallel_for_each(x.begin(), x.end(), step);
		tbb::parallel_for_each(x.begin(), x.end(), step2);
	}
}

void sortHoarwithBatcher3(int* mass, int size, int numThreads)
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
