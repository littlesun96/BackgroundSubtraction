#ifndef BACKGROUND_SUBTRACTION_MULTI_GAUSS
#define BACKGROUND_SUBTRACTION_MULTI_GAUSS

#include "opencv2/opencv.hpp"
#include "BackgroundSubtractionSingleGauss.h"
#include <stdio.h>
#include <cmath>
#include <queue>
#include <vector>

class BackgroundSubtractionMultiGauss : public BackgroundSubtractionSingleGauss {

private:
	//std::vector<cv::Mat> models;
	std::vector<cv::Vec3f> vecModels;
	std::vector<std::vector<std::vector<cv::Vec3f>>> mats;

	float q_rsqrt(float number);

	//cv::Mat status;
	const int MODELS_COUNT = 2;
	const float A_MEAN = 0.01f;
	const float A_VARIANCE = 0.01f;
	const float A_WEIGHT = 0.2f;
	const float A_MEAN_NEG = 1 - A_MEAN;
	const float A_VARIANCE_NEG = 1 - A_VARIANCE;
	const float A_WEIGHT_NEG = 1 - A_WEIGHT;
	const float DELTA_WEIGHT_THRESHOLD = 0.2f;

	//int modelWithMinWeight(int y, int x);
	//void stepTwo(int y, int x, cv::Mat& dst);
	//void stepThree(int model, uchar u, int y, int x, cv::Mat& dst);
	//void stepFour(int y, int x, cv::Mat& dst);
	//void stepFive(int y, int x, cv::Mat& dst);
	
public:

	BackgroundSubtractionMultiGauss() : BackgroundSubtractionSingleGauss() {}

	virtual void Apply(cv::Mat& src, cv::Mat& dst);
	virtual cv::Mat GetBackground() { return cv::Mat(); }

	~BackgroundSubtractionMultiGauss();

protected:

};

#endif // !BACKGROUND_SUBTRACTION_MULTI_GAUSS