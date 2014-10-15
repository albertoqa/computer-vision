//
//  hybrid.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 12/10/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "hybrid.h"
#include "aux.h"

void imGaussConv(Mat &src, Mat &dst, float sigma) {
	
	// compute kernel size according to sigma
	// the kernel must be an odd number
	
	int border_type = 0; // 0 uniforme a 0; 1 reflejada
	GaussFilter(src, dst, sigma, border_type);
	
}


void GaussFilter(InputArray &src, OutputArray &dst, float sigma, int border_type) {
	
	int type = src.type();
	Size size = src.size();
	dst.create( size, type );
	
	Mat xk;
	createGaussKernel(xk, sigma);
	
	//sepFilter2D(_src, _dst, CV_MAT_DEPTH(type), kx, ky, Point(-1,-1), 0, borderType );
	
	//usar propiedades de las matrices (producto escalar?)
	//usar solo una implementacion -> hacerlo por filas, transponer y hcaer lo mismo??
	
	
	
}

void createGaussKernel(Mat &xk, float sigma) {
	
	int size = ((sigma + 3) * 2 ) + 1;
	
	xk.create(size, 1, CV_32F);
	//por alguna razón esto ahora no funciona
	float *xxk = xk.ptr<float>();
	
	float sum = 0, x;
	
	for(int i = 0; i < size; i++) {
		
		float pos = i - ((size-1)/2);
		x = exp(-0.5*(pos/(sigma*sigma)));
		
		xxk[i] = x;
		sum = sum + x;
		
	}
	
	sum = 1.0/sum;
	
	for(int i = 0; i < size; i++ )
		xxk[i] = xxk[i] * sum;
	
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
