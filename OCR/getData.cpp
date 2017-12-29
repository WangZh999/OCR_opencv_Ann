#include"train.h"
#include"getData.h"
#include"getFeature.h"


bool GetData(string path, Mat &trainData, Mat &labelData)
{
	//文件句柄  
	intptr_t	hFile1 = 0;
	intptr_t	hFile2 = 0;

	struct _finddata_t fileinfo;  //存放文件信息;

	string p;
	string subDir;  //子目录路径名称
	string fileName;	//文件目录名称

	int classNum = 0;

	cout << "Get the image feature vectors...." << endl;

	if ((hFile1 = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib == _A_SUBDIR)) //判断是否为文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					classNum = strToInt(fileinfo.name);
					Mat temp_classNum = (Mat_<float>(1, CLASS_NUM));
					for (int i = 0; i < CLASS_NUM; i++)
					{
						if (i == classNum)
						{
							temp_classNum.at<float>(i) = 1.0f;
						}
						else
						{
							temp_classNum.at<float>(i) = 0.0f;
						}
					}
					cout << "Character:    " << fileinfo.name << endl;
					

					hFile2 = 0;
					subDir.assign(path).append("\\").append(fileinfo.name);  //子目录路径


					/*获取子目录下第一个文件句柄*/
					if ((hFile2 = _findfirst(p.assign(subDir).append("\\*").c_str(), &fileinfo)) != -1)
					{
						do
						{
							if ((fileinfo.attrib != _A_SUBDIR))  //文件为图形文件
							{
								fileName.assign(subDir).append("\\").append(fileinfo.name); //文件绝对路径+文件名
								Mat temp_img = imread(fileName, 0);
								if (NULL != temp_img.data)
								{
									Mat temp_feature = getFeature(temp_img); //获取图像特征向量
									trainData.push_back(temp_feature);
									labelData.push_back(temp_classNum);
								}
							}
						} while (_findnext(hFile2, &fileinfo) == 0);  //寻找下一个文件
					}
				}
			}
		} while (_findnext(hFile1, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
	}

	return true;
}



/*********************
0-9: 0-9
10-35: A-Z
*********************/
int strToInt(string str)
{
	int num;
	char c = str.c_str()[0];
	if (c < 'A')
	{
		num = c - '0';
	}
	else
	{
		num = c - 'A' + 10;
	}

	return num;
}