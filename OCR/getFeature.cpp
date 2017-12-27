#include "getFeature.h"

Mat element = getStructuringElement(MORPH_RECT,
	Size(3, 3),
	Point(1, 1));

cv::Mat getFeature(Mat raw_img)
{	
	//Mat raw_img = imread(fileName,0);
	//imshow("1", raw_img);
	blur(raw_img, raw_img, Size(3, 3));
	//imshow("2", raw_img);
	threshold(raw_img, raw_img, 120, 255, CV_THRESH_OTSU);
	//imshow("3", raw_img);
	Rect _rect = getMinRect(raw_img);
	rectangle(raw_img, _rect, Scalar( 125));
	//imshow("4", raw_img);
	Mat roi_img = raw_img(_rect);
	//imshow("5", roi_img);

	Mat lowData;
	resize(roi_img, lowData, Size(IMAGE_SIZE, IMAGE_SIZE));

	Mat hist = getHistogram(lowData);

	//waitKey();
	return hist;
}

cv::Rect getMinRect(Mat &img)
{
	unsigned short _top, _bottom, _left, _right;
	short cols, rows;
	cols = img.cols;
	rows = img.rows;
	_top = rows;
	_left = cols;
	_bottom = _right = 0;

	bool _flag = false;

	for (int i = 0; i < rows; i++)
	{
		unsigned char* ptr = img.ptr(i);		
		for (int j = 0; j < cols; j++)
		{
			if (ptr[j])
			{
				_flag = true;
				_left = j < _left ? j : _left;
				_right = j > _right ? j : _right;
			}
		}
		if (_flag)
		{
			_top = i < _top ? i : _top;
			_bottom = i > _bottom ? i : _bottom;
		}
	}
	if ((_right > _left)&(_bottom > _top))
	{
		return cv::Rect(_left, _top, _right - _left + 1, _bottom - _top + 1);
	}
	else
	{
		return cv::Rect(0, 0, cols, rows);
	}
}


/***************************
获取水平、垂直方向的直方图
return：水平+垂直  直方图
**************************/
cv::Mat getHistogram(Mat &img)
{
	long hist_h[IMAGE_SIZE] = { 0 };
	long hist_v[IMAGE_SIZE] = { 0 };
	short cols, rows;
	cols = img.cols;
	rows = img.rows;
	for (int i = 0; i < rows; i++)
	{
		unsigned char* ptr = img.ptr(i);
		for (int j = 0; j < cols; j++)
		{
			hist_h[j] += ptr[j];
			hist_v[i] += ptr[j];
		}
	}
	Mat hist(1, 2 * IMAGE_SIZE, CV_32F);
	int hist_cols = hist.cols;
	for (int i = 0; i < IMAGE_SIZE; i++)
	{
		hist.at<float>(i) = (float)(hist_h[i]) / IMAGE_SIZE;
	}
	for (int i = IMAGE_SIZE; i < hist_cols; i++)
	{
		hist.at<float>(i) = (float)(hist_v[i - IMAGE_SIZE]) / IMAGE_SIZE;
	}

	return hist;
}
