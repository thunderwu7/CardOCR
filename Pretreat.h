#pragma once

#include "Parameter.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Pretreat
{
public:
	Pretreat();
	~Pretreat();

	void CharSegmentation(Mat &inimg, Mat &outimg);
	void GetRegion(Mat &inimg, Mat &outimg);
	int imageAdjust(Mat &src, Mat &dst);
};

