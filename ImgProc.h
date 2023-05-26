#pragma once

#include <opencv2/opencv.hpp>

namespace ImgProc
{

	void treshold(cv::Mat I, cv::Mat& O, unsigned int t);
	void absoluteMotion(cv::Mat I, cv::Mat prevI, cv::Mat& O);
	void temporalFiltering(cv::Mat I, cv::Mat& O, float t);
	void convolution(cv::Mat I, cv::Mat& O, float* K);
	void sobelEdge(cv::Mat I, cv::Mat& O, float* kHorizontal, float* kVertical);
	void morphologicalDilation(cv::Mat I, cv::Mat& O, uchar t);
	void morphologicalErosion(cv::Mat I, cv::Mat& O, uchar t);
	void morphologicalEdge(cv::Mat I, cv::Mat& O, uchar t);
	void medianFilter(cv::Mat I, cv::Mat& O);
	void adaptiveTreshold(cv::Mat I, cv::Mat &O, float t);
	void asciify(cv::Mat I, cv::Mat& O, int divider, float fontSize, int thickness=1);
	
	
};

