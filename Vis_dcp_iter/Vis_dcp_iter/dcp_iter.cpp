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

//�����������
Mat dcp_iter::dcp(Mat src) {
	Mat darkChannel = Mat::zeros(src.rows, src.cols, CV_8UC1);	//��ʼ����ͨ�����������ظ�ֵΪ0
	Vec3b intensity;											//������rgb��ɫǿ�ȵı���
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			intensity = src.at<Vec3b>(i, j);                    //��ÿ�����ص��ǿ�Ⱦ���
			darkChannel.at<uchar>(i, j) = min(min(intensity.val[0], intensity.val[1]), intensity.val[2]);	//��rgb����Сֵ�������
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

	for (int i = 0; i < (src.rows * src.cols / 1000); i++)					//����ÿһ��ͼ�����أ����������������ٶȣ�
	{																		//ʵ����ֻ��Ҫ����������/1000��	
		double minVal = 0;
		double maxVal = 0;
		Point minLoc;
		Point maxLoc;
		minLoc.x = 0;
		minLoc.y = 0;
		maxLoc.x = 0;
		maxLoc.y = 0;
		minMaxLoc(temp, &minVal, &maxVal, &minLoc, &maxLoc);

		darkMaxPoint.push_back(maxLoc);                             //�������������ֵ��λ�÷���darkMaxPoint
																	//cout << maxLoc << endl;                                   //�������ǿ�ĵ�ַ
		darkMax.push_back(dark.at<uchar>(maxLoc));                 //�������������ֵ����darkMax
																   //cout << image.at<uchar>(maxLoc) << endl;                  //�������ǿ
		circle(temp, maxLoc, 3, Scalar(0), -1, 8, 0);				//ÿ�ҵ�һ�����ֵ�����õ���ֲ�"Ϳ��"���Ա�Ѱ�Ҵδ�ֵ�ĵ�
		temp.at<uchar>(maxLoc) = temp.at<uchar>(minLoc);			//���ֵ������ͱ������Сֵ����
	}																//ѭ���������Եõ�һ����������������0.1%��ֵ��������������
	int sum = 0;
	for (int i = 0; i < darkMax.size() - 1; i++)
	{
		sum += darkMax[i + 1];
	}
	A = sum / darkMax.size();										//���������ǿ��ֵ
	return A;
}

Mat dcp_iter::tra(Mat image, Mat darkChannel, double A) {
	int count = 0;
	float w = 0.95;
	float t;
	uchar intensity;											//�洢ԭͼ��ĳ���ص㴦ǿ�ȵı���
	Mat tra = Mat::zeros(image.rows, image.cols, CV_32FC1);		//��ʼ��32λ͸����ͼ��
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			//tra.at<float>(i, j) = 1 - w * (darkChannel.at<uchar>(i, j) / A);		//���ʽ�������⣬���ºܶ����Ϊ1���������intensity��t�������й���
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
	for (int i = 0; i < row; i++)		//��ֲ�͸���ʵ��ܺ�
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


//��ʾ����
void dcp_iter::showSrc()
{
	namedWindow("ԭͼ��", WINDOW_KEEPRATIO);
	imshow("ԭͼ��", _src);					//��ʾ����ͼ��ͨ��
}

void dcp_iter::showDark()
{
	namedWindow("����ͼ��ͨ��", WINDOW_KEEPRATIO);
	imshow("����ͼ��ͨ��", _darkS);					//��ʾ����ͼ��ͨ��
}

void dcp_iter::showAir()
{
	cout << "����ͼ��������ǿ��Ϊ:" << _airS << endl;
}

void dcp_iter::showTra()
{
	namedWindow("����ͼ��͸����ͼ", WINDOW_KEEPRATIO);
	imshow("����ͼ��͸����ͼ", _traS);					//��ʾ����ͼ��͸����ͼ
}

void dcp_iter::showVis()
{
	cout << "����ͼ����ܼ���ֵΪ:" << _vis << "��" << endl;
}

//��ģ�飨����ܼ��ȣ�
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
