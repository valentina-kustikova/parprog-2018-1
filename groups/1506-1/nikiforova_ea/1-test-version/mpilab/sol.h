#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

void linears(Mat source, Mat result)
{
	for (int i = 0; i < result.rows; ++i)
		for (int j = 0; j < result.cols; ++j)
			result.at<uchar>(i, j) = source.at<uchar>(i, j);
	int *hist;
	hist = new int[256];
	for (int i = 0;i<256;i++)
	{
		hist[i] = 0;
	}
	int min = INT_MAX;
	int max = INT_MIN;

	for (int i = 0; i < source.rows; i++)
	{
		for (int j = 0; j < source.cols; j++)
		{
			hist[source.at<uchar>(i, j)]++;
		}
	}
	int isfirst = true;
	for (int r = 0;r < 256;r++)
	{
		if ((hist[r] > 0) && (isfirst == true))
		{
			min = r;
			isfirst = false;
		}
		if ((hist[r] > 0) && (r > max)) max = r;
	}
	int buf = 0;
	for (int i = 0; i < source.rows; ++i)
	{
		for (int j = 0; j < source.cols; ++j)
		{
			buf = result.at<uchar>(i, j);
			result.at<uchar>(i, j) = round(255 * (buf - min) / (max - min));

		}
	}
}

//int main()
//{
//	string a= "C:\\test\\";
//	Mat source;
//	source = imread(a+"20.jpg", IMREAD_GRAYSCALE);
//	Mat res = Mat::zeros(source.size(), CV_8UC1);
//	linears(source, res);
//
//	//equalizeHist(source, res2);
//	imwrite("result.jpg", res);
//	//imwrite("result2.jpg", res2);
//	return 0;
//}

