#pragma once
#ifndef TRAIN_H
#define TRAIN_H

#include<opencv.hpp>

#define CLASS_NUM 10


int train(std::string dataPath);

float test(std::string dataPath);

class ANN_Wz
{
public:
	ANN_Wz();
	~ANN_Wz();

	int predict(cv::Mat img);

private:
	cv::Ptr<cv::ml::ANN_MLP> _Ann;
	cv::Mat _feature_mat;		//图像特征向量集
	cv::Mat _result;

};
#endif // !TRAIN_H
