#ifndef BACKGROUND_SUBTRACTION_SINGLE_GAUSS
#define BACKGROUND_SUBTRACTION_SINGLE_GAUSS

#include "opencv2/opencv.hpp"
#include "BackgroundSubtractionBase.h"
#include <stdio.h>
#include <cmath>
#include <queue>
#include <vector>

class BackgroundSubtractionSingleGauss : public BackgroundSubtractionBase {

private:
	//cv::Mat mean;
	//cv::Mat variance;
	std::vector<std::vector<double>> meanArray;
	std::vector<std::vector<double>> varianceArray;
	//������� �� ��������� INITIAL_FRAMES_COUNT ������ ��� �������� ���������� ����������� � ���������
	std::queue<cv::Mat> queue;
	const int INITIAL_FRAMES_COUNT = 3;
	const int DELTA_THRESHOLD = 3;
	bool initialFrames = false;
	//void countInitialVariance();
	void stdDev(int i, int k, std::vector<cv::MatIterator_<uchar>> a);

public:

	BackgroundSubtractionSingleGauss() : BackgroundSubtractionBase() {}

	virtual void Apply(cv::Mat& src, cv::Mat& dst);
	virtual cv::Mat GetBackground() { return cv::Mat(); }

	~BackgroundSubtractionSingleGauss();

protected:

};

#endif // !BACKGROUND_SUBTRACTION_SINGLE_GAUSS