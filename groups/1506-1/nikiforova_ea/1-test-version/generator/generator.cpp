#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range2d.h"
#include "TBB.h"
#include "sol.h"
#include "Parallel.h"
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

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
Mat generatepic(int a, int b)
{
	Mat pic = Mat::zeros(2000,2000, CV_8UC1);
	for (int i=0;i<pic.rows;i++)
		for (int j=0;j<pic.cols;j++)
		{
			pic.at<uchar>(i, j) = a+rand() % (b-a);
		}
	return pic;
}
Mat generatepicCIRCLE(int a, int b)
{

	Mat pic = Mat::zeros(300, 300, CV_8UC1);
	int n = a + rand() % (b - a);
	for (int i = 0; i < pic.rows; ++i)
		for (int j = 0; j < pic.cols; ++j)
			pic.at<uchar>(i, j) = n;
	int x = 0;
	int y = 0;
	for (int i = 0;i < 10;i++)
	{
		x = rand() % 300;
		y = rand() % 300;
		circle(pic, cvPoint(x, y), 30, a + rand() % (b - a), -1, 8, 0);
	}
	return pic;
}
void clamp(int a, int b)
{
	if (b < a) swap(a,b);
	if (a < 0) a = 0;
	if (b > 255) b = 255;
}
int main(int argc, char* argv[])
{
	    srand(time(0));
		int numtest = atoi(argv[1]);
		int count = atoi(argv[2]);         
		string inputf = argv[3];           
		string outpf = argv[4];				
		int min = stoi(argv[5]);			
		int max = stoi(argv[6]);
		clamp(min, max);
		Mat input;
		string a = "test\\";
		string b = "test2\\";
		imwrite(a + inputf + ".jpg", generatepic(min, max));		
		input = imread(a + inputf + ".jpg", IMREAD_GRAYSCALE);     
		Mat output = Mat::zeros(input.size(), CV_8UC1);
		//linearsPARALLEL(input, output,count);                            
		linearsTBB(input, output, count);
		imwrite(a + "res_" + inputf + ".jpg", output);					
		//пишу исходник 
		string place = b + to_string(numtest);
		typer(input, place);
		//пишу результат
		place = b + outpf + ".ans";
		typer(output, place);

	return 0;
}
