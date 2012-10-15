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

//Blue: 105,116,64 --- 125,209,255
//Green: 53,90,32 --- 100,185,255
Mat Camera::ColorThreshold(Mat src, int HSV[2][3])
{
    Mat imgHSV;
    Mat dst;
    cvtColor(src, imgHSV, CV_BGR2HSV);
    inRange(imgHSV, Scalar(HSV[0][0], HSV[0][1], HSV[0][2]), Scalar(HSV[1][0], HSV[1][1], HSV[1][2]), dst);
    blur(dst, dst, Size(9, 9));
    threshold(dst, dst, 200, 255, THRESH_BINARY);
    return dst;
}

Mat Camera::edgeThreshold(Mat src, Scalar Color)
{

    x.clear();
    contours.clear();
    hierarchy.clear();
    Mat canny_output;

    Canny( src, canny_output, 100, 300, 3 );
    findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0) );
    vector<RotatedRect> minRect( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
    {
	minRect[i] = minAreaRect( Mat(contours[i]) );
    }
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( int i = 0; i < contours.size(); i++ )
    {
        Point2f rect_points[4];
	minRect[i].points( rect_points );
        for( int j = 0; j < 4; j++ )
	    line( drawing, rect_points[j], rect_points[(j+1)%4], Color, 1, 8 );
    }

  /*      
    Canny(src, canny_output, 100, 300, 3);
    findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0,0));

    vector<Point2f> mc(contours.size());
    vector<Moments> mu(contours.size());
   
   
    
    for(int i=0; i<contours.size(); i++)
	mu[i] = moments(contours[i], false);

    for(int i=0; i<contours.size(); i++)
 
	mc[i] = Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00);
    }

    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for(int i=0; i<contours.size(); i++)
    {
	drawContours(drawing, contours, i, Color, 1, 4, hierarchy, 0, Point());
	//circle(drawing, mc[i], 1, Scalar(255,255,255), -1, 8, 0);
	//x.push_back(mc[i].x);
    }
*/
    return drawing;  
}
/*
Mat Camera::drawRect()
{
    vector<vector<Point> > contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());
    
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );

    for(int i=0; i<contours.size(); i++)
    {
	approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
	boundRect[i] = boundingRect( Mat(contours_poly[i]) );
//	minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
    }
	
    Mat drawing = Mat::zeros( canny_output.size(),  CV_8UC3 );
    for(int i=0; i<contours.size(); i++)
    {
//	drawContours( drawing, contours_poly, i, Scalar(255,255,255), 1, 8, vector<Vec4i>(), 0, Point() );
	rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(255,255,255), 2, 8, 0 );
    }
    return drawing;
}*/

Mat Camera::drawRect(Mat src)
{
    Mat threshold_output;
    
    threshold(src, threshold_output, 100, 255, THRESH_BINARY);
    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );    
    vector<RotatedRect> minRect( contours.size() );
    vector<RotatedRect> minEllipse( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    {
	minRect[i] = minAreaRect( Mat(contours[i]) );
	if( contours[i].size() > 5 )
	{
	    minEllipse[i] = fitEllipse( Mat(contours[i]));
	}
    }
    
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
	drawContours( drawing, contours, i, Scalar(255,255,255), 1, 8, vector<Vec4i>(), 0, Point() );
	Point2f rect_points[4]; 
	minRect[i].points( rect_points );
        for( int j = 0; j < 4; j++ )
            line( drawing, rect_points[j], rect_points[(j+1)%4], Scalar(255,255,255), 1, 8 );
    }
    return drawing;
}

vector<int> Camera::getX()
{
    return x;
}
