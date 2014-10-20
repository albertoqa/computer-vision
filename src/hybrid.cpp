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


int border(int M, int x, int border_type)
{

	if(border_type == 0) { // reflected
		if(x < 0) {
			return -x - 1;
		}
		if(x >= M) {
			return 2*M - x - 1;
		}

		return x;
	}
    else {	// uniform to 0
        if(x < 0 || x >= M)
            return 0;
    
        return x;
    }

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
					y1 = border(m[rgb].rows, y - i, border_type);
					sum = sum + xxk[i]*m[rgb].at<float>(y1, x);
				}
				tmp.at<float>(y,x) = sum;
			}
		}

		for(int y = 0; y < m[rgb].rows; y++) {
			for(int x = 0; x < m[rgb].cols; x++) {
				sum = 0.0;
				for(int i = 0; i < ss; i++){
					x1 = border(m[rgb].cols, x - i, border_type);
					sum = sum + xxk[i]*tmp.at<float>(y, x1);
				}
				m[rgb].at<float>(y,x) = sum;
			}
		}

	}

	merge(m, dst);

}

//create a gauss kernel based on sigma (input)
//the kernel is saved in the matrix xk
void createGaussKernel(Mat &xk, float sigma) {

	int size = (sigma * 6) + 1;

	xk.create(size, 1, CV_32F);
	float *xxk = xk.ptr<float>();

	float sum = 0, x, pos;

	for(int i = 0; i < size; i++) {

		pos = i - ((size-1)/2);
		x = exp((-0.5/(sigma*sigma))*pos*pos);

		xxk[i] = x;
        
        sum = sum + x;

	}

	sum = 1.0/sum;

	for(int i = 0; i < size; i++ )
		xxk[i] = xxk[i] * sum;

}

void createHighLow(Mat &src, Mat &low, Mat &high, float sigma, int border_type) {
    //low.create(src.size(), src.type());
    GaussFilter(src, low, sigma, border_type);
    
    high.create(src.size(), src.type());
    high = src.clone();
    
    high = high - low;
    
    
    
    
    
    
}


