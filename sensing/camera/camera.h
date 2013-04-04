#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <iostream>
#include <stdio.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>

#define WIDTH 540
#define HEIGHT 400

using namespace std;
using namespace cv;

class Camera
{
    public:
	    Camera();
	    Camera(int device);
	    ~Camera();

	    Mat ColorThreshold(Mat src, int HSV[2][3]);
	    Mat edgeThreshold(Mat src, Scalar Color);
	    Mat streamVid();
	    Mat drawRect(Mat src);
	    vector<int> getX();

    private:
	    Mat original;
	    Mat canny_output;
	    VideoCapture cap;
	    vector<vector<Point> > contours;
	    vector<Vec4i> hierarchy;
	    vector<int> x;
};

#endif
