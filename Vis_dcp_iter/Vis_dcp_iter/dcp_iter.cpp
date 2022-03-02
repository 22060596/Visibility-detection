#include "dcp_iter.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

dcp_iter::dcp_iter()
{
}

//计算所需参数
Mat dcp_iter::dcp(Mat src) {
	Mat darkChannel = Mat::zeros(src.rows, src.cols, CV_8UC1);	//初始化暗通道，所有像素赋值为0
	Vec3b intensity;											//定义存放rgb颜色强度的变量
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			intensity = src.at<Vec3b>(i, j);                    //把每个像素点的强度矩阵化
			darkChannel.at<uchar>(i, j) = min(min(intensity.val[0], intensity.val[1]), intensity.val[2]);	//将rgb中最小值存入矩阵
		}
	}
	//Mat dst;
	//Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
	//erode(darkChannel, dst, kernel, Point(-1, -1), 1);
	return darkChannel;
}

double dcp_iter::air(Mat dark, Mat src){
	int A;
	Mat temp;
	dark.copyTo(temp);
	vector<Point> darkMaxPoint;
	vector<uchar> darkMax;

	for (int i = 0; i < (src.rows * src.cols / 1000); i++)					//遍历每一个图像像素（这样会拖慢计算速度）
	{																		//实际上只需要找总像素数/1000次	
		double minVal = 0;
		double maxVal = 0;
		Point minLoc;
		Point maxLoc;
		minLoc.x = 0;
		minLoc.y = 0;
		maxLoc.x = 0;
		maxLoc.y = 0;
		minMaxLoc(temp, &minVal, &maxVal, &minLoc, &maxLoc);

		darkMaxPoint.push_back(maxLoc);                             //将大气光照最大值的位置返回darkMaxPoint
																	//cout << maxLoc << endl;                                   //输出最大光强的地址
		darkMax.push_back(dark.at<uchar>(maxLoc));                 //将大气光照最大值返回darkMax
																   //cout << image.at<uchar>(maxLoc) << endl;                  //输出最大光强
		circle(temp, maxLoc, 3, Scalar(0), -1, 8, 0);				//每找到一个最大值，将该点与局部"涂黑"，以便寻找次大值的点
		temp.at<uchar>(maxLoc) = temp.at<uchar>(minLoc);			//最大值的坐标就变成了最小值坐标
	}																//循环结束可以得到一个储存了亮度最大的0.1%的值与坐标两个数组
	int sum = 0;
	for (int i = 0; i < darkMax.size() - 1; i++)
	{
		sum += darkMax[i + 1];
	}
	A = sum / darkMax.size();										//计算大气光强的值
	return A;
}

Mat dcp_iter::tra(Mat image, Mat darkChannel, double A) {
	int count = 0;
	float w = 0.95;
	float t;
	uchar intensity;											//存储原图像某像素点处强度的变量
	Mat tra = Mat::zeros(image.rows, image.cols, CV_32FC1);		//初始化32位透射率图像
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			//tra.at<float>(i, j) = 1 - w * (darkChannel.at<uchar>(i, j) / A);		//这个式子有问题，导致很多点结果为1，因此设置intensity与t变量进行过渡
			intensity = darkChannel.at<uchar>(i, j);
			t = 1 - (w * intensity / A);
			tra.at<float>(i, j) = t;
			//tra.at<float>(i, j) = t * image.rows / (1.35 * i);
		}
	}
	return tra;
}

double dcp_iter::vis(Mat tra, Mat dep, Mat m) {
	double result = -1;
	Mat T = tra(Range(200, 800), Range(0, 400));
	int row = T.rows;
	int col = T.cols;
	int flag = 0;
	int count = 0;
	float sum = 0;
	float sigma = 0;
	float t = 0;
	float d = 1;
	for (int i = 0; i < row; i++)		//求局部透射率的总和
	{
		for (int j = 0; j < col; j++)
		{
			flag = m.at<uchar>(i, j);
			d = dep.at<float>(i, j);
			if (flag > 0) {
				t = T.at<float>(i, j);
				sigma = -log(t) / float(d);
				sum += sigma;
				count++;
			}
		}
	}
	sigma = sum / count;
	result = 3.912 / sigma;
	return result;
}


//显示部分
void dcp_iter::showSrc()
{
	namedWindow("原图像", WINDOW_KEEPRATIO);
	imshow("原图像", _src);					//显示待测图像暗通道
}

void dcp_iter::showDark()
{
	namedWindow("待测图像暗通道", WINDOW_KEEPRATIO);
	imshow("待测图像暗通道", _darkS);					//显示待测图像暗通道
}

void dcp_iter::showAir()
{
	cout << "待测图像大气光的强度为:" << _airS << endl;
}

void dcp_iter::showTra()
{
	namedWindow("待测图像透射率图", WINDOW_KEEPRATIO);
	imshow("待测图像透射率图", _traS);					//显示待测图像透射率图
}

void dcp_iter::showVis()
{
	cout << "待测图像的能见度值为:" << _vis << "米" << endl;
}

//主模块（求解能见度）
dcp_iter::dcp_iter(Mat src, Mat depth, Mat mask)
{
	_src = src;
	_darkS = dcp(src);
	_airS = air(_darkS, src);
	_traS = tra(src, _darkS, _airS);
	_vis = vis(_traS, depth, mask);
}

dcp_iter::~dcp_iter()
{
}
