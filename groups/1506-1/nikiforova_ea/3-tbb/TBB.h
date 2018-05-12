#pragma once
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"
#include <omp.h> 
#include "sol.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <ctime>
using namespace tbb;
using namespace cv;
using namespace std;

class HistCalculator
{
	Mat& pic;
	int* hist;
public:
	HistCalculator(Mat& _pic, int* _hist): pic(_pic), hist(_hist) {}
	void operator() (const blocked_range2d<int>& r) const
	{
		for (int i = r.rows().begin(); i !=r.rows().end(); i++)
		{
			for (int j = r.cols().begin(); j  !=r.cols().end(); j++)
			{
				hist[pic.at<uchar>(i, j)]++;
			}
		}
	}
};
class HistSetting
{
	Mat& pic;
	Mat& res;
	int min;
	int max;
public:
	HistSetting(Mat& _pic, Mat& _res, int _min, int _max): pic(_pic), res(_res), min(_min), max(_max) {}
	void operator() (blocked_range2d<int>& r) const
	{
		for (int i = r.rows().begin(); i <r.rows().end(); i++)
		{
			for (int j = r.cols().begin(); j < r.cols().end(); j++)
			{
				res.at<uchar>(i, j) = round(255 * (pic.at<uchar>(i, j) - min) / (max - min));
			}
		}
	}
};


void linearsTBB(Mat source, Mat result, int CountOfProc)
{

    //unsigned int start_time = clock();
	task_scheduler_init init(CountOfProc);
	for (int i = 0; i < result.rows; ++i)
		for (int j = 0; j < result.cols; ++j)
			result.at<uchar>(i, j) = source.at<uchar>(i, j);
	int *hist;
	hist = new int[256];
	memset(hist, 0, 256 * sizeof(int));
	int min = INT_MAX;
	int max = INT_MIN;

	parallel_for(blocked_range2d<int>(0,source.rows,0,source.cols),HistCalculator(source,hist));

	painthist(source);
	int i = 0;
	while (i < 256 && hist[i] == 0) i++;
	min = i;
	i = 255;
	while (i > 0 && hist[i] == 0) i--;
	max = i;

	parallel_for(blocked_range2d<int>(0, source.rows, 0, source.cols), HistSetting(source,result, min,max));
	//unsigned int end_time = clock(); // конечное время
	//unsigned int search_time = end_time - start_time; // искомое время
	//cout << "TBB ----------> "<< search_time << endl;
	painthist(result);
}


