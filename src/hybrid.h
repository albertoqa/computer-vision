//
//  hybrid.h
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 12/10/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#ifndef __computer_vision__hybrid__
#define __computer_vision__hybrid__

#include "opencv2/opencv.hpp"
using namespace cv;

void imGaussConv(Mat &src, Mat &dst, float sigma);
void GaussFilter(Mat &src, Mat &dst, float sigma, int border_type);
void createGaussKernel(Mat &xk, float sigma);
void createHighLow(Mat &src, Mat &low, Mat &high, float sigma, int border_type);

//void dFilterA(Mat &src, Mat &mask, Mat &dst);

#endif /* defined(__computer_vision__hybrid__) */
