#include <stdio.h>
#include <cmath>
#include <memory>
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "BackgroundSubtractionAverage.h"
#include "BackgroundSubtractionSingleGauss.h"


// --------------------------------------------------------

int main(int argc, char* argv[])
{
	std::string fileName("../../atrium.avi");
	if (argc > 1)
	{
		//return -1;
		fileName = argv[1];
	}

	//std::string fileName = argv[1];
	//int t = atoi(argv[2]);

	cv::namedWindow("image", cv::WINDOW_NORMAL);
	//cv::namedWindow("maskAv", cv::WINDOW_NORMAL);
	cv::namedWindow("maskSG", cv::WINDOW_NORMAL);

	cv::VideoCapture cap;
	cap.open(fileName);
	
	if (!cap.isOpened())
	{
		return -1;
	}

	std::shared_ptr<BackgroundSubtractionBase> bgSubSG(new BackgroundSubtractionSingleGauss());
	//std::shared_ptr<BackgroundSubtractionBase> bgSubAv(new BackgroundSubtractionAverage());

	cv::Mat img1, img;

	for (;;) {
		cap >> img;

		if (img.empty()) {
			break;
		}
		
		cvtColor(img, img, CV_BGR2GRAY);

		//bgSubAv->Apply(img, img1);

		cv::imshow("image", img);
		//cv::imshow("maskAv", img1);

		bgSubSG->Apply(img, img1);

		cv::imshow("maskSG", img1);

		int c = cv::waitKey(30);
		if (c == 'q' || c == 'Q' || (c & 255) == 27)
			break;

	}

	return 0;
}
