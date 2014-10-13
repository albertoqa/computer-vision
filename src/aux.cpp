//
//  aux.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 12/10/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "aux.h"

void showIM(Mat &src, string windowName){
	
	namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	imshow(windowName, src);
	waitKey();
	destroyWindow(windowName);
	
}

// 0 = vertical flip
// 1 = horizontal flip
// 2 = vertical and horizontal flip
void flipIM(Mat &src, OutputArray _dst, int mode) {
	
	size_t tt = CV_ELEM_SIZE(src.type());
	Size size = src.size();
	
	int type = src.type();
	_dst.create( size, type );
	Mat dst = _dst.getMat();
	
	if(mode == 0)
		flipV(src, dst, tt);
	else if (mode == 1)
		flipH(src.ptr(), dst.ptr(), src.step, dst.step, size, tt);
	else if (mode == 2) {
		flipV(src, dst, tt);
		flipH(src.ptr(), dst.ptr(), src.step, dst.step, size, tt);
	}
	else
		cout << "Mode not recognised" << endl << endl;
	
	//cout << "size: " << src.size() << "    type: " << tt << endl << endl;
	
}


void flipV(Mat &src, Mat &dst, size_t tt){
	
	
	
}

void flipH(const uchar* src, uchar* dst, size_t sstep, size_t dstep, Size size, size_t tt){
	
	int i, j, limit = (int)(((size.width + 1)/2)*tt);
	AutoBuffer<int> _tab(size.width*tt);
	int* tab = _tab;
	
	for( i = 0; i < size.width; i++ )
		for( size_t k = 0; k < tt; k++ )
			tab[i*tt + k] = (int)((size.width - i - 1)*tt + k);
	
	for( ; size.height--; src += sstep, dst += dstep )
	{
		for( i = 0; i < limit; i++ )
		{
			j = tab[i];
			uchar t0 = src[i], t1 = src[j];
			dst[i] = t1; dst[j] = t0;
		}
	}
	
	
}