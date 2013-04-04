#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <time.h>
#include "camera.h"

using namespace cv;
using namespace std; 

void timer(clock_t end)
{
    clock_t start, curr, stop;
    start = clock();
    while(1)
    {
	curr = clock();
	stop = (curr-start)/(CLOCKS_PER_SEC);
	if(stop >= end)
	    break;
    }
}
//TODO: get length of a bar
int main(int argc, char** argv)
{
    Camera cam;
    int blueHSV[2][3] = {{105,116,0},{125,209,255}};
    int greenHSV[2][3] = {{53,90,0},{100,185,255}};
    vector<int> x;
    vector<Mat> slices;

    while(1)
    {
	imshow("Blue", cam.edgeThreshold(cam.ColorThreshold(cam.streamVid(),blueHSV), Scalar(255,150,150)));
	imshow("Green", cam.edgeThreshold(cam.ColorThreshold(cam.streamVid(),greenHSV), Scalar(150,255,150)));
//	x = cam.getX();
//	for(int i=0; i<x.size(); i++)
//	{
//	    if(x.at(i) < 250)		//TODO careful for NaN
//		printf("move left\n");
//	    if(x.at(i) > 350)
//		printf("move right\n");
//	    if(x.at(i) >= 250 && x.at(i) <= 350)
//		printf("move forward\n");

//	    printf("x = %d\t\t", x.at(i));
//	    if(i == x.size() - 1)
//		cout<<endl;
//	}
	if(waitKey(30) >= 0)
	    break;
    }

    return 0;
}
