//
//  panorama.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 4/11/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "panorama.h"

Mat harrisPoints(Mat &src) {
    
    if(src.type() != 0)
        cvtColor(src, src, CV_RGB2GRAY);
    
    int levels = 4, blocksize = 10, ksize = 6;
    float k = 0.04, hvalue;
    float x, y;
    
    vector<hpoint> hpoints;
    vector<Mat> pyramid = gaussPyramid(src, levels), hmat;
    Mat aux, haux, out;
    hpoint paux;
    
    for(int i = 0; i < pyramid.size(); i++) {
        
        aux.zeros(pyramid[i].size(), CV_32FC1);
        haux.zeros(pyramid[i].size(), CV_32FC1);
        
        cornerEigenValsAndVecs(pyramid[i], aux, blocksize, ksize);
        
        for(int j = 0; j < pyramid[i].rows; j++) {
            for (int z = 0; z < pyramid[i].cols; z++) {
                
                x = aux.at<Vec6f>(j, z)[0];
                y = aux.at<Vec6f>(j, z)[1];
                
                hvalue = harrisValue(k, x, y);
                haux.at<float>(j,z) = hvalue;
            }
        }
        
        hmat.push_back(haux);
    }
    
    // falta supresión de non-máximos
    
    
    for(int i = 0; i < 1000; i++) {
        
        hpoints.push_back(<#const_reference __x#>);
    }
    
    
    
    src.copyTo(out);
    cvtColor(out, out, CV_GRAY2RGB, 3);
    for(int i = 0; i < hpoints.size(); i++) {
        //circle( out, Point(i,j), 4, Scalar( rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255) ), -1, 8, 0 ); }
    }
        
    return out;
}


float harrisValue(float k, float x, float y) {
    
    // Harris = Ix*Iy - k * (Ix+Iy)^2
    
    return ((x*y) - k*pow(x+y, 2));

}