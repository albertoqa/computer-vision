//
//  main.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 12/10/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include "blend.h"
#include "aux.h"
#include "hybrid.h"

using namespace std;
using namespace cv;


int main (int argc, const char * argv[])
{
	Mat src1 = imread("/Users/alberto/Desktop/dev/computer-vision/src/images/fish.bmp", 1);
    //cout << src1.channels();
   // Mat_<CV_32F> fm;
    Size siz = src1.size();
    Mat fm(siz, CV_32F);
    src1.convertTo(fm,CV_32F);
    
	//Mat src2 = imread("/Users/alberto/Desktop/dev/computer-vision/src/images/dog.bmp");
	Mat dst;

	//convertir las matrices a flotante y hacer las operaciones en flotante
	
	
	//cout << "Rows: " << src1.rows << "   Cols: " << src1.cols << endl;
	//cout << src1.channels();

	//if( !src1.data ) { printf("Error loading src1 \n"); return -1; }
	//if( !src2.data ) { printf("Error loading src2 \n"); return -1; }

	//blend(src1, src2, dst, 0.5);
	//dFilterA(src1, mask, dst);
	//horizontalTrans(src1);
	//flipIM(src1, dst, 1);
	//showIM(dst, "horizontal");
	
	GaussFilter(fm, dst, 3, 0);
    
    double min, max;
    minMaxLoc(dst, &min, &max);
    dst.convertTo(src1,CV_8U,255.0/(max-min));

	showIM(src1, "gauss");


	//createGaussKernel(dst, 1);
	//for(int i = 0; i < (3+3)*2; i++){
	//cout << dst;
	//}
	
}
