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
    
    int levels = 4;
    vector<hpoint> hpoints;
    vector<Mat> pyramid = gaussPyramid(src, levels);
    
    Mat aux;
    
    
    for(int i = 0; i < pyramid.size(); i++) {
        
        
        cornerEigenValsAndVecs(pyramid[i], aux, 10, 6);
        
        
    }
    
    
    
    return hpoints;
}