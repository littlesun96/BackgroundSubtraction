#ifndef BACKGROUND_SUBTRACTION_AVERAGE_H
#define BACKGROUND_SUBTRACTION_AVERAGE_H


#include "opencv2/opencv.hpp"
#include "BackgroundSubtractionBase.h"
#include <stdio.h>
#include <cmath>

class BackgroundSubtractionAverage : public BackgroundSubtractionBase {
private:
	cv::Mat frameAverage;
	//int threshold = 20;
	
public:

	BackgroundSubtractionAverage() : BackgroundSubtractionBase() {}

	virtual void Apply(cv::Mat& src, cv::Mat& dst);
	//void SetThreshold(int t);
	//int GetThreshold();
	virtual cv::Mat GetBackground() { return cv::Mat(); }

	~BackgroundSubtractionAverage();

protected:
	/*cv::Mat kern3 = (cv::Mat_<char>(3, 3) << 1, 1, 1,
											 1, 1, 1,
											 1, 1, 1);*/
};

#endif // !BACKGROUND_SUBTRACTION_AVERAGE_H