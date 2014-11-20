//
//  panorama.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 4/11/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "panorama.h"
#include "aux.h"


void showIM(Mat &src, string windowName){
    
    namedWindow(windowName, CV_WINDOW_AUTOSIZE);
    imshow(windowName, src);
    waitKey();
    destroyWindow(windowName);
    
}

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
    
    ////////////////////////////////////////////
    
    for(int i = centrox; i < tamx; i++) {
        for(int j = centroy; j < tamy; j++) {
            
            if(binary_maximum.at<float>(i,j) == 255) {
                //cout << i << "  " << j << endl;
                bool max = false;
                double max_val;
                
                //crear ROI para pasarselo a minmaxloc
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
    
    ////////////////////////////////////////////

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
    vector<Mat> vMc, pyramid;
    hpoint paux;
    
    cvtColor( src, src_gray, COLOR_BGR2GRAY );

    int levels = 2, blocksize = 3, ksize = 5, num_points = 1000;
    float k = 0.04;
    
    ////////////////////////////////////////////
    
    pyramid = gaussPyramid(src_gray, levels);
    
    for(int l = 0; l < levels; l++) {
        
        myHarris_dst = Mat::zeros( pyramid[l].size(), CV_32FC(6) );
        Mc = Mat::zeros( pyramid[l].size(), CV_32FC1 );
        
        cornerEigenValsAndVecs( pyramid[l], myHarris_dst, blocksize, ksize, BORDER_DEFAULT );
        
        for( int j = 0; j < pyramid[l].rows; j++ ) {
            for( int i = 0; i < pyramid[l].cols; i++ ) {
                float lambda_1 = myHarris_dst.at<Vec6f>(j, i)[0];
                float lambda_2 = myHarris_dst.at<Vec6f>(j, i)[1];
                Mc.at<float>(j,i) = harrisValue(k, lambda_1, lambda_2);
            }
        }
        
        vMc.push_back(Mc);
        
    }
    
    ////////////////////////////////////////////
    
    for(int l = 0; l < levels; l++) {
        Mat binary_maximum (vMc[l].rows, vMc[l].cols, vMc[l].type(), Scalar::all(255));
        supNonMax(vMc[l], binary_maximum, 7);
        
        for(int i = 0; i < binary_maximum.rows; i++) {
            for(int j = 0; j < binary_maximum.cols; j++) {
                if(binary_maximum.at<float>(i,j) == 255) {
                    paux.x = i;
                    paux.y = j;
                    paux.value = vMc[l].at<float>(i,j);
                    paux.level = l+1;
                    hpoints.push_back(paux);
                }
            }
        }
    }
    
    sort(hpoints.begin(), hpoints.end(), compareValue);
    
    subpixelRef(hpoints, num_points, pyramid);
    
    ////////////////////////////////////////////
    
    src_gray.copyTo(out);
    cvtColor(out, out, CV_GRAY2RGB, 3);
    for(int i = 0; i < num_points; i++) {
        circle( out, Point(hpoints[i].y * hpoints[i].level,hpoints[i].x * hpoints[i].level), 2, Scalar(0,0,0), -1, 8, 0);
    }
    showIM(out, "harris");

    ////////////////////////////////////////////
    
    //llamar a orientation
    
    src_gray.copyTo(out);
    cvtColor(out, out, CV_GRAY2RGB, 3);
    for(int i = 0; i < num_points; i++) {
        RotatedRect r = RotatedRect(Point2f(hpoints[i].y * hpoints[i].level,hpoints[i].x * hpoints[i].level), Size2f(6*hpoints[i].level, 6*hpoints[i].level), hpoints[i].angle);
        Point2f vertices[4];
        r.points(vertices);
        for (int i = 0; i < 4; i++)
            line(out, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));
        
        float angle = hpoints[i].angle * CV_PI / 180.0;
        int length = 6*hpoints[i].level;
        float x2 = (hpoints[i].x * hpoints[i].level) + length * cos(angle);
        float y2 = (hpoints[i].y * hpoints[i].level) + length * sin(angle);
        
        Point2f p2(y2,x2);
        line(out, Point2f(hpoints[i].y * hpoints[i].level, hpoints[i].x * hpoints[i].level), p2, Scalar(255,0,0));
        
    }
    
    showIM(out, "harris");

    ////////////////////////////////////////////

    

    return out;
    
}


void subpixelRef(vector<hpoint> &hpoints, int num_points, vector<Mat> &pyramid) {
    
    vector<Point2f> corners;
    Point2f p;
    TermCriteria criteria = TermCriteria( CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 40, 0.001 );
    
    for(int y = 0; y < pyramid.size(); y++) {
        
        for(int i = 0; i < num_points; i++) {
            if(hpoints[i].level == y+1) {
                p.x = hpoints[i].x;
                p.y = hpoints[i].y;
                corners.push_back(p);
            }
        }
        
        cornerSubPix(pyramid[y], corners, Size(5,5), Size(-1,-1), criteria);
        
        int c = 0;
        
        for(int i = 0; i < num_points; i++) {
            if(hpoints[i].level == y+1) {
                hpoints[i].x = corners[c].x;
                hpoints[i].y = corners[c].y;
                c++;
            }
        }
    }
}

void orientation() {
    
    
    
    
    
    
    
    
    
    
}


void drawSIFT(Mat &src) {
    
    Mat src_gray, out;
    cvtColor( src, src_gray, COLOR_BGR2GRAY );

    SiftFeatureDetector detector;
    vector<KeyPoint> kp;
    
    detector.detect(src_gray, kp);
    
    src_gray.copyTo(out);
    drawKeypoints(out, kp, out);
    
    showIM(out, "sift");
    
}

void drawSURF(Mat &src) {
    
    Mat src_gray, out;
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    
    SurfFeatureDetector detector;
    //SiftFeatureDetector detector;
    vector<KeyPoint> kp;
    
    detector.detect(src_gray, kp);
    
    src_gray.copyTo(out);
    drawKeypoints(out, kp, out);
    
    showIM(out, "surf");
    
}


void matchesSurf(Mat &src, Mat &src1) {
    
    Mat src_gray, src1_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    cvtColor(src1, src1_gray, COLOR_BGR2GRAY);
    
    SURF surf;
    
    vector<KeyPoint> kp, kp1;
    Mat descriptor, descriptor1;
    
    surf(src_gray, Mat(), kp, descriptor, false);
    surf(src1_gray, Mat(), kp1, descriptor1, false);
    
    BFMatcher matcher( NORM_L2, true );
    vector<DMatch> matches;
    
    matcher.match( descriptor, descriptor1, matches );
    
    //cout << matches.size();
    
    //pinto solo 20 puntos
    int size = matches.size();
    
    for(int i = 0; i < size-20; i++)
        matches.erase(matches.begin()+(rand() % matches.size()-1));

    Mat img_matches;
    drawMatches( src_gray, kp, src1_gray, kp1, matches, img_matches );
    
    showIM(img_matches, "matches");
    
}

void matchesSift(Mat &src, Mat &src1) {
    
    Mat src_gray, src1_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    cvtColor(src1, src1_gray, COLOR_BGR2GRAY);
    
    SIFT sift;
    
    vector<KeyPoint> kp, kp1;
    Mat descriptor, descriptor1;
    
    sift(src_gray, Mat(), kp, descriptor, false);
    sift(src1_gray, Mat(), kp1, descriptor1, false);
    
    BFMatcher matcher( NORM_L2, true );
    vector<DMatch> matches;
    
    matcher.match( descriptor, descriptor1, matches );
    
    //cout << matches.size();
    
    //pinto solo 20 puntos
    int size = matches.size();
    
    for(int i = 0; i < size-20; i++)
        matches.erase(matches.begin()+(rand() % matches.size()-1));
    
    Mat img_matches;
    drawMatches( src_gray, kp, src1_gray, kp1, matches, img_matches );
    
    showIM(img_matches, "matches");
    
}




