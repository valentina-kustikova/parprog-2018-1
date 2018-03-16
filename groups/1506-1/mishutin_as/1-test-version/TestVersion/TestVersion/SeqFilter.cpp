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
void seqGaussFilter(Mat& source, Mat& output1, int radius, double sigma) {
	double **kernel;
	//output is a temporary matrix
	Mat output = Mat::zeros(source.rows + 2, source.cols + 2, CV_8UC3);
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
			double rSum = 0, gSum = 0, bSum = 0, kSum = 0;
			//work with kernel
			for (int i = 0; i < radius * 2 + 1; ++i) {
				for (int j = 0; j < radius * 2 + 1; ++j) {
					int posX = x + ( i - (radius * 2 + 1) / 2);
					int posY = y + (j - (radius * 2 + 1) / 2);
					int r = output.at<Vec3b>(posX, posY)[0];
					int g = output.at<Vec3b>(posX, posY)[1];
					int b = output.at<Vec3b>(posX, posY)[2];
					double kernelVal = kernel[i][j];
					rSum += r * kernelVal;
					gSum += g * kernelVal;
					bSum += b * kernelVal;

					kSum += kernelVal;

				}
			}
			if (kSum <= 0) kSum = 1;

			// онтролируем переполнени€ переменных
			rSum /= kSum;
			if (rSum < 0) rSum = 0;
			if (rSum > 255) rSum = 255;

			gSum /= kSum;
			if (gSum < 0) gSum = 0;
			if (gSum > 255) gSum = 255;

			bSum /= kSum;
			if (bSum < 0) bSum = 0;
			if (bSum > 255) bSum = 255;
			output.at<Vec3b>(x,y)[0] = rSum;
			output.at<Vec3b>(x, y)[1] = gSum;
			output.at<Vec3b>(x, y)[2] = bSum;

		}
	}	
	//copying into the final image
	for (int i = 1; i < output.rows -1; ++i) {
		for (int j = 1; j < output.cols -1; ++j) {
			output1.at<Vec3b>(i - 1, j - 1)[0] = output.at<Vec3b>(i, j)[0];
			output1.at<Vec3b>(i - 1, j - 1)[1] = output.at<Vec3b>(i, j)[1];
			output1.at<Vec3b>(i - 1, j - 1)[2] = output.at<Vec3b>(i, j)[2];
		}
	}

	delete(kernel);
}
