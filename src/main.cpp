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


int main (int argc, const char * argv[])
{
    
    // Image Lecture
    //-------------------------------------------------------------------------------------
    
    Mat src = imread("/Users/alberto/Desktop/dev/computer-vision/src/images/Yosemite1.jpg", 1);
    Mat src1 = imread("/Users/alberto/Desktop/dev/computer-vision/src/images/Yosemite2.jpg", 1);

    if( !src.data || !src1.data ) {
        printf("Error loading src \n"); return -1;
    }
    
    Mat dst;
    //convertToFloat(fm, src);
    //convertToFloat(fm1, src1);
    
    //dst = harrisPoints(src);

    //drawSIFT(src);
    //drawSURF(src);
    
    //matchesSurf(src, src1);
    //matchesSift(src, src1);
    
    mosaic(src, src1);

    
}
