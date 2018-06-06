#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include "iostream"
#include <ctime>
#include <algorithm>
#include "tbb/task_scheduler_init.h"
#include "tbb/concurrent_vector.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/tick_count.h"

using namespace std;
using std::vector;
using namespace tbb;
using Iterator = vector<int>::iterator;

struct VecPart
{
	int start;
	int partsize;
	VecPart(int start_, int partsize_) : start(start_), partsize(partsize_) {}
};

concurrent_vector<VecPart> parts;


int get_dig(int num, int dig_num)
{
	num = abs(num);
	for (int i = 0; i < (dig_num - 1); ++i)
		num /= 10;

	return num % 10;
}

void RadixSort(vector<int>& ForSort)
{
	int num_of_numbers = static_cast<int>(ForSort.size());
	vector<int> buffer(num_of_numbers);
	vector<int> pos_ForSort(10);
	int digit = 10;
	int digit_val = 10;
	int count, tmp;

	for (int i = 0; i < digit; ++i)
	{
		for (int j = 0; j < digit_val; ++j)
			pos_ForSort[j] = 0;

		for (int j = 0; j < num_of_numbers; ++j)
			++pos_ForSort[get_dig(ForSort[j], i)];

		count = 0;

		for (int j = 0; j < digit_val; ++j)
		{
			tmp = pos_ForSort[j];
			pos_ForSort[j] = count;
			count += tmp;
		}

		for (int j = 0; j < num_of_numbers; ++j)
			buffer[pos_ForSort[get_dig(ForSort[j], i)]++] = ForSort[j];

		ForSort = buffer;
	}
	ForSort.clear();
	copy_if(buffer.rbegin(), buffer.rend(), back_inserter(ForSort), [](int i)
	{ 
		return i < 0; 
	});
	copy_if(buffer.begin(), buffer.end(), back_inserter(ForSort), [](int i)
	{
		return i >= 0; 
	});
}


void RadixSort_Part(vector<int>& vec, const int& begin, const int& end, vector<int>& BUF)
{
	VecPart vp(begin, end - begin);
	parts.push_back(vp);

	if (end - begin == 1) return;

	int max_dig_count = 10;
	int max_dig = 10;
	int num_of_numbers = static_cast<int>(vec.size());
	int count, tmp;
	vector<int> pos_vec(10);

	for (int i = 0; i < max_dig_count; ++i)
	{
		for (int j = 0; j < max_dig; ++j)
			pos_vec[j] = 0;

		for (int j = begin; j < end; ++j)
			++pos_vec[get_dig(vec[j], i)];

		count = 0;
		for (int j = 0; j < max_dig; ++j)
		{
			tmp = pos_vec[j];
			pos_vec[j] = count;
			count += tmp;
		}

		for (int j = begin; j < end; ++j)
			BUF[begin + pos_vec[get_dig(vec[j], i)]++] = vec[j];

		for (int j = begin; j < end; ++j)
			vec[j] = BUF[j];
	}

	int nneg = 0;

	for (int i = end - 1, j = begin; i >= begin; --i)
		if (BUF[i] < 0)
		{
			++nneg;
			vec[j++] = BUF[i];
		}

	for (int i = begin, j = begin + nneg; i < end; ++i)
		if (BUF[i] >= 0)
			vec[j++] = BUF[i];
}

void Merge(vector<int>& vec, const int& merge_num, const int& step, vector<int>& BUF)
{
	int current_part_num = merge_num * step * 2;

	if (current_part_num + step >= parts.size())
		return;

	Iterator start1 = vec.begin() + parts[current_part_num].start;
	Iterator finish1 = start1 + parts[current_part_num].partsize;
	Iterator start2 = finish1;
	Iterator finish2 = start2 + parts[current_part_num + step].partsize;
	Iterator buffer_start = BUF.begin() + parts[current_part_num].start;
	int count = static_cast<int>(std::distance(start1, finish2));

	while (start1 != finish1 && start2 != finish2)
		*buffer_start++ = *start1 <= *start2 ? *start1++ : *start2++;

	while (start1 != finish1)
		*buffer_start++ = *start1++;

	while (start2 != finish2)
		*buffer_start++ = *start2++;

	buffer_start = BUF.begin() + parts[current_part_num].start;
	start1 = vec.begin() + parts[current_part_num].start;

	for (int i = 0; i < count; ++i)
		*start1++ = *buffer_start++;

	parts[current_part_num].partsize += parts[current_part_num + step].partsize;
}

void RadixSort_TBB(vector<int>& vec, int nthreads = 1)
{
	task_scheduler_init init(task_scheduler_init::deferred);

	int num_of_threads;
	if (nthreads > 1)
	{
		num_of_threads = nthreads;
		init.initialize(nthreads);
	}
	else
	{
		num_of_threads = task_scheduler_init::default_num_threads();
		init.initialize(num_of_threads);
	}

	int num_of_numbers = static_cast<int>(vec.size());
	int grainsize1 = num_of_numbers / num_of_threads;

	if (grainsize1 == 0)
	{
		RadixSort(vec);
		return;
	}

	vector<int> buffer(num_of_numbers);

	parallel_for(blocked_range<int>(0, num_of_numbers, grainsize1),
		[&vec, &buffer](const blocked_range<int>& r)
	{
		RadixSort_Part(vec, r.begin(), r.end(), buffer);
	}, auto_partitioner()
		);

	sort(parts.begin(), parts.end(), [](const VecPart& vp1, const VecPart& vp2) 
	{
		return vp1.start < vp2.start; 
	});

	int parts_count = static_cast<int>(parts.size());

	for (int step = 1; step < parts_count; step *= 2)
	{
		parallel_for(blocked_range<int>(0, parts_count / 2),
			[&vec, &buffer, &num_of_numbers, &step](const blocked_range<int>& r)
		{
			int grainsize = r.grainsize();
			Merge(vec, r.begin(), step, buffer);
		}, auto_partitioner()
			);
	}

	if (num_of_threads >= 1)
		init.terminate();
}

int main(int argc, char * argv[])
{
	int size;

	std::vector<int> ForSort;

	char* input = "ExArr.in";
	char* output = "SortedExArr.out";
	int thread = 2;

	if (argc > 3)
	{
		thread = atoi(argv[1]);
		input = argv[2];
		output = argv[3];
	}

	freopen(input, "rb", stdin);
	fread(&size, sizeof(size), 1, stdin);
	ForSort.resize(size);
	fread(ForSort.data(), sizeof(int), size, stdin);
	fclose(stdin);

	double startTime = 0;
	double endTime = 0;
	double timeOfParallel = 0;

	startTime = clock();
	RadixSort_TBB(ForSort, thread);
	endTime = clock();

	timeOfParallel = endTime - startTime;
	cout << "---" << timeOfParallel << "---" << endl;

	freopen(output, "wb", stdout);
	fwrite(&size, sizeof(size), 1, stdout);
	fwrite(ForSort.data(), sizeof(int), size, stdout);

	fclose(stdout);

	return 0;
}
