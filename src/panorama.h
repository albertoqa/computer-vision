//
//  panorama.h
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 4/11/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#ifndef __computer_vision__panorama__
#define __computer_vision__panorama__

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "hybrid.h"

using namespace cv;

struct hpoint {
    
    int x, y, level;
    float value;
    float angle;
    //falta orientación
    
};

Mat harrisPoints(Mat &src);
float harrisValue(float k, float x, float y);
void supNonMax(vector<Mat> &hmat, int size, vector<Mat> &auxiliar);
bool isMax(Mat &m, int x, int y, int size);
void zeroEnv(Mat &m, int x, int y, int size);
void subpixelRef(vector<hpoint> &hpoints, int num_points, vector<Mat> &pyramid);
void drawSIFT(Mat &src);
void drawSURF(Mat &src);

#endif /* defined(__computer_vision__panorama__) */
