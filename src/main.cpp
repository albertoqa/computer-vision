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
	Mat fm = imread("/Users/alberto/Desktop/dev/computer-vision/src/images/fish.bmp", 1);
    Mat src, dst;
    
    convertToFloat(fm, src);
    
    
	//Mat src2 = imread("/Users/alberto/Desktop/dev/computer-vision/src/images/dog.bmp");
	
	
	//cout << "Rows: " << src1.rows << "   Cols: " << src1.cols << endl;
	//cout << src1.channels();

	//if( !src1.data ) { printf("Error loading src1 \n"); return -1; }
	//if( !src2.data ) { printf("Error loading src2 \n"); return -1; }

	//blend(src1, src2, dst, 0.5);
	//dFilterA(src1, mask, dst);
	//horizontalTrans(src1);
	//flipIM(src1, dst, 1);
	//showIM(dst, "horizontal");
	
	GaussFilter(src, dst, 3, 0);
    
    Mat low, high;
    //createHighLow(src1, low, high, 3, 0);
    
    convertToUchar(dst, src);
	showIM(src, "gauss");


	//createGaussKernel(dst, 1);
	//for(int i = 0; i < (3+3)*2; i++){
	//cout << dst;
	//}
	
}
