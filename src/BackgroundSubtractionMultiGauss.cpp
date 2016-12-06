#include "BackgroundSubtractionMultiGauss.h"

BackgroundSubtractionMultiGauss::~BackgroundSubtractionMultiGauss() {}

int BackgroundSubtractionMultiGauss::modelWithMinWeight(int y, int x)
{
	if (models[0].at<cv::Vec3f>(y, x)[0] > models[1].at<cv::Vec3f>(y, x)[0])
	{
		return (models[1].at<cv::Vec3f>(y, x)[0] > models[2].at<cv::Vec3f>(y, x)[0]) ? 2 : 1;
	}
	else {
		return (models[0].at<cv::Vec3f>(y, x)[0] > models[2].at<cv::Vec3f>(y, x)[0]) ? 2 : 0;
	}
}

void BackgroundSubtractionMultiGauss::stepTwo(int y, int x) 
{
	float minWeight = modelWithMinWeight(y, x);
	if (minWeight == 0)
	{
		stepFour(y, x);
	}
	else {
		stepFive(y, x);
	}
}

void BackgroundSubtractionMultiGauss::stepThree(int model, uchar u, int y, int x)
{
	cv::Vec3f v = models[model].at<cv::Vec3f>(y, x);
	v[1] = (1 - A_MEAN) * v[1] + A_MEAN * u;
	v[2] = (1 - A_VARIANCE) * v[2] + A_VARIANCE * (v[1] - u) * (v[1] - u);
}

void BackgroundSubtractionMultiGauss::stepFour(int y, int x)
{

}

void BackgroundSubtractionMultiGauss::stepFive(int y, int x)
{

}

void BackgroundSubtractionMultiGauss::Apply(cv::Mat& src, cv::Mat& dst) 
{
	if (models.empty()) {
		models.push_back(cv::Mat(src.size(), CV_32FC3, cv::Scalar(0, 0, 0)));
		status = cv::Mat(src.size(), CV_8U, cv::Scalar(0, 0, 0));
		for (int y = 0; y < src.rows; y++)
		{
			for (int x = 0; x < src.cols; x++)
			{
				status.at<uchar>(y, x) = -1;
				cv::Vec3f v;
				v[0] = 1;
				v[1] = src.at<uchar>(y, x);
				v[2] = 100;
				models[0].at<cv::Vec3f>(y, x) = v;
				v[0] = 0;
				models[1].at<cv::Vec3f>(y, x) = models[1].at<cv::Vec3f>(y, x) = v;
				
			}
		}
		return;
	}

	bool satisfyThreshold = false;

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			for (int q = 0; q < models.size(); q++)
			{
				uchar u = src.at<uchar>(y, x);
				cv::Vec3f v = models[q].at<cv::Vec3f>(y, x);
				if (v[0] != 0 && abs(v[1] - u) / sqrt(v[2]) <= DELTA_THRESHOLD)
				{
					status.at<uchar>(y, x) = q;
					stepThree(q, u, y, x);
					break;
				}
				if (q == 2) 
				{
					stepTwo(y, x);
				}
			}

		}
	}

}