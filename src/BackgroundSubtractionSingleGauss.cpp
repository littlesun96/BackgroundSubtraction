#include "BackgroundSubtractionSingleGauss.h"

BackgroundSubtractionSingleGauss::~BackgroundSubtractionSingleGauss() {}


void BackgroundSubtractionSingleGauss::Apply(cv::Mat& src, cv::Mat& dst) 
{

	// accept only char type matrices
	CV_Assert(src.depth() == CV_8U);

	dst = cv::Mat(src.size(), src.type());
	const int channels = src.channels();
	float a1 = 0.05f;
	float a2 = 0.02f;


	//Начальная диспресия и матожидание
	if (count < INITIAL_FRAMES_COUNT) 
	{

		if (model.size() != src.size())
		{
			model = cv::Mat(src.size(), CV_32FC2, cv::Scalar(0, 0, 0));
		}

		
		for (int y = 0; y < src.rows; ++y)
		{
			for (int x = 0; x < src.cols; ++x)
			{
				cv::Vec2f v;
				v[0] = src.at<uchar>(y, x);
				v[1] = v[0] * v[0];

				model.at<cv::Vec2f>(y, x) += v;
			}
		}

		count++;
	}

	//Набралось заданное количество
	if (!initialFrames && count == INITIAL_FRAMES_COUNT) {
		for (int y = 0; y < src.rows; ++y)
		{
			for (int x = 0; x < src.cols; ++x)
			{
				model.at<cv::Vec2f>(y, x)[0] /= INITIAL_FRAMES_COUNT;
				model.at<cv::Vec2f>(y, x)[1] = model.at<cv::Vec2f>(y, x)[1] / (INITIAL_FRAMES_COUNT - 1) - model.at<cv::Vec2f>(y, x)[0] * model.at<cv::Vec2f>(y, x)[0];
			}
		}
		initialFrames = true;
	}

	switch (channels) 
	{
		case 1: 
		{
			for (int y = 0; y < src.rows; ++y)
			{
				for (int x = 0; x < src.cols; ++x)
				{
					cv::Vec2f v = model.at<cv::Vec2f>(y, x);
					uchar u = src.at<uchar>(y, x);
					dst.at<uchar>(y, x) = ((abs(v[0] - u))/sqrt(v[1]) > DELTA_THRESHOLD) ? 255 : 0;
					{
						v[0] = (1 - a1) * v[0] + a1 * u;
						v[1] = (1 - a2) * v[1] + a2 * (u - v[0]) * (u - v[0]);
						model.at<cv::Vec2f>(y, x) = v;
						
					}
					
				}
			}

			//cv::morphologyEx(dst, dst, cv::MORPH_OPEN, kern3);

			break;
		}
	}

}