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
	cv::Mat model;
	const int INITIAL_FRAMES_COUNT = 5;
	int count = 0;
	bool initialFrames = false;

public:

	BackgroundSubtractionSingleGauss() : BackgroundSubtractionBase() {}

	virtual void Apply(cv::Mat& src, cv::Mat& dst);
	virtual cv::Mat GetBackground() { return cv::Mat(); }

	~BackgroundSubtractionSingleGauss();

protected:
	const float DELTA_THRESHOLD = 2.5f;
};

#endif // !BACKGROUND_SUBTRACTION_SINGLE_GAUSS