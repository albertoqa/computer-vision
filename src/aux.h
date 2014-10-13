//
//  aux.h
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 12/10/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#ifndef __computer_vision__aux__
#define __computer_vision__aux__

#include <iostream>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

void showIM(Mat &src, string windowName);
void flipIM(Mat &src, OutputArray _dst, int mode);



void flipV(Mat &src, Mat &dst, size_t tt);
void flipH(const uchar* src, uchar* dst, size_t sstep, size_t dstep, Size size, size_t tt);

#endif /* defined(__computer_vision__aux__) */
