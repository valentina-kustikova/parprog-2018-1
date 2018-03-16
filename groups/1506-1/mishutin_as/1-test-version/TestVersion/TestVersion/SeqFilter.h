#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

void seqGaussFilter(Mat& source, Mat& output, int radius, double sigma);
void fillKernel(double **kernel, int radius, double sigma);
