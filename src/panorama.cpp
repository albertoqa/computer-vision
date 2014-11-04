//
//  panorama.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 4/11/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "panorama.h"

vector<hpoint> harriPoints(Mat &src) {
    
    if(src.type() != 0)
        cvtColor(src, src, CV_RGB2GRAY);
    
    int levels = 4, blocksize = 10, ksize = 6;
    double k = 0.04, hvalue;
    
    vector<hpoint> hpoints;
    vector<Mat> pyramid = gaussPyramid(src, levels);
    
    Mat aux;
    
    for(int i = 0; i < pyramid.size(); i++) {
        
        aux.zeros(pyramid[i].size(), CV_32FC1);
        
        cornerEigenValsAndVecs(pyramid[i], aux, blocksize, ksize);
        hvalue = harriValue(k);
        
    }
    
    
    
    return hpoints;
}


double harriValue(double k) {
    
    double value;
    
    
    
    
    return value;
}