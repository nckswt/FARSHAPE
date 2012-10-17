#include "camera.h"


Camera::Camera()
{
//    cap.open(1);
    bluex.clear();
    bluey.clear();
    greenx.clear();
    greeny.clear();
    bluewidth.clear();
    blueheight.clear();
    greenwidth.clear();
    greenheight.clear();
    contours.clear();
    hierarchy.clear();
//    if(!cap.isOpened())
  //  {
//	printf("Failed to open video device\n");
  //  }    
}

Camera::Camera(int device)
{
    //cap.open(device);
    //if(!cap.isOpened())
    //{
//	printf("Failed to open video device\n");
  //  }
}

Camera::~Camera()
{
    bluex.clear();
    bluey.clear();
    greenx.clear();
    greeny.clear();
    bluewidth.clear();
    blueheight.clear();
    greenwidth.clear();
    greenheight.clear();
    contours.clear();
    hierarchy.clear();
}

Mat Camera::streamVid()
{
    //cap >> original;
    return original;
}

//Fucking HSV values
//Blue: 105,116,64 --- 125,209,255
//Green: 53,90,32 --- 100,185,255
Mat Camera::ColorThreshold(Mat src, int HSV[2][3])
{
    Mat imgHSV;
    Mat dst;
    cvtColor(src, imgHSV, CV_BGR2HSV);
    inRange(imgHSV, Scalar(HSV[0][0], HSV[0][1], HSV[0][2]), Scalar(HSV[1][0], HSV[1][1], HSV[1][2]), dst);
    blur(dst, dst, Size(15, 15));
    threshold(dst, dst, 200, 255, THRESH_BINARY);
    return dst;
}


Mat Camera::edgeThreshold(Mat src, Scalar Color, int piece)
{
    bluex.clear();
    bluey.clear();
    greenx.clear();
    greeny.clear();
    bluewidth.clear();
    blueheight.clear();
    greenwidth.clear();
    greenheight.clear();
    contours.clear();
    hierarchy.clear();

    Canny(src, canny_output, 100, 300, 3);
    findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
    vector<RotatedRect> minRect(contours.size());
    vector<Moments> mu(contours.size());

    for( int i = 0; i < contours.size(); i++ )
    {
	minRect[i] = minAreaRect(Mat(contours[i]));
	mu[i] = moments(contours[i], false); 
    }

    vector<Point2f> mc(contours.size());
    for(int i = 0; i < contours.size(); i++)
    {
	mc[i] = Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00);
    }

    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );

    for( int i = 0; i < contours.size(); i++ )
    {
	double x1 = 0;
	double y1 = 0;
	double x2 = 9999;
        double y2 = 9999;
        Point2f rect_points[4];
	minRect[i].points(rect_points);	

	if(arcLength(contours[i], true) < 50)
	{
	    continue;
	}
	else
	{

	//    drawContours(drawing, contours, i, Color, 1, 8, vector<Vec4i>(), 0, Point());
	    double w=0;
	    double h=0;
	    for(int j=0; j<4; j++)
	    {
		line(drawing, rect_points[j], rect_points[(j+1)%4], Color, 1, 4);
		if(x1 < rect_points[j].x)
		    x1 = rect_points[j].x;
		if(x2 > rect_points[j].x)
		    x2 = rect_points[j].x;
		if(y1 < rect_points[j].y)
		    y1 = rect_points[j].y;
		if(y2 > rect_points[j].y)
		    y2 = rect_points[j].y;
		w = x1 - x2;
		h = y1 - y2;
	    }
	    if(piece == P_BLUE)
	    {
		bluex.push_back(mc[i].x);
		bluey.push_back(mc[i].y);
		bluewidth.push_back(w);
		blueheight.push_back(h);
	    }
	    else if(piece == P_GREEN)
	    {
		greenx.push_back(mc[i].x);
		greeny.push_back(mc[i].y);
		greenwidth.push_back(w);
		greenheight.push_back(h);
	    }
	 }
    }
    return drawing;  
}

vector<double> Camera::getX(int piece)
{
    if(piece == P_BLUE)
    {
	return bluex;
    }
    else if(piece == P_GREEN)
    {
	return greenx;
    }
}
vector<double> Camera::getY(int piece)
{
    if(piece == P_BLUE)
    {
	return bluey;
    }
    else if(piece == P_GREEN)
    {
	return greeny;
    }
}
vector<double> Camera::getWidth(int piece)
{
    if(piece == P_BLUE)
    {
	return bluewidth;
    }
    else if(piece == P_GREEN)
    {
	return greenwidth;
    }
}
vector<double> Camera::getHeight(int piece)
{
    if(piece == P_BLUE)
    {
	return blueheight;
    }
    else if(piece == P_GREEN)
    {
	return greenheight;
    }
}
