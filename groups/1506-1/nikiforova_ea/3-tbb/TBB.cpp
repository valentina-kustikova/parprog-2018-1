#pragma once
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"
#include <omp.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
using namespace tbb;
using namespace cv;
using namespace std;
bool typer(Mat pic, string buf)
{
	ofstream file;
	file.open(buf, ofstream::out | ios::binary);
	if (file.is_open())
	{
		if (pic.empty())
		{
			int s = 0;
			file.write((char*)(&s), sizeof(int));
			return true;
		}
		int type = pic.type();
		file.write((char*)(&pic.rows), sizeof(int));
		file.write((char*)(&pic.cols), sizeof(int));
		file.write((char*)(&type), sizeof(int));
		file.write((char*)(pic.data), pic.elemSize() * pic.total());
		file.close();
		return true;
	}
	else return false;
}
void painthist(Mat source)
{
	int hist_size = 64;
	float range_0[] = { 0,256 };
	float* ranges[] = { range_0 };
	CvHistogram *histogr;
	IplImage* image2 = cvCreateImage(cvSize(source.cols, source.rows), 8, 1);
	IplImage ipltemp = source;
	cvCopy(&ipltemp, image2);
	IplImage *hist_image = cvCreateImage(cvSize(9000, 5000), 8, 1);
	histogr = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	cvCalcHist(&image2, histogr, 0, NULL);
	int bin_w = cvRound((double)hist_image->width / hist_size);
	cvSet(hist_image, cvScalarAll(255), 0);
	for (int i = 0; i < hist_size; i++)
		cvRectangle(hist_image, cvPoint(i*bin_w, (hist_image->height)), cvPoint((i + 1)*bin_w, (hist_image->height - cvRound(cvGetReal1D(histogr->bins, i)))), cvScalarAll(0), -1, 8, 0);
	cvNamedWindow("histogram", 1);
	IplImage* new_img = cvCreateImage(cvSize(900, 600), hist_image->depth, hist_image->nChannels);
	cvResize(hist_image, new_img);
	cvShowImage("histogram", new_img);
	cvWaitKey(0);
}
class HistCalculator
{
	Mat& pic;
	int* hist;
public:
	HistCalculator(Mat& _pic, int* _hist) : pic(_pic), hist(_hist) {}
	void operator() (const blocked_range2d<int>& r) const
	{
		for (int i = r.rows().begin(); i != r.rows().end(); i++)
		{
			for (int j = r.cols().begin(); j != r.cols().end(); j++)
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
	HistSetting(Mat& _pic, Mat& _res, int _min, int _max) : pic(_pic), res(_res), min(_min), max(_max) {}
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

	unsigned int start_time = clock();
	task_scheduler_init init(CountOfProc);
	for (int i = 0; i < result.rows; ++i)
		for (int j = 0; j < result.cols; ++j)
			result.at<uchar>(i, j) = source.at<uchar>(i, j);
	int *hist;
	hist = new int[256];
	memset(hist, 0, 256 * sizeof(int));
	int min = INT_MAX;
	int max = INT_MIN;

	parallel_for(blocked_range2d<int>(0, source.rows, 0, source.cols), HistCalculator(source, hist));

	painthist(source);
	int i = 0;
	while (i < 256 && hist[i] == 0) i++;
	min = i;
	i = 255;
	while (i > 0 && hist[i] == 0) i--;
	max = i;

	parallel_for(blocked_range2d<int>(0, source.rows, 0, source.cols), HistSetting(source, result, min, max));
	unsigned int end_time = clock(); // конечное время
	unsigned int search_time = end_time - start_time; // искомое время
	cout << "TBB ----------> " << search_time << endl;
	painthist(result);
}
int main(int argc, char* argv[])
{
	Mat input;
	int numtest = atoi(argv[1]);
	int count = atoi(argv[2]);
	string inputf = argv[3];
	string outpf = argv[4];
	string a = "test\\";
	string b = "test2\\";
	input = imread(a + inputf + ".jpg", IMREAD_GRAYSCALE);
	Mat output = Mat::zeros(input.size(), CV_8UC1);
	linearsTBB(input, output, count);
	imwrite(a + "res_" + inputf + ".jpg", output);
	string place = b + outpf + ".ans";
	typer(output, place);
}

