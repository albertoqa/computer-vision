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
#include "hybrid.h"

using namespace cv;

struct hpoint {
    
    int x, y, level;
    float value;
    //falta orientación
    
};

Mat harrisPoints(Mat &src);
float harrisValue(float k, float x, float y);
void supNonMax(vector<Mat> &hmat, int size, vector<Mat> &auxiliar);

#endif /* defined(__computer_vision__panorama__) */
