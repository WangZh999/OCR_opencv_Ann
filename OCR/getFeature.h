#pragma once
#ifndef  GETFEATURE_H
#define	 GETFEATURE_H

#define IMAGE_SIZE 16

#include<opencv.hpp>

using namespace cv;
using namespace std;

cv::Mat getFeature(Mat img);
cv::Rect getMinRect(Mat &img);
cv::Mat getHistogram(Mat &img);


#endif // ! GETFEATURE_H