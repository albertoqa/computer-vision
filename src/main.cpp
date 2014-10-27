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
	Mat fm = imread("/Users/alberto/Desktop/dev/computer-vision/src/images/cat.bmp", 1);
    
    if( !fm.data ) { printf("Error loading src \n"); return -1; }

    Mat fm1 = imread("/Users/alberto/Desktop/dev/computer-vision/src/images/dog.bmp", 1);
    
    if( !fm1.data ) { printf("Error loading src \n"); return -1; }

    Mat src, src1, dst;
    
    convertToFloat(fm, src);
    convertToFloat(fm1, src1);

    //GaussFilter(src, dst, 3, 0);
    
    //Mat low, high;
    //createHighLow(src, src1, low, high, 5, 0);
    
    //convertScaleAbs(low, low);
    //convertScaleAbs(high, high);
    //convertScaleAbs(src, src);

    //Laplacian(src, dst, CV_32F);
    //convertScaleAbs( dst, src );
    
    //dst = createOne(v, 2, 5);
    
    vector<Mat> v;
    
    v = gaussPyramid(src, 5);
    dst = createOne(v, 4, 0);
    dst.convertTo(dst, CV_8UC3);
    //convertScaleAbs(dst, dst);

    showIM(dst, "input");
    //showIM(low, "low");
    //showIM(high, "high");

}
