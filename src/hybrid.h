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
Mat GaussFilter(Mat &src, float sigma, int border_type);
void createGaussKernel(Mat &xk, float sigma);
Mat createHighLow(Mat &src, Mat &src1, Mat &low, Mat &high, float sigma1, float sigma2, int border_type);
vector<Mat> gaussPyramid(Mat &src, int levels);
cv::Mat createOne(std::vector<cv::Mat> & images, int cols, int min_gap_size);


//void dFilterA(Mat &src, Mat &mask, Mat &dst);

#endif /* defined(__computer_vision__hybrid__) */
