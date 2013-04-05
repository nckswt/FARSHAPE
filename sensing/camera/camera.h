#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <iostream>
//#include <math.h>
#include <stdio.h>
//#include <highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define WIDTH 540
#define HEIGHT 400

using namespace std;
using namespace cv;

enum
{
    P_GREEN,
    P_BLUE
};

class Camera
{
    public:
	    Camera();
	    Camera(int device);
	    ~Camera();

	    Mat ColorThreshold(Mat src, int HSV[2][3]);
	    Mat edgeThreshold(Mat src, Scalar Color, int piece);
	    Mat streamVid();
	    Mat Draw(int piece);
	    vector<double> getX(int piece);
	    vector<double> getY(int piece);
	    vector<double> getWidth(int piece);
	    vector<double> getHeight(int piece);

    private:
	    Mat original;
	    Mat canny_output;
	    VideoCapture cap;
	    vector<vector<Point> > contours;
	    vector<Vec4i> hierarchy;
	    vector<double> bluex;
	    vector<double> bluey;
	    vector<double> bluewidth;
	    vector<double> blueheight;
	    vector<double> greenx;
	    vector<double> greeny;
	    vector<double> greenwidth;
	    vector<double> greenheight;
};

#endif

