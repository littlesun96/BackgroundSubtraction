#include <stdio.h>
#include <cmath>
#include "opencv2/opencv.hpp"

// --------------------------------------------------------

cv::Mat& ScanImageAndReduceIterator(cv::Mat& I, int t1, int t2)
{
	// accept only char type matrices
	CV_Assert(I.depth() == CV_8U);

	const int channels = I.channels();

	int d = 254 / (t2 - t1);
	switch (channels)
	{
	case 1:
	{
		cv::MatIterator_<uchar> it, end;
		for (it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
			if (*it < t1) {
				*it = 0;
			}
			else if (*it > t2) {
				*it = 255;
			}
			else {
				*it = (*it - t1)*d + 1;
			}
			break;
	}
	case 3:
	{
		cv::MatIterator_<cv::Vec3b> it, end;
		for (it = I.begin<cv::Vec3b>(), end = I.end<cv::Vec3b>(); it != end; ++it)
		{
			(*it)[0] >>= 2;
			(*it)[1] >>= 2;
			//(*it)[2] >>= 2;
		}
	}
	}

	return I;
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
	if (argc < 4)
	{
		return 1;
		//fileName = argv[1];
	}

	std::string fileName = argv[1];
	int t1 = atoi(argv[2]);
	int t2 = atoi(argv[3]);
	if (t1 < 0) t1 = 0;
	if (t2 > 255) t2 = 255;

	cv::Mat img = cv::imread(fileName);
	if (img.empty())
	{
		return -1;
	}

	resize(img, img, cv::Size(1366, 1366 * img.rows / img.cols), cv::INTER_LINEAR);

	cv::imshow("ImgBGR", img);

	cvtColor(img, img, CV_BGR2GRAY);

	cv::imshow("ImgGray", img);


	//ScanImageAndReduceC(img);


	ScanImageAndReduceIterator(img, t1, t2);
	cv::imshow("NewImgGray", img);

	//cv::Mat img1 = cv::Mat::zeros(img.size(), img.type());


	/*cv::Mat kern = (cv::Mat_<char>(3, 3) << 0, 0.25, 0,
	1.5, 0.25, 1.5,
	0, 0.25, 0);
	*/
	//filter2D(img, img1, img.depth(), kern);

	//cv::addWeighted(img, a, img0, b, 20, img1);

	//cv::imshow("img", img);
	//cv::imshow("img0", img0);

	//imwrite("../../../4.jpg", img1);

	cv::waitKey(0);

	return 0;
}
