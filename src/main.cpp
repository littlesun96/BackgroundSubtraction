#include <stdio.h>
#include <cmath>
#include <memory>
#include <time.h>
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "BackgroundSubtractionAverage.h"
#include "BackgroundSubtractionSingleGauss.h"
#include "BackgroundSubtractionMultiGauss.h"


// --------------------------------------------------------

int main(int argc, char* argv[])
{
	std::string fileName("../../video/16_00_06.789.avi");
	//std::string fileName("../../atrium.avi");
	if (argc > 1)
	{
		//return -1;
		fileName = argv[1];
	}

	//std::string fileName = argv[1];
	//int t = atoi(argv[2]);

	cv::namedWindow("image", cv::WINDOW_NORMAL);
	//cv::namedWindow("maskSG", cv::WINDOW_NORMAL);
	cv::namedWindow("maskMG", cv::WINDOW_NORMAL);

	cv::VideoCapture cap;
	cap.open(fileName);
	
	if (!cap.isOpened())
	{
		return -1;
	}

	std::shared_ptr<BackgroundSubtractionBase> bgSubMG(new BackgroundSubtractionMultiGauss());
	//std::shared_ptr<BackgroundSubtractionBase> bgSubSG(new BackgroundSubtractionSingleGauss());

	cv::Mat img1, img;
	auto time1 = clock();
	auto time2 = clock();

	for (;;) {

		cap >> img;

		if (img.empty()) {
			break;
		}
		
		cvtColor(img, img, CV_BGR2GRAY);

		//bgSubSG->Apply(img, img1);

		cv::imshow("image", img);
		//cv::imshow("maskSG", img1);

		time1 = clock();

		bgSubMG->Apply(img, img1);
		time2 = clock();

		cv::imshow("maskMG", img1);
		std::cout << time2 - time1 << std::endl;

		int c = cv::waitKey(30);
		if (c == 'q' || c == 'Q' || (c & 255) == 27)
			break;

	}

	return 0;
}
