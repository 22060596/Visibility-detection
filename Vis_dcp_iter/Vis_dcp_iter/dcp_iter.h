#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

class dcp_iter
{
private:



public:
	dcp_iter();
	~dcp_iter();

	//成员都设置为公有，方便在main中查看
	Mat _darkS, _traS;					//暗通道，透射率图
	Mat _src;
	double _airS, _vis;					//大气光，能见度值

	//构造函数(一个主构造函数以及若干模块函数)
	dcp_iter(Mat, Mat, Mat);			//求能见度的值（待测图，深度图，Mask）

	void showSrc();						//显示原图像
	Mat dcp(Mat);						//求暗通道
	void showDark();					//显示暗通道
	double air(Mat, Mat);				//求大气光
	void showAir();						//显示大气光
	Mat tra(Mat, Mat, double);			//求透射率图
	void showTra();						//显示透射率图
	double vis(Mat, Mat, Mat);			//计算能见度
	void showVis();						//显示能见度的值

};


