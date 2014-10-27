//
//  main.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 12/10/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include "blend.h"
#include "aux.h"
#include "hybrid.h"

using namespace std;
using namespace cv;


int main (int argc, const char * argv[])
{
    
    // Image Lecture
    //-------------------------------------------------------------------------------------

	Mat fm = imread("/Users/alberto/Desktop/dev/computer-vision/src/images/cat.bmp", 1);
    if( !fm.data ) { printf("Error loading src \n"); return -1; }

    Mat fm1 = imread("/Users/alberto/Desktop/dev/computer-vision/src/images/dog.bmp", 1);
    if( !fm1.data ) { printf("Error loading src \n"); return -1; }

    Mat src, src1, dst;
    convertToFloat(fm, src);
    convertToFloat(fm1, src1);

    // Gauss Filter
    //-------------------------------------------------------------------------------------

    GaussFilter(src, dst, 3, 0);
    dst.convertTo(dst, CV_8U);
    convertScaleAbs(dst, dst);
    showIM(dst, "dst");
    
    // High and low frecuency + hybrid image
    //-------------------------------------------------------------------------------------
    
    Mat low, high;
    createHighLow(src, src1, low, high, 4, 0);
    
    convertScaleAbs(low, low);
    convertScaleAbs(high, high);
    convertScaleAbs(src, src);
    
    vector<Mat> v;
    v.push_back(low);
    v.push_back(high);
    v.push_back(src);
    
    dst = createOne(v, 5, 0);
    showIM(dst, "dst");

    // Gaussian Pyramid
    //-------------------------------------------------------------------------------------

    v = gaussPyramid(src, 5);
    
    dst = createOne(v, 5, 0);
    dst.convertTo(dst, CV_8UC3);
    convertScaleAbs(dst, dst);
    
    showIM(dst, "input");
    
    //-------------------------------------------------------------------------------------


}
