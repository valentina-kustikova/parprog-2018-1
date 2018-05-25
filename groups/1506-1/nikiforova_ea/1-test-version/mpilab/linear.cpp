#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include <fstream>
#include <ctime>
using namespace cv;
using namespace std;
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
void linears(Mat source, Mat result)
{
	//unsigned int start_time = clock();
	for (int i = 0; i < result.rows; ++i)
		for (int j = 0; j < result.cols; ++j)
			result.at<uchar>(i, j) = source.at<uchar>(i, j);
	int *hist;
	hist = new int[256];
	memset(hist, 0, 256 * sizeof(int));
	int min = INT_MAX;
	int max = INT_MIN;

	for (int i = 0; i < source.rows; i++)
	{
		for (int j = 0; j < source.cols; j++)
		{
			hist[source.at<uchar>(i, j)]++;
		}
	}
	painthist(source);


	int i = 0;
	while (i < 256 && hist[i] == 0) i++;
	min = i;
	i = 255;
	while (i > 0 && hist[i] == 0) i--;
	max = i;

	double buf = 0;
	for (int i = 0; i < result.rows; i++)
	{
		for (int j = 0; j < result.cols; j++)
		{
			result.at<uchar>(i, j) = round(255 * (result.at<uchar>(i, j) - min) / (max - min));
		}
	}
	//unsigned int end_time = clock(); // конечное время
	//unsigned int search_time = end_time - start_time; // искомое время
	//cout << search_time << endl;
	painthist(result);

}
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
	unsigned int start_time = clock();
	linears(input, output);
	unsigned int end_time = clock(); // конечное время
	unsigned int search_time = end_time - start_time; // искомое время
	cout << "LINE ----------> " << search_time << endl;
	imwrite(a + "res_" + inputf + ".jpg", output);
	string place = b + outpf + ".ans";
	typer(output, place);
}

