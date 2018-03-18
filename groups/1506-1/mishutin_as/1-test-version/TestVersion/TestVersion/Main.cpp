#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "SeqFilter.h"
#include <string>

//do NOT change RADIUS
#define RADIUS 1

//linear image filtration, (blocks) Gaussian Filter 3x3

using namespace cv;
using namespace std;



int main(int argc, char** argv) {
	string sourceName = "../../TestVersion/source", outputName = "../../TestVersion/output", stdoutputName = "../../TestVersion/stdoutput";
	int number = 1;
	number = atoi(argv[1]);
	Mat source, output, stdfilter;

	sourceName += std::to_string(number) + ".png";
	outputName += std::to_string(number) + ".png";
	stdoutputName += std::to_string(number) + ".png";
	try {
		Exception e;
		source = imread(sourceName, CV_LOAD_IMAGE_COLOR);
		throw e;
		//loads in BGR
		source.copyTo(stdfilter);
		output = Mat::zeros(source.rows, source.cols, CV_8UC3);
		double sigma = stod(argv[2]);
		seqGaussFilter(source, output, RADIUS, sigma);
		imshow("source1", source);
		GaussianBlur(source, stdfilter, Size(3, 3), sigma); //just for check, standart cv filter
		imshow("stdfilter", stdfilter);
		imwrite(stdoutputName, stdfilter);
		imshow("filtered", output);
		imwrite(outputName, output);
	}
	catch (Exception e) {
		cout << "No such img!" << endl;
		printf(e.msg.c_str());
	}
	waitKey();
	return 0;
}
