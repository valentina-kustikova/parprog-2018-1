#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <string>


using namespace std;
// 7 is a min number, that gets TRUE in comparasion
bool checkRes(cv::Mat &output, cv::Mat &stdoutput) {
	bool res = true;
	if (output.size() != stdoutput.size()) { return false; }
	else {
		for (int i = 0; i < output.rows; ++i) { 
			for (int j = 0; j < output.cols; ++j) {
				if ((output.at<cv::Vec3b>(i, j)[0] - stdoutput.at<cv::Vec3b>(i, j)[0] > 7) ||
					(output.at<cv::Vec3b>(i, j)[1] - stdoutput.at<cv::Vec3b>(i, j)[1] > 7) ||
					(output.at<cv::Vec3b>(i, j)[2] - stdoutput.at<cv::Vec3b>(i, j)[2] > 7)
					) return false;
			}
		}
	}

	return res;
}

//checkers needs only img numer
int main(int argc, char **argv) {
	string sourceName = "../../TestVersion/source", outputName = "../../TestVersion/output", stdoutputName = "../../TestVersion/stdoutput";
	cv::Mat source, output, stdfilter;
	int num = atoi(argv[1]);
	sourceName += std::to_string(num) + ".png";
	outputName += std::to_string(num) + ".png";
	stdoutputName += std::to_string(num) + ".png";
	
	output = cv::imread(outputName, CV_LOAD_IMAGE_COLOR); //loads in BGR
	imshow("output1", output);
	stdfilter = cv::imread(stdoutputName, CV_LOAD_IMAGE_COLOR); //loads in BGR
	imshow("stdfilter", stdfilter);
	printf(checkRes(output, stdfilter) ? "true" : "false");
	cv::waitKey();
	return 0;
}