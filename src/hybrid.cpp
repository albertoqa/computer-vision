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


int border(int M, int x, int border_type)
{

	if(border_type == 0) { // reflected
		if(x < 0) {
			return -x - 1;
		}
		if(x >= M) {
			return 2*M - x - 1;
		}

		return x;
	}
    else {	// uniform to 0
        if(x < 0 || x >= M)
            return 0;
    
        return x;
    }

}

void GaussFilter(Mat &src, Mat &dst, float sigma, int border_type) {

	dst.create(src.size(), src.type());
	Mat tmp (src.size(), src.type());
	float sum, x1, y1;

	Mat xk;
	createGaussKernel(xk, sigma);

	float *xxk = xk.ptr<float>();

	int ss = (sigma * 6) + 1;
	int channels = src.channels();
	//cout << channels;

	vector<Mat> m;
	split(src, m);

	for(int rgb = 0; rgb < channels; rgb++) {

		for(int y = 0; y < m[rgb].rows; y++) {
			for(int x = 0; x < m[rgb].cols; x++) {
				sum = 0.0;
				for(int i = 0; i < ss; i++){
					y1 = border(m[rgb].rows, y - i, border_type);
					sum = sum + xxk[i]*m[rgb].at<float>(y1, x);
				}
				tmp.at<float>(y,x) = sum;
			}
		}

		for(int y = 0; y < m[rgb].rows; y++) {
			for(int x = 0; x < m[rgb].cols; x++) {
				sum = 0.0;
				for(int i = 0; i < ss; i++){
					x1 = border(m[rgb].cols, x - i, border_type);
					sum = sum + xxk[i]*tmp.at<float>(y, x1);
				}
				m[rgb].at<float>(y,x) = sum;
			}
		}

	}

	merge(m, dst);

}

//create a gauss kernel based on sigma (input)
//the kernel is saved in the matrix xk
void createGaussKernel(Mat &xk, float sigma) {

	int size = (sigma * 6) + 1;

	xk.create(size, 1, CV_32F);
	float *xxk = xk.ptr<float>();

	float sum = 0, x, pos;

	for(int i = 0; i < size; i++) {

		pos = i - ((size-1)/2);
		x = exp((-0.5/(sigma*sigma))*pos*pos);

		xxk[i] = x;
        
        sum = sum + x;

	}

	sum = 1.0/sum;

	for(int i = 0; i < size; i++ )
		xxk[i] = xxk[i] * sum;

}

void createHighLow(Mat &src, Mat &src1, Mat &low, Mat &high, float sigma, int border_type) {
    
    Mat aux;
    
    GaussFilter(src, aux, sigma, border_type);
    
    high = src - aux;
    //high /= 2;
    
    // put the low frecuency image in Mat low
    GaussFilter(src1, low, sigma, border_type);
    
    // compute the hybrid image
    src = low + high;
    
}

Mat createOne(vector<Mat> & images, int cols, int min_gap_size)
{
    // let's first find out the maximum dimensions
    int max_width = 0;
    int max_height = 0;
    for ( int i = 0; i < images.size(); i++) {
        // check if type is correct
        // you could actually remove that check and convert the image
        // in question to a specific type
        if ( i > 0 && images[i].type() != images[i-1].type() ) {
            cerr << "WARNING:createOne failed, different types of images";
            return Mat();
        }
        max_height = max(max_height, images[i].rows);
        max_width = max(max_width, images[i].cols);
    }
    // number of images in y direction
    int rows = ceil(images.size() / cols)+1;
    
    // create our result-matrix
    Mat result = Mat::zeros(rows*max_height + (rows-1)*min_gap_size,
                                    cols*max_width + (cols-1)*min_gap_size, images[0].type());
    size_t i = 0;
    int current_height = 0;
    int current_width = 0;
    for ( int y = 0; y < rows; y++ ) {
        for ( int x = 0; x < cols; x++ ) {
            if ( i >= images.size() ) // shouldn't happen, but let's be safe
                return result;
            // get the ROI in our result-image
            Mat to(result,
                       Range(current_height, current_height + images[i].rows),
                       Range(current_width, current_width + images[i].cols));
            // copy the current image to the ROI
            images[i++].copyTo(to);
            current_width += images[i-1].cols + min_gap_size;
        }
        // next line - reset width and update height
        current_width = 0;
        current_height += max_height + min_gap_size;
    }
    return result;
}


vector<Mat> gaussPyramid(Mat &src, int levels) {
        
    if(src.type() == 0)
        cvtColor(src, src, CV_GRAY2BGR, 3);
    
    Mat aux, img = src;
    
    vector<Mat> v;
    v.push_back(src);
    
    for(int i = 0; i < levels; i++) {
        
        //GaussFilter(img, aux, 3, 0);
        pyrDown(img, aux);
        
        v.push_back(aux);

        aux.copyTo(img);
        
    }
    
    return v;
}


