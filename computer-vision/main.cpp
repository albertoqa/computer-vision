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

using namespace std;
using namespace cv;


int main (int argc, const char * argv[])
{
	Mat src1 = imread("/Users/alberto/Desktop/dev/computer-vision/computer-vision/images/cat.bmp");
	Mat src2 = imread("/Users/alberto/Desktop/dev/computer-vision/computer-vision/images/dog.bmp");
	Mat dst;

	if( !src1.data ) { printf("Error loading src1 \n"); return -1; }
	if( !src2.data ) { printf("Error loading src2 \n"); return -1; }

	//cout << "M = " << endl << " " << src << endl;

	blend(src1, src2, dst, 0.5);
	
}
