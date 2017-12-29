#include<opencv.hpp>
#include"train.h"
#include"getData.h"
#include"getFeature.h"

using namespace cv;

int train(string data_path)
{
	Mat train_data_mat;		//图像特征向量集
	Mat labels_mat;			//对应的图像类别向量

	train_data_mat.convertTo(train_data_mat, CV_32F);
	labels_mat.convertTo(labels_mat, CV_32FC1);

	/*BP 模型创建和参数设置*/
	Ptr<ml::ANN_MLP> bp = ml::ANN_MLP::create();

	Mat layers_size = (Mat_<int>(1, 4) << 32, 32, 16, 10); // 2维点，1维输出
	bp->setLayerSizes(layers_size);

	bp->setTrainMethod(ml::ANN_MLP::BACKPROP, 0.1, 0.1);
	bp->setActivationFunction(ml::ANN_MLP::SIGMOID_SYM);
	bp->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 0.01));

	GetData(data_path, train_data_mat, labels_mat);		//获取图像特征向量集、对应的图像类别向量


	/* 保存训练好的神经网络参数*/
	cout << "Start training........." << endl;
	bool trained = bp->train(train_data_mat, ml::ROW_SAMPLE, labels_mat);
	if (trained) {
		cout << "Training is over!!!!!!" << endl;
		bp->save("nn_param.xml");
		return 0;
	}
	else
	{
		cout << "Training failed!!!!!!" << endl;
		return -1;
	}

}

float test(string path)
{
	long correct_num = 0, total_num = 0;
	long cor_num_class = 0, total_num_class = 0;
	float correct_rate = 0.0f;
	ANN_Wz _ann = ANN_Wz();
	int result;

	intptr_t	hFile1 = 0;
	intptr_t	hFile2 = 0;
	struct _finddata_t fileinfo;  //存放文件信息;

	string p;
	string subDir;  //子目录路径名称
	string fileName;	//文件目录名称

	int classNum = 0;

	cout << "Start testing the test set...." << endl;

	if ((hFile1 = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib == _A_SUBDIR)) //判断是否为文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) //存放用于训练的文件夹
				{		
					cor_num_class = 0;
					total_num_class = 0;
					classNum = strToInt(fileinfo.name);	

					hFile2 = 0;
					subDir.assign(path).append("\\").append(fileinfo.name);  //子目录路径																			
					if ((hFile2 = _findfirst(p.assign(subDir).append("\\*").c_str(), &fileinfo)) != -1)		// 获取子目录下第一个文件句柄
					{
						do
						{
							if ((fileinfo.attrib != _A_SUBDIR))  //文件为图形文件
							{								
								fileName.assign(subDir).append("\\").append(fileinfo.name); //文件绝对路径+文件名
								Mat temp_img = imread(fileName, 0);
								if (NULL==temp_img.data){
									continue;
								}
								result = _ann.predict(temp_img);	//根据训练好的神经网络预测结果

								total_num++;
								total_num_class++;
								if (result == classNum)
								{
									correct_num++;
									cor_num_class++;
								}
								else
								{
									cout << fileName << endl;
								}

							}
						} while (_findnext(hFile2, &fileinfo) == 0);  //寻找下一个文件
					}

					correct_rate = float(cor_num_class) / float(total_num_class);//每一类正确率的计算
					cout << "Character:   " << classNum << "     " << correct_rate << endl << endl << endl;

				}
			}
		} while (_findnext(hFile1, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
	}

	correct_rate = float(correct_num) / float(total_num);//每一类正确率的计算
	cout << "Correct Rate:   " << correct_rate << endl;

	return correct_rate;
}

ANN_Wz::ANN_Wz()
{
	/*载入训练好的神经网络参数*/
	_Ann = ml::ANN_MLP::load("nn_param.xml");
	_feature_mat.convertTo(_feature_mat, CV_32F);	//用于存放图片特征向量
	_result = (Mat_<float>(1, CLASS_NUM));	//用于存放预测结果
}


int ANN_Wz::predict(cv::Mat img)
{
	float maxVal = -2;
	int result = 0;
	_feature_mat = getFeature(img);
	_Ann->predict(_feature_mat, _result);	//根据特征求取结果
	for (int j = 0; j < CLASS_NUM; j++) {	//最大隶属度原则
		float val = _result.at<float>(j);
		if (val > maxVal) {
			maxVal = val;
			result = j;
		}
	}
	
	return result;
}


ANN_Wz::~ANN_Wz()
{
}