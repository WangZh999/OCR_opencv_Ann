#pragma once

#include<iostream>
#include<string>
#include <fstream>
#include <sstream>
#include <vector>
#include <io.h>
#include<opencv.hpp>


using namespace std;
using namespace cv;

bool GetData(string path, Mat &trainData, Mat &labelData);

int strToInt(string str);
