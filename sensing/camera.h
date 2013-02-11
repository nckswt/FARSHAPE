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

	    Mat blueThreshold(Mat src);
	    Mat edgeThreshold(Mat src);
	    Mat streamVid();

	    vector<int> getX();

    private:
	    Mat original;
	    Mat blueProcessed;
	    Mat edgeProcessed;
	    VideoCapture cap;
	    vector<vector<Point> > contours;
	    vector<Vec4i> hierarchy;
	    vector<int> x;
};

#endif
