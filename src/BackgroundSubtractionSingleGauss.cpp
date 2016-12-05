#include "BackgroundSubtractionSingleGauss.h"

BackgroundSubtractionSingleGauss::~BackgroundSubtractionSingleGauss() {}

//Подсчет начальной дисперсии
/*void BackgroundSubtractionSingleGauss::countInitialVariance() 
{
	if (queue.empty())
		return;
	cv::Mat m = queue.front();
	queue.pop();
	//const int channels = m.channels();
	cv::MatIterator_<uchar> it1, end1, itVar, itMean;
	itMean = mean.begin<uchar>();
	itVar = variance.begin<uchar>();
	for (it1 = m.begin<uchar>(), end1 = m.end<uchar>(); it1 != end1; ++it1) 
	{
		*itVar += pow(*it1 - *itMean, 2);
		itMean++;
		itVar++;
	}
	countInitialVariance();
}*/

void BackgroundSubtractionSingleGauss::stdDev(int i, int k, std::vector<cv::MatIterator_<uchar>> a) 
{
	double sum = 0;
	double sq_sum = 0;
	for (int q = 0; q < INITIAL_FRAMES_COUNT; ++q) {
		sum += *a[q];
		sq_sum += (*a[q]) * (*a[q]);
		a[q]++;
	}
	meanArray[i].push_back(sum / (INITIAL_FRAMES_COUNT - 1));
	varianceArray[i].push_back(sq_sum / (INITIAL_FRAMES_COUNT - 1) - meanArray[i][k] * meanArray[i][k]);
}

void BackgroundSubtractionSingleGauss::Apply(cv::Mat& src, cv::Mat& dst) 
{

	// accept only char type matrices
	CV_Assert(src.depth() == CV_8U);

	dst = cv::Mat(src.size(), src.type());
	const int channels = src.channels();
	double a = 0.02;
	//int n = 10;

	//Первые кадры в очередь
	if (!initialFrames && queue.size() < INITIAL_FRAMES_COUNT) 
	{
		queue.push(src);
		return;
	}

	//Набралось заданное количество
	if (!initialFrames && queue.size() == INITIAL_FRAMES_COUNT) 
	{
		/*mean = cv::Mat(src.size(), src.type());
		variance = cv::Mat(src.size(), src.type());
		//Подсчет начального матожидания
		for (int i = 0; i < INITIAL_FRAMES_COUNT; i++) 
		{
			cv::Mat x = queue.front();
			queue.pop();
			mean += x;
			queue.push(x);
		}
		mean /= INITIAL_FRAMES_COUNT - 1;
		//Подсчет дисперсии
		countInitialVariance();
		variance /= INITIAL_FRAMES_COUNT - 1;*/

		std::vector<cv::MatIterator_<uchar>> a;
		for (int q = 0; q < INITIAL_FRAMES_COUNT; q++) {
			a.push_back(queue.front().begin<uchar>());
			queue.pop();
		}

		for (int i = 0; i < src.rows; i++)
		{
			meanArray.push_back(std::vector<double>());
			varianceArray.push_back(std::vector<double>());
			for (int k = 0; k < src.cols; k++)
			{
				stdDev(i, k, a);
			}
		}

		initialFrames = true;
	}
	/*if (mean.empty()) {
		src.copyTo(mean);
		variance = cv::Mat::eye(src.size(), src.type()) * 20;
		return;
	}*/

	switch (channels) 
	{
		case 1: 
		{
			int i = 0;
			int k = 0;
			cv::MatIterator_<uchar> it1, end1;
			cv::MatIterator_<uchar> it2 = dst.begin<uchar>();
			//std::vector<std::vector<double>> m;
			//std::vector<std::vector<double>> v;
			for (it1 = src.begin<uchar>(), end1 = src.end<uchar>(); it1 != end1; ++it1) 
			{
				if (k >= src.cols)
				{
					k = 0;
					i++;
				}
				//Определяем принадлежность к переднему/заднему плану
				*it2 = ((abs(meanArray[i][k] - *it1)) / sqrt(varianceArray[i][k]) > DELTA_THRESHOLD) ? 255 : 0;
				//Пересчитываем матожидание
				meanArray[i][k] = (1 - a) * meanArray[i][k] + a * (*it1);
				//Пересчитываем дисперсию
				varianceArray[i][k] = (1 - a) * varianceArray[i][k] + a * (pow(*it1 - meanArray[i][k], 2));
				it2++;
				k++;
			}

			cv::morphologyEx(dst, dst, cv::MORPH_OPEN, kern3);

			break;
		}
	}

}