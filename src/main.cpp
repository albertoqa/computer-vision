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
#include "hybrid.h"
#include "panorama.h"

using namespace std;
using namespace cv;


void showIM(Mat &src, string windowName){
    
    namedWindow(windowName, CV_WINDOW_AUTOSIZE);
    imshow(windowName, src);
    waitKey();
    destroyWindow(windowName);
    
}

int main (int argc, const char * argv[])
{
    
    // Image Lecture
    //-------------------------------------------------------------------------------------
    
    Mat src = imread("/Users/alberto/Desktop/dev/computer-vision/src/images/lena.jpg", 1);
    
    if( !src.data ) {
        printf("Error loading src \n"); return -1;
    }
    
    Mat dst;
    //convertToFloat(fm, src);
    //convertToFloat(fm1, src1);
    
    dst = harrisPoints(src);
    //cornerHarris(src, dst, 2, 3, 0.04);
    
    dst.convertTo(dst, CV_8U);
    convertScaleAbs(dst, dst);
    showIM(dst, "harris");
    
}
