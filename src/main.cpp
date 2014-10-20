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
	Mat fm = imread("/Users/alberto/Desktop/dev/computer-vision/src/images/fish.bmp", 1);
    
    if( !fm.data ) { printf("Error loading src \n"); return -1; }

    //Mat src2 = imread("/Users/alberto/Desktop/dev/computer-vision/src/images/dog.bmp");
    Mat src, dst;
    
    convertToFloat(fm, src);


	//if( !src2.data ) { printf("Error loading src2 \n"); return -1; }

    //GaussFilter(src, dst, 3, 0);
    
    Mat low, high;
    createHighLow(src, low, high, 7, 0);
    
    convertScaleAbs(low, low);
    convertScaleAbs(high, high);
    convertScaleAbs(src, src);

    //Laplacian(src, dst, CV_32F);
    //convertScaleAbs( dst, src );

    showIM(src, "input");
    showIM(low, "low");
    showIM(high, "high");

}
