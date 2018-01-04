#include "getFeature.h"

Mat element = getStructuringElement(MORPH_RECT,
	Size(3, 3),
	Point(1, 1));

cv::Mat getFeature(Mat raw_img)
{	
	//模糊，滤波
	blur(raw_img, raw_img, Size(3, 3));

	//阈值分割
	threshold(raw_img, raw_img, 120, 255, CV_THRESH_OTSU);

	//寻找最小外接矩，得到感兴趣区域
	Rect _rect = getMinRect2(raw_img);
	Mat roi_img = raw_img(_rect);

	//将图片缩放只指定大小
	Mat lowData;
	resize(roi_img, lowData, Size(IMAGE_SIZE, IMAGE_SIZE));

	//直方图特征
	Mat hist = getHistogram(lowData);

	resize(lowData, lowData, Size(4, 4));

	Mat _feature(1, 2 * IMAGE_SIZE + 16, CV_32F);

	for (int i = 0; i < (2 * IMAGE_SIZE); i++)
	{
		_feature.at<float>(i) = hist.at<float>(i);
	}
	for (int i = 0; i < 4; i++)
	{
		unsigned char *ptr = lowData.ptr(i);
		for (int j = 0; j < 4; j++)
		{
			_feature.at<float>(2 * IMAGE_SIZE + i * 4 + j) = (float)ptr[j];
		}
	}
	return _feature;
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


cv::Rect getMinRect2(Mat &img)
{
	short cols, rows;
	unsigned short _top, _bottom, _left, _right;
	cols = img.cols;
	rows = img.rows;
	_bottom = rows;
	_right = cols;
	_top = _left = 0;

	float _value = 0.0f;

	Mat hist = getHistogram2(img);

	//由左至右第一个大于阈值的坐标作为左边界
	for (int i = 0; i < cols; i++)
	{
		_value = hist.at<float>(i);
		if (_value < _THRESHOLD)	{
			//_left = _left > i ? _left : i;
			_left = i;
		}
		else {
			break;
		}
	}

	//由右至左第一个大于阈值的坐标作为右边界
	for (int i = cols - 1; i >= 0; i--)
	{
		_value = hist.at<float>(i);
		if (_value < _THRESHOLD)	{
			//_right = _right < i ? _right : i;
			_right = i;
		}
		else {
			break;
		}
	}

	//由上至下第一个大于阈值的坐标作为上边界
	for (int i = cols; i < rows + cols; i++)
	{
		_value = hist.at<float>(i);
		if (_value < _THRESHOLD)	{
			//_top = _top > (i - cols) ? _top : (i - cols);
			_top = i - cols;
		}
		else {
			break;
		}
	}

	//由下至上第一个大于阈值的坐标作为下边界
	for (int i = rows + cols - 1; i >= cols; i--)
	{
		_value = hist.at<float>(i);
		if (_value < _THRESHOLD)	{
			//_bottom = _bottom < (i - cols) ? _bottom : (i - cols);
			_bottom = i - cols;
		}
		else {
			break;
		}
	}



	if ((_right > _left)&(_bottom > _top))
	{
		return cv::Rect(_left, _top, _right - _left, _bottom - _top);
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


/***************************
获取水平、垂直方向的直方图(非固定大小图片，最大1000*1000）
return：水平+垂直  直方图
**************************/
cv::Mat getHistogram2(Mat &img)
{
	long hist_h[1000] = { 0 };
	long hist_v[1000] = { 0 };
	short cols, rows;
	cols = img.cols;
	rows = img.rows;

	if ((cols > 1000) || (rows > 1000))
	{
		return Mat();
	}

	for (int i = 0; i < rows; i++)
	{
		unsigned char* ptr = img.ptr(i);
		for (int j = 0; j < cols; j++)
		{
			hist_h[j] += ptr[j];
			hist_v[i] += ptr[j];
		}
	}
	Mat hist(1, cols + rows, CV_32F);
	int hist_cols = hist.cols;
	for (int i = 0; i < cols; i++)
	{
		hist.at<float>(i) = (float)(hist_h[i]) / cols;
	}
	for (int i = cols; i < rows + cols; i++)
	{
		hist.at<float>(i) = (float)(hist_v[i - cols]) / rows;
	}

	return hist;
}

