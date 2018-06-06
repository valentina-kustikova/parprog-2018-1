#pragma once
#include <omp.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <ctime>
#include <fstream>
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
	//cvCalcHist(ipltemp, histogr, 0, NULL);
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
void linearsPARALLEL(Mat source, Mat result, int CountOfProc)
{
	unsigned int start_time = clock();
	for (int i = 0; i < result.rows; ++i)
		for (int j = 0; j < result.cols; ++j)
			result.at<uchar>(i, j) = source.at<uchar>(i, j);
	int *hist;
	hist = new int[256];
	memset(hist, 0, 256 * sizeof(int));
	int min = INT_MAX;
	int max = INT_MIN;
	int tid;
	omp_set_num_threads(CountOfProc);
#pragma omp parallel shared(hist,source) private(tid)
	{
		int n = omp_get_max_threads();
		tid = omp_get_thread_num();


#pragma omp for schedule(guided)
		for (int i = 0; i < source.rows; i++)
		{
			for (int j = 0; j < source.cols; j++)
			{
				hist[source.at<uchar>(i, j)]++;
			}
		}
	}
	painthist(source);
	int i = 0;
	while (i < 256 && hist[i] == 0) i++;
	min = i;
	i = 255;
	while (i > 0 && hist[i] == 0) i--;
	max = i;
#pragma omp parallel shared(hist,result) private(tid)
	{
		int n = omp_get_max_threads();
		tid = omp_get_thread_num();

#pragma omp for schedule(guided)
		for (int i = 0; i <result.rows; i++)
		{
			for (int j = 0; j < result.cols; j++)
			{
				result.at<uchar>(i, j) = round(255 * (result.at<uchar>(i, j) - min) / (max - min));
			}
		}
	}
	unsigned int end_time = clock(); // конечное время
	unsigned int search_time = end_time - start_time; // искомое время
	cout << "OMP ----------> " << search_time << endl;
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
	linearsPARALLEL(input, output, count);
	imwrite(a + "res_" + inputf + ".jpg", output);
	string place = b + outpf + ".ans";
	typer(output, place);
}