#ifndef BACKGROUND_SUBTRACTION_BASE_H
#define BACKGROUND_SUBTRACTION_BASE_H


#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <cmath>

class BackgroundSubtractionBase {
private:
	cv::Mat frameAverage;

public:

	BackgroundSubtractionBase() {}

	virtual void Apply(cv::Mat& src, cv::Mat& dst);
	void SetThreshold(int t);
	int GetThreshold();
	virtual cv::Mat GetBackground() { return cv::Mat(); }

	~BackgroundSubtractionBase();

protected:
	int threshold = 20;
	cv::Mat kern3 = (cv::Mat_<char>(3, 3) << 1, 1, 1,
											 1, 1, 1,
											 1, 1, 1);
};

#endif // !BACKGROUND_SUBTRACTION_BASE_H