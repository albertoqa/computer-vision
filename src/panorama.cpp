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

Mat harrisPoints(Mat &src) {
    
    if(src.type() != 0)
        cvtColor(src, src, CV_RGB2GRAY);
    
    int levels = 4, blocksize = 10, ksize = 5, num_points = 1000;
    float k = 0.04, hvalue;
    float x, y;
    
    vector<hpoint> hpoints;
    vector<Mat> pyramid = gaussPyramid(src, levels), hmat;
    Mat aux, haux, out;
    hpoint paux;
    src.copyTo(haux);
    
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
                //cout << hvalue << endl;
            }
        }
        
        hmat.push_back(haux);
    }
    
    // en hmat tengo 4 matrices con los puntos harris en cada una de ellas (piramides)
    supNonMax(hmat, 7);
    
    for(int i = 0; i < hmat.size(); i++) {
        for(int j = 0; j < hmat[i].rows; j++) {
            for (int z = 0; z < hmat[i].cols; z++) {
                // una vez hecha la supresión de non maximos, que puntos me quedo?
               // if (hmat[i].at<float>(j, z) == 255) {
                    paux.x = j;
                    paux.y = z;
                    paux.level = i+1;
                    paux.value = hmat[i].at<float>(j, z);
                    hpoints.push_back(paux);
               // }
            }
        }
    }

    //como los distribuyo entre las distintas escalas?
    sort(hpoints.begin(), hpoints.end(), compareValue);

    src.copyTo(out);
    cvtColor(out, out, CV_GRAY2RGB, 3);
    for(int i = 0; i < num_points; i++) {
        circle( out, Point(hpoints[i].x,hpoints[i].y), 4, Scalar(0,0,0), -1, 8, 0);
    }
        
    return out;
}

// preguntar esta función
void supNonMax(vector<Mat> &hmat, int size) {
    
    
    
    
    
}

float harrisValue(float k, float x, float y) {
    
    // Harris = Ix*Iy - k * (Ix+Iy)^2
    
    return ((x*y) - k*pow(x+y, 2));

}








