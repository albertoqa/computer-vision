//
//  hybrid.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 12/10/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "hybrid.h"
#include "aux.h"

void imGaussConv(Mat &src, Mat &dst, float sigma) {
	
	int border_type = 0; // 0 uniforme a 0; 1 reflejada
	GaussFilter(src, dst, sigma, border_type);
	
}


int reflect(int M, int x)
{
	if(x < 0)
	{
		return -x - 1;
	}
	if(x >= M)
	{
		return 2*M - x - 1;
	}
	
	return x;
}

void GaussFilter(Mat &src, Mat &dst, float sigma, int border_type) {
	
	dst.create(src.size(), src.type());
	Mat tmp (src.size(), src.type());
	float sum, x1, y1;
	
	Mat xk;
	createGaussKernel(xk, sigma);
	
	float *xxk = xk.ptr<float>();
	
	int ss = (sigma * 6) + 1;
	int channels = src.channels();
	//cout << channels;
	
	vector<Mat> m;
	split(src, m);
	
	for(int rgb = 0; rgb < channels; rgb++) {
	
		for(int y = 0; y < m[rgb].rows; y++) {
			for(int x = 0; x < m[rgb].cols; x++) {
				sum = 0.0;
				for(int i = 0; i < ss; i++){
					y1 = reflect(m[rgb].rows, y - i);
					sum = sum + xxk[i]*m[rgb].at<uchar>(y1, x);
				}
				tmp.at<uchar>(y,x) = sum;
			}
		}
		
		for(int y = 0; y < m[rgb].rows; y++) {
			for(int x = 0; x < m[rgb].cols; x++) {
				sum = 0.0;
				for(int i = 0; i < ss; i++){
					x1 = reflect(m[rgb].cols, x - i);
					sum = sum + xxk[i]*tmp.at<uchar>(y, x1);
				}
				m[rgb].at<uchar>(y,x) = sum;
			}
		}
	
	}
	
	merge(m, dst);
	
	
}

void createGaussKernel(Mat &xk, float sigma) {
	
	int size = (sigma * 6) + 1;
	
	xk.create(size, 1, CV_32F);
	float *xxk = xk.ptr<float>();
	
	float sum = 0, x, pos;
	
	for(int i = 0; i < size; i++) {
		
		pos = i - ((size-1)/2);
		x = exp((-0.5/(sigma*sigma))*pos*pos);
		
		xxk[i] = x; // no se porque con Xcode no funciona, pero compilando normal si
		sum = sum + x;
		
	}
	
	sum = 1.0/sum;
	
	for(int i = 0; i < size; i++ )
		xxk[i] = xxk[i] * sum;
	
}

