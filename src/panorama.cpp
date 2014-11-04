//
//  panorama.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 4/11/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "panorama.h"

vector<hpoint> harrisPoints(Mat &src) {
    
    if(src.type() != 0)
        cvtColor(src, src, CV_RGB2GRAY);
    
    int levels = 4, blocksize = 10, ksize = 6;
    float k = 0.04, hvalue;
    float x, y;
    
    vector<hpoint> hpoints;
    vector<Mat> pyramid = gaussPyramid(src, levels);
    
    Mat aux;
    
    for(int i = 0; i < pyramid.size(); i++) {
        
        aux.zeros(pyramid[i].size(), CV_32FC1);
        
        cornerEigenValsAndVecs(pyramid[i], aux, blocksize, ksize);
        
        for(int j = 0; j < pyramid[i].rows; j++) {
            for (int z = 0; z < pyramid[i].cols; z++) {
                
                x = aux.at<Vec6f>(j, z)[0];
                y = aux.at<Vec6f>(j, z)[1];
                
                hvalue = harrisValue(k, x, y);
                
            }
        }
        
        
        
    }
    
    
    
    return hpoints;
}


float harrisValue(float k, float x, float y) {
    
    // Harris = Ix*Iy - k * (Ix+Iy)^2
    
    return ((x*y) - k*pow(x+y, 2));

}