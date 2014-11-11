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

void supNonMax(Mat &Mc, Mat &binary_maximum, int window) {
    
    if(window%2 == 0)
        window++;
    if(window < 3)
        window = 3;
    
    int centrox, centroy;
    centrox = window/2;
    centroy = window/2;
    
    int tamx = Mc.rows - window/2;
    int tamy = Mc.cols - window/2;
    
    //cout << "Columnas: " << Mc.cols << endl;
    //cout << "Filas: " << Mc.rows << endl;
    
    for(int i = centrox; i < tamx; i++) {
        for(int j = centroy; j < tamy; j++) {
            
            if(binary_maximum.at<float>(i,j) == 255) {
                //cout << i << "  " << j << endl;
                bool max = false;
                double max_val;
                
                //crear ROI para pasarselo a minmaxloc
                // error found!!! change Mc for binary_maximum
                Mat ROI_Mc = Mc( Rect(j-window/2, i-window/2, window, window) );
                Mat ROI_bin = binary_maximum( Rect(j-window/2, i-window/2, window, window) );
                minMaxLoc(ROI_Mc, NULL, &max_val);
                
                if(float(max_val) == Mc.at<float>(i,j))
                    max = true;
                
                if(max) {
                    //ponerl su alrededor a 0
                    ROI_bin = 0.0;
                    ROI_bin.at<float>(centrox,centroy) = 255;
                }
                else {
                    binary_maximum.at<float>(i,j) = 0;
                }
            }
        }
    }
    
    //tengo que poner a cero los bordes
    for(int i = 0; i < window/2; i++)
        for(int j = 0; j < binary_maximum.cols; j++)
            binary_maximum.at<float>(i,j) = 0;
    for(int i = binary_maximum.rows-window/2; i < binary_maximum.rows; i++)
        for(int j = 0; j < binary_maximum.cols; j++)
            binary_maximum.at<float>(i,j) = 0;
    for(int i = 0; i < window/2; i++)
        for(int j = 0; j < binary_maximum.rows; j++)
            binary_maximum.at<float>(j,i) = 0;
    for(int i = binary_maximum.cols-window/2; i < binary_maximum.cols; i++)
        for(int j = 0; j < binary_maximum.rows; j++)
            binary_maximum.at<float>(j,i) = 0;
    
}


Mat harrisPoints(Mat &src) {
    
    Mat src_gray, myHarris_dst, myHarris_copy, Mc, out;
    vector<hpoint> hpoints;
    hpoint paux;
    
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    
    int levels = 0, blocksize = 3, ksize = 5, num_points = 1000;
    float k = 0.04;
    
    myHarris_dst = Mat::zeros( src_gray.size(), CV_32FC(6) );
    Mc = Mat::zeros( src_gray.size(), CV_32FC1 );
    
    cornerEigenValsAndVecs( src_gray, myHarris_dst, blocksize, ksize, BORDER_DEFAULT );
    
    for( int j = 0; j < src_gray.rows; j++ ) {
        for( int i = 0; i < src_gray.cols; i++ ) {
            float lambda_1 = myHarris_dst.at<Vec6f>(j, i)[0];
            float lambda_2 = myHarris_dst.at<Vec6f>(j, i)[1];
            Mc.at<float>(j,i) = harrisValue(k, lambda_1, lambda_2);
        }
    }
    
    double myHarris_minVal; double myHarris_maxVal;
    minMaxLoc( Mc, &myHarris_minVal, &myHarris_maxVal, 0, 0, Mat() );
    
    
    ////////////////////////////////////////////
    
    Mat binary_maximum (Mc.rows, Mc.cols, Mc.type(), Scalar::all(255));
    supNonMax(Mc, binary_maximum, 7);
    
    for(int i = 0; i < binary_maximum.rows; i++) {
        for(int j = 0; j < binary_maximum.cols; j++) {
            if(binary_maximum.at<float>(i,j) == 255) {
                paux.x = i;
                paux.y = j;
                paux.value = Mc.at<float>(i,j);
                paux.level = 1;
                hpoints.push_back(paux);
            }
        }
    }
    
    sort(hpoints.begin(), hpoints.end(), compareValue);

    for(int i = 0; i < hpoints.size(); i++)
        cout << hpoints[i].value << "   ";
    
    src_gray.copyTo(out);
    cvtColor(out, out, CV_GRAY2RGB, 3);
    for(int i = 0; i < num_points; i++) {
        circle( out, Point(hpoints[i].y * hpoints[i].level,hpoints[i].x * hpoints[i].level), 4, Scalar(0,0,0), -1, 8, 0);
    }
    
    
        return out;
    
}



















/*
void supNonMax(vector<Mat> &hmat, int size, vector<Mat> &auxiliar) {
    
    //int x = size/2, y = size/2;
    
    for(int i = 0; i < hmat.size(); i++) {
        for(int j = 0; j < hmat[i].rows; j++) {
            for (int z = 0; z < hmat[i].cols; ) {
                if(auxiliar[i].at<float>(j,z) == 255) {
                    if(isMax(hmat[i], j, z , size)){
                        //poner a cero el entorno
                        zeroEnv(auxiliar[i], size, j, z);
                        z = z + size/2;
                    }
                    else {
                        auxiliar[i].at<float>(j,z) = 0;
                        //j++;
                        z++;
                    }
                }
                else
                    z++;
            }
        }
    }
    
}

void zeroEnv(Mat &m, int x, int y, int size){
    int a, b, c, d;
    
    if(x - size/2 < 0)
        a = 0;
    else
        a = x-size/2;
    if(y - size/2 < 0)
        b = 0;
    else
        b = y-size/2;
    
    if(x + size/2 >= m.rows)
        c = m.rows;
    else
        c = x+size/2;
    if(y + size/2 >= m.cols)
        d = m.cols;
    else
        d = y+size/2;
    
    for(int i = a; i < c; i++) {
        for(int j = b; j < d; j++) {
            m.at<float>(i,j) = 0;
        }
    }
    
    m.at<float>(x,y) = 255;
    
}

bool isMax(Mat &m, int x, int y, int size) {
    
    bool max = false;
    int a, b, c, d;
    
    float maximum = 0;
    if(x - size/2 < 0)
        a = 0;
    else
        a = x-size/2;
    if(y - size/2 < 0)
        b = 0;
    else
        b = y-size/2;
    
    if(x + size/2 >= m.rows)
        c = m.rows;
    else
        c = x+size/2;
    if(y + size/2 >= m.cols)
        d = m.cols;
    else
        d = y+size/2;
    
    
    for(int i = a; i < c; i++) {
        for(int j = b; j < d; j++) {
            if(m.at<float>(i,j) > maximum)
                maximum = m.at<float>(i,j);
        }
    }
    
    if(maximum == m.at<float>(x,y)) {
        max = true;
    }
    
    
    return max;
}


*/

/*
 
 
 float hvalue;
 float x, y;
 
 vector<hpoint> hpoints;
 vector<Mat> pyramid = gaussPyramid(src, levels), hmat;
 Mat aux, haux, out;
 hpoint paux;
 src.copyTo(haux);
 
 for(int i = 0; i < pyramid.size(); i++) {
 
 aux.zeros(pyramid[i].size(), CV_32FC1);
 haux.zeros(pyramid[i].size(), CV_32FC1);
 
 
  }
 }
 
 hmat.push_back(haux);
 }
 
 vector<Mat> auxiliar;
 
 for(int i = 0; i < pyramid.size(); i++) {
 Mat aux (pyramid[i].rows, pyramid[i].cols, CV_32FC1, 255);
 //aux.zeros(pyramid[i].size(), CV_32FC1);
 auxiliar.push_back(aux);
 }
 
 // en hmat tengo 4 matrices con los puntos harris en cada una de ellas (piramides)
 supNonMax(hmat, 7, auxiliar);
 
 for(int i = 0; i < hmat.size(); i++) {
 for(int j = 0; j < hmat[i].rows; j++) {
 for (int z = 0; z < hmat[i].cols; z++) {
 if (auxiliar[i].at<float>(j, z) == 255) {
 paux.x = j;
 paux.y = z;
 paux.level = i+1;
 paux.value = hmat[i].at<float>(j, z);
 hpoints.push_back(paux);
 }
 }
 }
 }
 
 //como los distribuyo entre las distintas escalas?
 sort(hpoints.begin(), hpoints.end(), compareValue);
 
 src.copyTo(out);
 //cvtColor(out, out, CV_GRAY2RGB, 3);
 for(int i = 0; i < num_points; i++) {
 circle( out, Point(hpoints[i].x * hpoints[i].level,hpoints[i].y * hpoints[i].level), 4, Scalar(0,0,0), -1, 8, 0);
 }
 
 return out;
 }
 
 // preguntar esta función
 void supNonMax(vector<Mat> &hmat, int size, vector<Mat> &auxiliar) {
 
 //int x = size/2, y = size/2;
 
 for(int i = 0; i < hmat.size(); i++) {
 for(int j = 0; j < hmat[i].rows; j++) {
 for (int z = 0; z < hmat[i].cols; ) {
 if(auxiliar[i].at<float>(j,z) == 255) {
 if(isMax(hmat[i], j, z , size)){
 //poner a cero el entorno
 zeroEnv(auxiliar[i], size, j, z);
 z = z + size/2;
 }
 else {
 auxiliar[i].at<float>(j,z) = 0;
 //j++;
 z++;
 }
 }
 else
 z++;
 }
 }
 }
 
 }
 
 void zeroEnv(Mat &m, int x, int y, int size){
 int a, b, c, d;
 
 if(x - size/2 < 0)
 a = 0;
 else
 a = x-size/2;
 if(y - size/2 < 0)
 b = 0;
 else
 b = y-size/2;
 
 if(x + size/2 >= m.rows)
 c = m.rows;
 else
 c = x+size/2;
 if(y + size/2 >= m.cols)
 d = m.cols;
 else
 d = y+size/2;
 
 for(int i = a; i < c; i++) {
 for(int j = b; j < d; j++) {
 m.at<float>(i,j) = 0;
 }
 }
 
 m.at<float>(x,y) = 255;
 
 }
 
 bool isMax(Mat &m, int x, int y, int size) {
 
 bool max = false;
 int a, b, c, d;
 
 float maximum = 0;
 if(x - size/2 < 0)
 a = 0;
 else
 a = x-size/2;
 if(y - size/2 < 0)
 b = 0;
 else
 b = y-size/2;
 
 if(x + size/2 >= m.rows)
 c = m.rows;
 else
 c = x+size/2;
 if(y + size/2 >= m.cols)
 d = m.cols;
 else
 d = y+size/2;
 
 
 for(int i = a; i < c; i++) {
 for(int j = b; j < d; j++) {
 if(m.at<float>(i,j) > maximum)
 maximum = m.at<float>(i,j);
 }
 }
 
 if(maximum == m.at<float>(x,y)) {
 max = true;
 }
 
 
 return max;
 }
 
 */


