#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "SeqFilter.h"



#define RADIUS 1
#define SIGMA 2
 
//linear image filtration, (blocks) Gaussian Filter 3x3

using namespace cv;
using namespace std;



int main(int argc, char** argv) {
	Mat source, output, stdfilter;
	source = imread("source1.png", CV_LOAD_IMAGE_COLOR); //loads in BGR
	source.copyTo(stdfilter);
	output = Mat::zeros(source.rows + 2, source.cols + 2, CV_8UC3);
	seqGaussFilter(source, output, RADIUS, SIGMA);
	imshow("source1", source);
	GaussianBlur(source, stdfilter, Size(3,3), SIGMA); //just for check, standart cv filter
	imshow("stdfilter", stdfilter);
	imshow("filtered", output);
	imwrite("output1.png", output);
	waitKey();
	return 0;
}
