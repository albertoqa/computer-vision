//
//  aux.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 12/10/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "aux.h"

void showIM(Mat &img, string windowName){
	
	namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	imshow(windowName, img);
	waitKey();
	destroyWindow(windowName);
	
}