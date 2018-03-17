#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "sol.h"
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

int main(int argc, char* argv[])
{
	int numtest = atoi(argv[1]);
		Mat input;
		string a = "C:\\test\\";
		string b = "C:\\test2\\";

		input = imread(a + to_string(numtest) + ".jpg", IMREAD_GRAYSCALE);
		Mat output = Mat::zeros(input.size(), CV_8UC1);
		linears(input, output);
		//в последствии заменим на работу параллельной версии
		//пишу исходник 
		string place = b + to_string(numtest);
		typer(input, place);
		//пишу результат
		place = b + to_string(numtest) + ".ans";
		typer(output, place);
	
	return 0;
}
