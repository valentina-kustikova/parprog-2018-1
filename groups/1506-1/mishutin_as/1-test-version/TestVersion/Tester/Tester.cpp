#include "SeqFilter.h"
#include <string>
#include <iostream>

#define RADIUS 1

using namespace std;



int main(int argc, char **argv) {
	string sourceName = "../../TestVersion/source", outputName = "../../TestVersion/output", stdoutputName = "../../TestVersion/stdoutput";
	cv::Mat source, output, stdfilter;
	int num = atoi(argv[1]); //1st arg of cmd
	sourceName += std::to_string(num) + ".png";
	outputName += std::to_string(num) + ".png";
	stdoutputName += std::to_string(num) + ".png";
	double sigma = stod(argv[2]);

	output = cv::imread(outputName, CV_LOAD_IMAGE_COLOR); //loads in BGR
	imshow("output1", output);
	stdfilter = cv::imread(stdoutputName, CV_LOAD_IMAGE_COLOR); //loads in BGR
	imshow("stdfilter", stdfilter);
	waitKey();
	return 0;
}