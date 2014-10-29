//
//  hybrid.cpp
//  computer-vision
//
//  Created by Alberto Quesada Aranda on 12/10/14.
//  Copyright (c) 2014 Alberto Quesada Aranda. All rights reserved.
//

#include "hybrid.h"
#include "aux.h"


// Border detection
// 0 - reflected, 1 - uniform to 0
//-------------------------------------------------------------------------------------

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

// Gauss Filter
// src - input image
// sigma
// border_type -> 0 - reflected, 1 - uniform to 0
//-------------------------------------------------------------------------------------

Mat GaussFilter(Mat &src, float sigma, int border_type) {

    Mat dst; // return matrix
    
	dst.create(src.size(), src.type()); //create it with same size and type as input image
	Mat tmp (src.size(), src.type()); // used for 1D
	float sum, x1, y1;

	Mat xk; // kernel matrix (it is a vector actually...)
	createGaussKernel(xk, sigma); // generate the kernel
    //xk = firstDerivative(sigma);
    //xk = secondDerivative(sigma);
    
	float *xxk = xk.ptr<float>(); // pointer to the kernel for make easier the access

	int ss = (sigma * 6) + 1; // size of the kernel
	int channels = src.channels();  // channels of the input image

	vector<Mat> m; // save spplited image
	split(src, m); // split input image

	for(int rgb = 0; rgb < channels; rgb++) { // for each channel of the image

		for(int y = 0; y < m[rgb].rows; y++) {
			for(int x = 0; x < m[rgb].cols; x++) {
				sum = 0.0; // restart sum
				for(int i = 0; i < ss; i++){ // iterate over all the kernel size
					y1 = border(m[rgb].rows, y - i, border_type); // check if border
					sum = sum + xxk[i]*m[rgb].at<float>(y1, x); // add value to the sum
				}
				tmp.at<float>(y,x) = sum; // temporal value for the pixel y,x
			}
		}

		for(int y = 0; y < m[rgb].rows; y++) {
			for(int x = 0; x < m[rgb].cols; x++) {
				sum = 0.0; // restart sum
				for(int i = 0; i < ss; i++){ // iterate over all the kernel size
					x1 = border(m[rgb].cols, x - i, border_type); // check if border
					sum = sum + xxk[i]*tmp.at<float>(y, x1); // add value to the sum
				}
				m[rgb].at<float>(y,x) = sum; // save final value for the pixel y,x
			}
		}

	}

	merge(m, dst); // merge image

    return dst;
}

// Create a gauss kernel based on sigma (input)
// The kernel is saved in the matrix xk
//-------------------------------------------------------------------------------------

void createGaussKernel(Mat &xk, float sigma) {

	int size = (sigma * 6) + 1; // size of the kernel

	xk.create(size, 1, CV_32F); // create matrix (vector)
	float *xxk = xk.ptr<float>(); // pointer to the matrix, it makes easier the access

	float sum = 0, x, pos;

	for(int i = 0; i < size; i++) { // calculate the value of each position

		pos = i - ((size-1)/2);
		x = exp((-0.5/(sigma*sigma))*pos*pos);

		xxk[i] = x;
        
        sum = sum + x;

	}

	sum = 1.0/sum;

	for(int i = 0; i < size; i++ ) // divide by the total sum
		xxk[i] = xxk[i] * sum;

}

// Creation of the hybrid image
//-------------------------------------------------------------------------------------

Mat createHighLow(Mat &src, Mat &src1, Mat &low, Mat &high, float sigma1, float sigma2, int border_type) {
    
    Mat aux, hybrid;
    
    aux = GaussFilter(src, sigma1, border_type); // generate low frecuency image
    src.copyTo(high);
    high -= aux; // generate high frecuency image
    
    // put the low frecuency image in Mat low
    low = GaussFilter(src1, sigma2, border_type);
    
    // compute the hybrid image
    hybrid = high + low;
    return hybrid;
    
}

// Draw various images in one canvas
//-------------------------------------------------------------------------------------

Mat createOne(vector<Mat> & images, int cols, int min_gap_size)
{
    // let's first find out the maximum dimensions
    int max_width = 0;
    int max_height = 0;
    for ( int i = 0; i < images.size(); i++) {
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

// Gauss Pyramid with x levels
//-------------------------------------------------------------------------------------

vector<Mat> gaussPyramid(Mat &src, int levels) {
    
    if(src.type() == 0)
        cvtColor(src, src, CV_GRAY2BGR, 3); // convert to color image
    
    Mat aux, img = src;
    
    vector<Mat> v; // output vector of images
    v.push_back(src); // include initial input image
    
    for(int i = 0; i < levels; i++) { // for each level
        
        pyrDown(img, aux); // reduce and smooth image
        
        v.push_back(aux); // add to the output
        
        aux.copyTo(img);
    }
    
    return v;
    
    
    /*-------------------------------------------------------------------------------------
        Implementation of the function pyrDown(). It is commented because the original
        works way better. Basically it makes usage of my implementation of the gauss
        filter and shrink to half the image by deleting the odd rows and columns.
    /-------------------------------------------------------------------------------------*/

    
    /*if(src.type() == 0)
        cvtColor(src, src, CV_GRAY2BGR, 3);
    
    Mat newS, smooth;
    src.copyTo(smooth);
    
    cvtColor(smooth, smooth, 3);
    
    vector<Mat> v;
    v.push_back(smooth);
    
    for(int i = 0; i < levels; i++) {
     
        newS.create(smooth.rows/2, smooth.cols/2, CV_32F);
        smooth = GaussFilter(smooth, 1.0, 0);
        
        for(int r = 0, r1 = 0; r < newS.rows; r++, r1 = r1+2) {
            for(int c = 0, c1 = 0; c < newS.cols; c++, c1 = c1+2) {
                newS.at<Vec3b>(r,c) = smooth.at<Vec3b>(r1,c1);
            }
        }
        
        v.push_back(newS);
        
        smooth.create(smooth.rows/2, smooth.cols/2, CV_32F);
        newS.copyTo(smooth);
        
    }
    
    return v;*/
}

// Calculate the first derivative of the kernel
//-------------------------------------------------------------------------------------

Mat firstDerivative(float sigma) {
    
    int middle = ((sigma * 6)/2);
    float x, tmp;
    
    Mat kernel;
    createGaussKernel(kernel, sigma);
    float *xxk = kernel.ptr<float>();
    
    xxk[middle] = 0.0;
    
    for(int i = 1; i < middle+1; i++) {
        x = (float)i;
        tmp = -x / (sigma*sigma) * xxk[middle + i];
        xxk[middle + i] = -tmp;
        xxk[middle - i] = tmp;
    }
    
    return kernel;
}

// Calculate the second derivative of the kernel
//-------------------------------------------------------------------------------------

Mat secondDerivative(float sigma) {
    
    int middle = ((sigma * 6)/2);
    float x, tmp;
    
    Mat kernel;
    createGaussKernel(kernel, sigma);
    float *xxk = kernel.ptr<float>();
    
    xxk[middle] *= -1.0/(sigma*sigma);
    
    for(int i = 1; i < middle+1; i++) {
        x = (float)i;
        tmp = (x * x / sigma*sigma - 1.0) * xxk[middle + i] / (sigma*sigma);
        xxk[middle + i] = tmp;
        xxk[middle - i] = tmp;
    }
    
    return kernel;
}

