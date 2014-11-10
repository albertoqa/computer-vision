//
//  panorama.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 4/11/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "panorama.h"
#include "aux.h"

bool compareValue(const hpoint &v1, const hpoint &v2) {
    return v1.value > v2.value;
}

float harrisValue(float k, float x, float y) {
    return ((x*y) - k*pow(x+y, 2));
}

Mat harrisPoints(Mat &src) {
    
    Mat src_gray;
    Mat myHarris_dst; Mat myHarris_copy; Mat Mc;
    
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    
    int blockSize = 3; int apertureSize = 3;
    
    myHarris_dst = Mat::zeros( src_gray.size(), CV_32FC(6) );
    Mc = Mat::zeros( src_gray.size(), CV_32FC1 );
    
    cornerEigenValsAndVecs( src_gray, myHarris_dst, blockSize, apertureSize, BORDER_DEFAULT );
    
    for( int j = 0; j < src_gray.rows; j++ ) {
        for( int i = 0; i < src_gray.cols; i++ ) {
            float lambda_1 = myHarris_dst.at<Vec6f>(j, i)[0];
            float lambda_2 = myHarris_dst.at<Vec6f>(j, i)[1];
            Mc.at<float>(j,i) = harrisValue(0.04, lambda_1, lambda_2);
        }
    }
    
    double myHarris_minVal; double myHarris_maxVal;
    minMaxLoc( Mc, &myHarris_minVal, &myHarris_maxVal, 0, 0, Mat() );
    
    cout << myHarris_maxVal;
    
    
    return src_gray;
    
}


