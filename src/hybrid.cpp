//
//  hybrid.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 12/10/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "hybrid.h"
#include "aux.h"

void imGaussConv(Mat &src, Mat &mask, Mat &dst) {
	
	
	
	
}

/*void dFilterA(Mat &src, Mat &mask, Mat &dst) {
	
	Point anchor;
	double delta;
	int ddepth;
	int kernel_size;
	
	int c;
	
	/// Initialize arguments for the filter
	anchor = Point( -1, -1 );
	delta = 0;
	ddepth = -1;
	
	/// Loop - Will filter the image with different kernel sizes each 0.5 seconds
	int ind = 0;
	while( true )
	{
		c = waitKey(500);
		/// Press 'ESC' to exit the program
		if( (char)c == 27 )
		{ break; }
		
		/// Update kernel size for a normalized box filter
		kernel_size = 3 + 2*( ind%5 );
		mask = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
		
		/// Apply filter
		filter2D(src, dst, ddepth , mask, anchor, delta, BORDER_DEFAULT );
		showIM(dst, "filterA");
		ind++;
	}
	
}*/
