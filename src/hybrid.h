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
void GaussFilter(InputArray &src, OutputArray &dst, float sigma, int border_type);
void createGaussKernel(Mat &xk, float sigma);


//void dFilterA(Mat &src, Mat &mask, Mat &dst);

#endif /* defined(__computer_vision__hybrid__) */
