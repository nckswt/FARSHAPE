#include "camera.h"


Camera::Camera()
{
    cap.open(0);
    x.clear();
    contours.clear();
    hierarchy.clear();
    if(!cap.isOpened())
    {
	printf("Failed to open video device\n");
    }    
}

Camera::Camera(int device)
{
    cap.open(device);
    if(!cap.isOpened())
    {
	printf("Failed to open video device\n");
    }
}

Camera::~Camera()
{
    x.clear();
    contours.clear();
    hierarchy.clear();    
}

Mat Camera::streamVid()
{
    cap >> original;
    return original;
}

Mat Camera::blueThreshold(Mat src)
{
    Mat imgHSV;
    Mat dst;
    cvtColor(src, imgHSV, CV_BGR2HSV);
    inRange(imgHSV, Scalar(80, 135, 135), Scalar(130, 255, 255), dst);
    blur(dst, dst, Size(15,15));
    threshold(dst, dst, 100, 255, THRESH_BINARY);
    return dst;
}

Mat Camera::edgeThreshold(Mat src)
{
    Mat canny_output;
    
    x.clear();
    contours.clear();
    hierarchy.clear();
        
    Canny(src, canny_output, 100, 200, 3);
    findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0,0));

    vector<Point2f> mc(contours.size());
    vector<Moments> mu(contours.size());

    for(int i=0; i<contours.size(); i++)
	mu[i] = moments(contours[i], false);

    for(int i=0; i<contours.size(); i++)
	mc[i] = Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00); 

    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);

    for(int i=0; i<contours.size(); i++)
    {
	drawContours(drawing, contours, i, Scalar(255,150,150), 1, 4, hierarchy, 0, Point());
	circle(drawing, mc[i], 1, Scalar(255,255,255), -1, 8, 0); 
	x.push_back(mc[i].x);
    }
    return drawing;
}

vector<int> Camera::getX()
{
    return x;
}
