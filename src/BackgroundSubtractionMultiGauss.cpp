#include "BackgroundSubtractionMultiGauss.h"

BackgroundSubtractionMultiGauss::~BackgroundSubtractionMultiGauss() {}

/*int BackgroundSubtractionMultiGauss::modelWithMinWeight(int y, int x)
{
	if (models[0].at<cv::Vec3f>(y, x)[0] > models[1].at<cv::Vec3f>(y, x)[0])
	{
		return (models[1].at<cv::Vec3f>(y, x)[0] > models[2].at<cv::Vec3f>(y, x)[0]) ? 2 : 1;
	}
	else {
		return (models[0].at<cv::Vec3f>(y, x)[0] > models[2].at<cv::Vec3f>(y, x)[0]) ? 2 : 0;
	}
}*/

/*void BackgroundSubtractionMultiGauss::stepTwo(int y, int x, cv::Mat& dst)
{
	//int minWeight = modelWithMinWeight(y, x);
	int minWeight = (models[0].at<cv::Vec3f>(y, x)[0] > models[1].at<cv::Vec3f>(y, x)[0]) ? 1 : 0;
	status.at<uchar>(y, x) = minWeight;
	if (models[minWeight].at<cv::Vec3f>(y, x)[0] == 0)
	{
		//stepFour(y, x, dst);
		for (int q = 0; q < models.size(); q++) {
			models[q].at<cv::Vec3f>(y, x)[0] = (1 - A_WEIGHT) * models[q].at<cv::Vec3f>(y, x)[0];
		}
		models[status.at<uchar>(y, x)].at<cv::Vec3f>(y, x)[0] += A_WEIGHT;
		//stepFive(y, x, dst);
		if (models[status.at<uchar>(y, x)].at<cv::Vec3f>(y, x)[0] > DELTA_WEIGHT_THRESHOLD)
		{
			dst.at<uchar>(y, x) = 0;
		}
		else {
			dst.at<uchar>(y, x) = 255;
		}
	}
	else {
		//stepFive(y, x, dst);
		if (models[status.at<uchar>(y, x)].at<cv::Vec3f>(y, x)[0] > DELTA_WEIGHT_THRESHOLD)
		{
			dst.at<uchar>(y, x) = 0;
		}
		else {
			dst.at<uchar>(y, x) = 255;
		}
	}
}*/

/*void BackgroundSubtractionMultiGauss::stepThree(int model, uchar u, int y, int x, cv::Mat& dst)
{
	cv::Vec3f v = models[model].at<cv::Vec3f>(y, x);
	v[1] = (1 - A_MEAN) * v[1] + A_MEAN * u;
	v[2] = (1 - A_VARIANCE) * v[2] + A_VARIANCE * (v[1] - u) * (v[1] - u);
	//stepFour(y, x, dst);
	for (int q = 0; q < models.size(); q++) {
		models[q].at<cv::Vec3f>(y, x)[0] = (1 - A_WEIGHT) * models[q].at<cv::Vec3f>(y, x)[0];
	}
	models[status.at<uchar>(y, x)].at<cv::Vec3f>(y, x)[0] += A_WEIGHT;
	//stepFive(y, x, dst);
	if (models[status.at<uchar>(y, x)].at<cv::Vec3f>(y, x)[0] > DELTA_WEIGHT_THRESHOLD)
	{
		dst.at<uchar>(y, x) = 0;
	}
	else {
		dst.at<uchar>(y, x) = 255;
	}
}*/

/*void BackgroundSubtractionMultiGauss::stepFour(int y, int x, cv::Mat& dst)
{
	for (int q = 0; q < models.size(); q++) {
		models[q].at<cv::Vec3f>(y, x)[0] = (1 - A_WEIGHT) * models[q].at<cv::Vec3f>(y, x)[0];
	}
	models[status.at<uchar>(y, x)].at<cv::Vec3f>(y, x)[0] += A_WEIGHT;
	//stepFive(y, x, dst);
	if (models[status.at<uchar>(y, x)].at<cv::Vec3f>(y, x)[0] > DELTA_WEIGHT_THRESHOLD)
	{
		dst.at<uchar>(y, x) = 0;
	}
	else {
		dst.at<uchar>(y, x) = 255;
	}
}*/

/*void BackgroundSubtractionMultiGauss::stepFive(int y, int x, cv::Mat& dst)
{
	if (models[status.at<uchar>(y, x)].at<cv::Vec3f>(y, x)[0] > DELTA_WEIGHT_THRESHOLD) 
	{
		dst.at<uchar>(y, x) = 0;
	}
	else {
		dst.at<uchar>(y, x) = 255;
	}
}*/

void BackgroundSubtractionMultiGauss::Apply(cv::Mat& src, cv::Mat& dst) 
{
	dst = cv::Mat(src.size(), src.type());
	if (models.empty()) {
		for (int q = 0; q < MODELS_COUNT; q++)
		{
			models.push_back(cv::Mat(src.size(), CV_32FC3, cv::Scalar(0, 0, 0)));
		}
		for (int y = 0; y < src.rows; y++)
		{
			for (int x = 0; x < src.cols; x++)
			{
				cv::Vec3f v;
				v[0] = 1;
				v[1] = src.at<uchar>(y, x);
				v[2] = 100;
				models[0].at<cv::Vec3f>(y, x) = v;
				v[0] = 0;
				for (int q = 1; q < models.size(); q++)
				{
					models[q].at<cv::Vec3f>(y, x) = v;
				}
			}
		}
		for (int q = 0; q < models.size(); q++)
		{
			vecModels.push_back(cv::Vec3f());
		}
		return;
	}

	int statusModel = -1;
	
	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			for (int i = 0; i < vecModels.size(); i++)
			{
				vecModels[i] = models[i].at<cv::Vec3f>(y, x);
			}
			for (int q = 0; q < models.size(); q++)
			{
				uchar u = src.at<uchar>(y, x);
				if (vecModels[q][0] != 0 && abs(vecModels[q][1] - u) / sqrt(vecModels[q][2]) <= DELTA_THRESHOLD)
				{
					//stepThree(q, u, y, x, dst);
					statusModel = q;
					vecModels[statusModel][1] = (1 - A_MEAN) * vecModels[statusModel][1] + A_MEAN * u;
					vecModels[statusModel][2] = (1 - A_VARIANCE) * vecModels[statusModel][2] + A_VARIANCE * (vecModels[statusModel][1] - u) * (vecModels[statusModel][1] - u);
					//stepFour(y, x, dst);
					for (int i = 0; i < models.size(); i++) {
						vecModels[i][0] = (1 - A_WEIGHT) * vecModels[i][0];
					}
					vecModels[statusModel][0] += A_WEIGHT;
					break;
				}
				if (q == models.size() - 1) 
				{
					//stepTwo(y, x, dst);
					statusModel = (models[0].at<cv::Vec3f>(y, x)[0] > models[1].at<cv::Vec3f>(y, x)[0]) ? 1 : 0;
					if (vecModels[statusModel][0] == 0)
					{
						//stepFour(y, x, dst);
						for (int i = 0; i < models.size(); i++) {
							vecModels[i][0] = (1 - A_WEIGHT) * vecModels[i][0];
						}
						vecModels[statusModel][0] += A_WEIGHT;
					}
				}
			}

			if (vecModels[statusModel][0] > DELTA_WEIGHT_THRESHOLD)
			{
				dst.at<uchar>(y, x) = 0;
			}
			else {
				dst.at<uchar>(y, x) = 255;
			}

			for (int i = 0; i < vecModels.size(); i++)
			{
				models[i].at<cv::Vec3f>(y, x) = vecModels[i];
			}
		}
	}

	//cv::morphologyEx(dst, dst, cv::MORPH_OPEN, kern3);

}