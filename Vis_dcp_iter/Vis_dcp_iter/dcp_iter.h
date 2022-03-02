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

	//��Ա������Ϊ���У�������main�в鿴
	Mat _darkS, _traS;					//��ͨ����͸����ͼ
	Mat _src;
	double _airS, _vis;					//�����⣬�ܼ���ֵ

	//���캯��(һ�������캯���Լ�����ģ�麯��)
	dcp_iter(Mat, Mat, Mat);			//���ܼ��ȵ�ֵ������ͼ�����ͼ��Mask��

	void showSrc();						//��ʾԭͼ��
	Mat dcp(Mat);						//��ͨ��
	void showDark();					//��ʾ��ͨ��
	double air(Mat, Mat);				//�������
	void showAir();						//��ʾ������
	Mat tra(Mat, Mat, double);			//��͸����ͼ
	void showTra();						//��ʾ͸����ͼ
	double vis(Mat, Mat, Mat);			//�����ܼ���
	void showVis();						//��ʾ�ܼ��ȵ�ֵ

};


