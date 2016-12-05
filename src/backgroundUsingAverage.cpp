#include <stdio.h>
#include <cmath>
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

// --------------------------------------------------------

cv::Mat& ScanImageAndReduceIterator(cv::Mat& I1, cv::Mat& I2, cv::Mat& IAverage, int t)
{
	// accept only char type matrices
	CV_Assert(I1.depth() == CV_8U && I2.depth() == CV_8U);

	const int channels = I1.channels();

	double a = 0.1;

	switch (channels)
	{
	case 1:
	{
		cv::MatIterator_<uchar> it1, end1;
		cv::MatIterator_<uchar> it2, end2;
		cv::MatIterator_<uchar> itAv, endAv;
		it2 = I2.begin<uchar>();
		itAv = IAverage.begin<uchar>();
		for (it1 = I1.begin<uchar>(), end1 = I1.end<uchar>(); it1 != end1; ++it1) {
			int d = abs((*itAv) - (*it2));
			*it1 = (d > t) ? 255 : 0;
			*itAv = a * (*itAv) + (1 - a) * (*it2);
			it2++;
			itAv++;
		}

		break;
	}
	case 3:
	{
		cv::MatIterator_<cv::Vec3b> it, end;
		for (it = I1.begin<cv::Vec3b>(), end = I1.end<cv::Vec3b>(); it != end; ++it)
		{
			(*it)[0] >>= 2;
			(*it)[1] >>= 2;
			//(*it)[2] >>= 2;
		}
	}
	}

	return I1;
}

cv::Mat& ScanImageAndReduceC(cv::Mat& I)
{
	// accept only char type matrices
	CV_Assert(I.depth() == CV_8U);

	int channels = I.channels();

	int nRows = I.rows;
	int nCols = I.cols * channels;

	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* p;
	for (i = 0; i < nRows; ++i)
	{
		p = I.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j)
		{
			//p[j] = 255 - p[j];
			if (j % 3 != 2 && p[j] != 0) p[j] >>= 2;
		}
	}
	return I;
}

int main(int argc, char* argv[])
{
	//std::string fileName("../../../3.jpg");
	if (argc < 3)
	{
		return -1;
		//fileName = argv[1];
	}

	std::string fileName = argv[1];
	int t = atoi(argv[2]);

	cv::namedWindow("image", cv::WINDOW_NORMAL);
	cv::namedWindow("mask", cv::WINDOW_NORMAL);

	cv::VideoCapture cap;
	cap.open(fileName);

	if (!cap.isOpened())
	{
		return -1;
	}

	cv::Mat img1, img2;

	cap >> img1;
	if (img1.empty()) {
		return -1;
	}
	//resize(img1, img1, cv::Size(640, 640 * img1.rows / img1.cols), cv::INTER_LINEAR);
	cvtColor(img1, img1, CV_BGR2GRAY);
	cv::imshow("image", img1);

	cv::Mat imgAverage;
	img1.copyTo(imgAverage);

	for (;;) {
		cap >> img2;

		if (img2.empty()) {
			break;
		}
		//resize(img2, img2, cv::Size(640, 640 * img2.rows / img2.cols), cv::INTER_LINEAR);
		cvtColor(img2, img2, CV_BGR2GRAY);

		img1 = ScanImageAndReduceIterator(img1, img2, imgAverage, t);
		cv::Mat img3;
		cv::Mat kern3 = (cv::Mat_<char>(3, 3) << 1, 1, 1,
			1, 1, 1,
			1, 1, 1);
		cv::morphologyEx(img1, img3, cv::MORPH_OPEN, kern3);

		cv::imshow("image", img2);
		cv::imshow("mask", img3);
		img1 = img2;

		int c = cv::waitKey(30);
		if (c == 'q' || c == 'Q' || (c & 255) == 27)
			break;

	}

	//cv::waitKey(0);

	return 0;
}
