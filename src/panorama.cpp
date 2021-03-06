//
//  panorama.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 4/11/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "panorama.h"
#include "aux.h"
#include <math.h>

// funcion to show an image
void showIM(Mat &src, string windowName){
    
    namedWindow(windowName, CV_WINDOW_AUTOSIZE);
    imshow(windowName, src);
    waitKey();
    destroyWindow(windowName);
    
}

// compare the harris value of two given points
bool compareValue(const hpoint &v1, const hpoint &v2) {
    return v1.value > v2.value;
}

// calculate the harris value
float harrisValue(float k, float x, float y) {
    return ((x*y) - k*pow(x+y, 2));
}

// non max supression function
void supNonMax(Mat &Mc, Mat &binary_maximum, int window) {
    
    // window must be odd
    if(window%2 == 0)
        window++;
    if(window < 3)
        window = 3;
    
    // center of the window
    int centrox, centroy;
    centrox = window/2;
    centroy = window/2;
    
    int tamx = Mc.rows - window/2;
    int tamy = Mc.cols - window/2;
    
    ////////////////////////////////////////////
    
    for(int i = centrox; i < tamx; i++) {
        for(int j = centroy; j < tamy; j++) {
            
            // only check if maximum if value is 255
            if(binary_maximum.at<float>(i,j) == 255) {

                bool max = false;
                double max_val;
                
                //crear ROI para pasarselo a minmaxloc
                Mat ROI_Mc = Mc( Rect(j-window/2, i-window/2, window, window) );
                Mat ROI_bin = binary_maximum( Rect(j-window/2, i-window/2, window, window) );
                minMaxLoc(ROI_Mc, NULL, &max_val);
                
                // if center is maximum of the window, then keep it
                if(float(max_val) == Mc.at<float>(i,j))
                    max = true;
                
                if(max) {
                    // put everything to 0 except the center
                    ROI_bin = 0.0;
                    ROI_bin.at<float>(centrox,centroy) = 255;
                }
                else {
                    // if it is non max put it to 0
                    binary_maximum.at<float>(i,j) = 0;
                }
            }
        }
    }
    
    ////////////////////////////////////////////

    // put borders to 0
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

// calculate harris points of a given image
Mat harrisPoints(Mat &src) {
    
    Mat src_gray, myHarris_dst, myHarris_copy, Mc, out;
    vector<hpoint> hpoints;
    vector<Mat> vMc, pyramid;
    hpoint paux;
    
    // convert image to grayscale
    cvtColor( src, src_gray, COLOR_BGR2GRAY );

    int levels = 2, blocksize = 3, ksize = 5, num_points = 1000;
    float k = 0.04;
    
    ////////////////////////////////////////////
    
    // gaussian pyramid of l levels
    pyramid = gaussPyramid(src_gray, levels);
    
    // for each level calculate the harris matrix
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
    
    // for each level implement the non maximal supression and fill the hpoint data
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
    
    // sort the points acording to its harris value
    sort(hpoints.begin(), hpoints.end(), compareValue);
    
    // implement the subpixel aproximation
    subpixelRef(hpoints, num_points, pyramid);
    
    ////////////////////////////////////////////
    
    // draw the image with circles in the best harris points
    src_gray.copyTo(out);
    cvtColor(out, out, CV_GRAY2RGB, 3);
    for(int i = 0; i < num_points; i++) {
        circle( out, Point(hpoints[i].y * hpoints[i].level,hpoints[i].x * hpoints[i].level), 2, Scalar(0,0,0), -1, 8, 0);
    }
    showIM(out, "harris");

    ////////////////////////////////////////////
    
    //llamar a orientation
    orientation(src_gray, levels, hpoints);
    
    // draw image with squares in the best harris points oriented to the gradient direction
    src_gray.copyTo(out);
    cvtColor(out, out, CV_GRAY2RGB, 3);
    for(int i = 0; i < num_points; i++) {
        RotatedRect r = RotatedRect(Point2f(hpoints[i].y * hpoints[i].level,hpoints[i].x * hpoints[i].level), Size2f(6*hpoints[i].level, 6*hpoints[i].level), hpoints[i].angle);
        Point2f vertices[4];
        r.points(vertices);
        for (int i = 0; i < 4; i++)
            
            line(out, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));
        
        //float angle = 180 + hpoints[i].angle / CV_PI * 180.0;
        float angle = hpoints[i].angle;
        int length = 6*hpoints[i].level;
        float y2 = (hpoints[i].x * hpoints[i].level) + length * cos(angle);
        float x2 = (hpoints[i].y * hpoints[i].level) + length * sin(angle);
        
        Point p2(x2,y2);
        line(out, Point(hpoints[i].y * hpoints[i].level, hpoints[i].x
                          * hpoints[i].level), p2, Scalar(255,0,0));
        
    }
    
    showIM(out, "harris");

    ////////////////////////////////////////////

    

    return out;
    
}

// sub pixel aproximation
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
        
        //corners.erase(corners.begin(), corners.end());
    }
}

// orientation of the gradient
void orientation(Mat &src, int levels, vector<hpoint> hpoints) {
    
    //vector<Mat> pyramid;
    //pyramid = gaussPyramid(src, levels);
    
    Mat gx, gy, agx, agy;
    
    // gradient x
    Sobel( src, gx, CV_64F, 1, 0);
    //convertScaleAbs( gx, agx);
    
    // gradient y
    Sobel( src, gy, CV_64F, 0, 1);
    //convertScaleAbs( gy, agy );
        
        //addWeighted( agx, 0.5, agy, 0.5, 0, grad );
        //gradx.push_back(gx);
        //grady.push_back(gy);
        
    //}
    
    for(int i = 0; i < hpoints.size(); i++) {
        
        double rad = atan((gy.at<double>(hpoints[i].x, hpoints[i].y) / gx.at<double>(hpoints[i].x, hpoints[i].y)) * (180.0/CV_PI));
        //float deg = 180 + rad /
        //cout << rad << endl;
        hpoints[i].angle = rad;
        
    }
    
}

// sift detector
void drawSIFT(Mat &src) {
    
    Mat src_gray, out;
    cvtColor( src, src_gray, COLOR_BGR2GRAY );

    SiftFeatureDetector detector;
    vector<KeyPoint> kp;
    
    detector.detect(src_gray, kp);
    
    //cout << kp.size() << endl;
    
    src_gray.copyTo(out);
    drawKeypoints(out, kp, out);
    
    showIM(out, "sift");
    
}

// surf detector
void drawSURF(Mat &src) {
    
    Mat src_gray, out;
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    
    SurfFeatureDetector detector;
    //SiftFeatureDetector detector;
    vector<KeyPoint> kp;
    
    detector.detect(src_gray, kp);
    
    //cout << kp.size();
    
    src_gray.copyTo(out);
    drawKeypoints(out, kp, out);
    
    showIM(out, "surf");
    
}

// matching points using the surf detector
void matchesSurf(Mat &src, Mat &src1) {
    
    Mat src_gray, src1_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    cvtColor(src1, src1_gray, COLOR_BGR2GRAY);
    
    SURF surf;
    
    vector<KeyPoint> kp, kp1;
    Mat descriptor, descriptor1;
    
    surf(src_gray, Mat(), kp, descriptor);
    surf(src1_gray, Mat(), kp1, descriptor1);
    
    //crosscheck = true and bruteforce
    BFMatcher matcher( NORM_L2, true );
    vector<DMatch> matches;
    
    matcher.match( descriptor, descriptor1, matches );
    
    //pinto solo 20 puntos
    int size = matches.size();
    
    for(int i = 0; i < size-20; i++)
        matches.erase(matches.begin()+(rand() % matches.size()-1));

    Mat img_matches;
    drawMatches( src_gray, kp, src1_gray, kp1, matches, img_matches );
    
    showIM(img_matches, "matches");
    
}

// matching points using the sift detector
void matchesSift(Mat &src, Mat &src1) {
    
    Mat src_gray, src1_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    cvtColor(src1, src1_gray, COLOR_BGR2GRAY);
    
    SIFT sift;
    
    vector<KeyPoint> kp, kp1;
    Mat descriptor, descriptor1;
    
    sift(src_gray, Mat(), kp, descriptor, false);
    sift(src1_gray, Mat(), kp1, descriptor1, false);
    
    //crosscheck = true and bruteforce
    BFMatcher matcher( NORM_L2, true );
    vector<DMatch> matches;
    
    matcher.match( descriptor, descriptor1, matches );
        
    //pinto solo 20 puntos
    int size = matches.size();
    
    for(int i = 0; i < size-20; i++)
        matches.erase(matches.begin()+(rand() % matches.size()-1));
    
    Mat img_matches;
    drawMatches( src_gray, kp, src1_gray, kp1, matches, img_matches );
    
    showIM(img_matches, "matches");
    
}

// draw a mosaic/panorama of two images
void mosaic(Mat &src, Mat &src1) {
    
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

    vector<Point2f> im1, im2;
    
    for( int i = 0; i < matches.size(); i++ ) {
        im1.push_back( kp[ matches[i].queryIdx ].pt );
        im2.push_back( kp1[ matches[i].trainIdx ].pt );
    }
    
    // find homography of the two images
    Mat homography = findHomography(im2, im1, CV_RANSAC, 1);
    
    Mat warped;
    warpPerspective(src1_gray, warped, homography, Size(src1_gray.cols*2,src1_gray.rows*2));
    
    Mat out(Size(src1_gray.cols*2 + src_gray.cols, src1_gray.rows*2), src1_gray.type());
    
    Mat roi1(out, Rect(0, 0,  src_gray.cols, src_gray.rows));
    Mat roi2(out, Rect(0, 0, warped.cols, warped.rows));
    warped.copyTo(roi2);
    src_gray.copyTo(roi1);
    
    showIM(out, "salida");

    
}


