#include "SeqFilter.h"
#include <iostream>
#include <cmath>

using namespace cv;

//fill kernel for gaussian blur
void fillKernel(double **kernel, int radius, double sigma) {
for (int i = -radius; i <= radius; ++i) {
	for (int j = -radius; j <= radius; ++j) {
		kernel[i+radius][j+radius] = 1 / (2 * CV_PI*sigma*sigma)*(exp(-(i*i + j*j) / (2 * sigma*sigma)));
		}
	}
}

//sequential gaussian filter
void seqGaussFilter(Mat& source, Mat& output, int radius, double sigma) {
	double **kernel;
	kernel = new double*[radius * 2 + 1];
	for (int i = 0; i < radius * 2 + 1; ++i) kernel[i] = new double[radius * 2 + 1];
	fillKernel(kernel, radius, sigma);
	//check kernel
	for (int i = 0; i < radius * 2 + 1; ++i) {
		for (int j = 0; j < radius * 2 + 1; ++j) {
			printf("%10.3f\t", kernel[i][j]);
		}
		std::cout << std::endl;
	}
	//left top corner
	output.at<Vec3b>(0, 0)[0] = source.at<Vec3b>(0, 0)[0];
	output.at<Vec3b>(0, 0)[1] = source.at<Vec3b>(0, 0)[1];
	output.at<Vec3b>(0, 0)[2] = source.at<Vec3b>(0, 0)[2];
	//left bot corner
	output.at<Vec3b>(output.rows - 1, 0)[0] = source.at<Vec3b>(source.rows - 1, 0)[0];
	output.at<Vec3b>(output.rows - 1, 0)[1] = source.at<Vec3b>(source.rows - 1, 0)[1];
	output.at<Vec3b>(output.rows - 1, 0)[2] = source.at<Vec3b>(source.rows - 1, 0)[2];
	//right top	corner
	output.at<Vec3b>(0, output.cols - 1)[0] = source.at<Vec3b>(0, source.cols - 1)[0];
	output.at<Vec3b>(0, output.cols - 1)[1] = source.at<Vec3b>(0, source.cols - 1)[1];
	output.at<Vec3b>(0, output.cols - 1)[2] = source.at<Vec3b>(0, source.cols - 1)[2];
	//right bot corner
	output.at<Vec3b>(output.rows - 1, output.cols - 1)[0] = source.at<Vec3b>(source.rows - 1, source.cols - 1)[0];
	output.at<Vec3b>(output.rows - 1, output.cols - 1)[1] = source.at<Vec3b>(source.rows - 1, source.cols - 1)[1];
	output.at<Vec3b>(output.rows - 1, output.cols - 1)[2] = source.at<Vec3b>(source.rows - 1, source.cols - 1)[2];

	//copying left border
	for (int i = 0; i < source.rows; ++i) {
		output.at<Vec3b>(i + 1, 0)[0] = source.at<Vec3b>(i, 0)[0];
		output.at<Vec3b>(i + 1, 0)[1] = source.at<Vec3b>(i, 0)[1];
		output.at<Vec3b>(i + 1, 0)[2] = source.at<Vec3b>(i, 0)[2];
	}
	//copying right border
	for (int i = 0; i < source.rows; ++i) {
		output.at<Vec3b>(i + 1, output.cols - 1)[0] = source.at<Vec3b>(i, source.cols - 1)[0];
		output.at<Vec3b>(i + 1, output.cols - 1)[1] = source.at<Vec3b>(i, source.cols - 1)[1];
		output.at<Vec3b>(i + 1, output.cols - 1)[2] = source.at<Vec3b>(i, source.cols - 1)[2];
	}
	//copying top border
	for (int j = 0; j < source.cols; ++j) {
		output.at<Vec3b>(0, j + 1)[0] = source.at<Vec3b>(0, j)[0];
		output.at<Vec3b>(0, j + 1)[1] = source.at<Vec3b>(0, j)[1];
		output.at<Vec3b>(0, j + 1)[2] = source.at<Vec3b>(0, j)[2];
	}
	//copying bot border
	for (int j = 0; j < source.cols; ++j) {
		output.at<Vec3b>(output.rows - 1, j + 1)[0] = source.at<Vec3b>(source.rows - 1, j)[0];
		output.at<Vec3b>(output.rows - 1, j + 1)[1] = source.at<Vec3b>(source.rows - 1, j)[1];
		output.at<Vec3b>(output.rows - 1, j + 1)[2] = source.at<Vec3b>(source.rows - 1, j)[2];
	}
	//copying the rest of the image
	for (int i = 0; i < source.rows; ++i) {
		for (int j = 0; j < source.cols; ++j) {
			output.at<Vec3b>(i + 1, j + 1)[0] = source.at<Vec3b>(i, j)[0];
			output.at<Vec3b>(i + 1, j + 1)[1] = source.at<Vec3b>(i, j)[1];
			output.at<Vec3b>(i + 1, j + 1)[2] = source.at<Vec3b>(i, j)[2];
		}
	}
	//work with inner image
	for (int x = 1; x < output.rows - 1; ++x) {
		for (int y = 1; y < output.cols - 1; ++y) {
			double rSum = 0, gSum = 0, bSum = 0;
			//work with kernel
			for (int i = 0; i < radius * 2 + 1; ++i) {
				for (int j = 0; j < radius * 2 + 1; ++j) {
					int posX = x + ( i - (radius * 2 + 1) / 2);
					int posY = y + (j - (radius * 2 + 1) / 2);

				}
			}

		}
	}
	delete(kernel);
}
