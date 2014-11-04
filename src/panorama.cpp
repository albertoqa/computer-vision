//
//  panorama.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 4/11/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "panorama.h"


vector<hpoint> harryPoints(Mat &src) {
    
    int levels = 4;
    vector<hpoint> hpoints;
    vector<Mat> pyramid = gaussPyramid(src, levels);
    
    
    return hpoints;
}