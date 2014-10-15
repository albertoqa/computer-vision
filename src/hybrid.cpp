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
	
	int border_type = 0; // 0 uniforme a 0; 1 reflejada
	GaussFilter(src, dst, sigma, border_type);
	
}


int reflect(int M, int x)
{
	if(x < 0)
	{
		return -x - 1;
	}
	if(x >= M)
	{
		return 2*M - x - 1;
	}
	
	return x;
}

void GaussFilter(Mat &src, Mat &dst, float sigma, int border_type) {
	
	/*int type = src.type();
	Size size = src.size();
	dst.create( size, type );
	
	Mat temp;
	temp.create(size, type);
	
	Mat xk;
	createGaussKernel(xk, sigma);
	
	float *mask = xk.ptr<float>();
	int pos = 0;
	float x1, y1;
	//cout << xk.rows;
	
	float sum = 0;*/
	
	dst.create(src.size(), src.type());
	Mat tmp (src.size(), src.type());
	float sum, x1, y1;
	
	Mat xk;
	createGaussKernel(xk, sigma);
	
	float *xxk = xk.ptr<float>();
	
	//for(int i = 0; i < (sigma * 6) + 1; i++)
	//	cout << xxk[i] << "    ";
	
	int ss = (sigma * 6) + 1;
	
	for(int y = 0; y < src.rows; y++){
		for(int x = 0; x < src.cols; x++){
			sum = 0.0;
			for(int i = 0; i < ss; i++){
				y1 = reflect(src.rows, y - i);
				sum = sum + xxk[i]*src.at<uchar>(y1, x);
			}
			tmp.at<uchar>(y,x) = sum;
		}
	}
	
	
	// along x - direction
	for(int y = 0; y < src.rows; y++){
		for(int x = 0; x < src.cols; x++){
			sum = 0.0;
			for(int i = 0; i < ss; i++){
				x1 = reflect(src.cols, x - i);
				sum = sum + xxk[i]*tmp.at<uchar>(y, x1);
			}
			dst.at<uchar>(y,x) = sum;
		}
	}
	
	
	
	
	//sepFilter2D(_src, _dst, CV_MAT_DEPTH(type), kx, ky, Point(-1,-1), 0, borderType );
	
	//usar propiedades de las matrices (producto escalar?)
	//usar solo una implementacion -> hacerlo por filas, transponer y hcaer lo mismo??
	
	
	
}

void createGaussKernel(Mat &xk, float sigma) {
	
	int size = (sigma * 6) + 1;
	
	xk.create(size, 1, CV_32F);
	float *xxk = xk.ptr<float>();
	//xk = new float [size];
	
	float sum = 0, x, pos;
	
	for(int i = 0; i < size; i++) {
		
		pos = i - ((size-1)/2);
		x = exp((-0.5/(sigma*sigma))*pos*pos);
		
		xxk[i] = x; // no se porque con Xcode no funciona, pero compilando normal si
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
