#include "BackgroundSubtractionBase.h"

BackgroundSubtractionBase::~BackgroundSubtractionBase() {}

void BackgroundSubtractionBase::SetThreshold(int t) 
{
	if (t > 0 && t < 254) 
	{
		this->threshold = t;
	}
}

int BackgroundSubtractionBase::GetThreshold() 
{
	return this->threshold;
}

void BackgroundSubtractionBase::Apply(cv::Mat& src, cv::Mat& dst) 
{

	// accept only char type matrices
	CV_Assert(src.depth() == CV_8U);

	dst = cv::Mat(src.size(), src.type());

	if (frameAverage.empty()) 
	{
		src.copyTo(frameAverage);
		return;
	}

	const int channels = src.channels();

	double a = 0.1;

	switch (channels)
	{
	case 1:
	{
		cv::MatIterator_<uchar> it1, end1;
		cv::MatIterator_<uchar> it2, end2;
		cv::MatIterator_<uchar> itAv, endAv;
		it2 = dst.begin<uchar>();
		itAv = frameAverage.begin<uchar>();
		for (it1 = src.begin<uchar>(), end1 = src.end<uchar>(); it1 != end1; ++it1) 
		{
			int d = abs((*itAv) - (*it1));
			*it2 = (d > threshold) ? 255 : 0;
			it2++;
			itAv++;
		}
		src.copyTo(frameAverage);
		//cv::morphologyEx(dst, dst, cv::MORPH_OPEN, kern3);

		break;
	}
	/*case 3:
	{
	cv::MatIterator_<cv::Vec3b> it, end;
	for (it = dst.begin<cv::Vec3b>(), end = dst.end<cv::Vec3b>(); it != end; ++it)
	{
	(*it)[0] >>= 2;
	(*it)[1] >>= 2;
	//(*it)[2] >>= 2;
	}
	}*/
	}

	//return dst;

}