//
//  blend.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 12/10/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "blend.h"

void blend(Mat &src1, Mat &src2, Mat &dst, double alpha) {
	double beta = (1.0 - alpha);
		
	namedWindow("Linear Blend", 1);
		
	addWeighted( src1, alpha, src2, beta, 0.0, dst);
		
	imshow( "Linear Blend", dst );
	waitKey(0);
	destroyWindow("Linear Blend");
	
}