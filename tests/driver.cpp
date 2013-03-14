#include "Helm.h"
#include "../sensing/camera.h"

int main {
  
  Helm wesley();
  
    Camera cam;
    vector<int> x;
    while(1)
   {
	imshow("blabla", cam.edgeThreshold((cam.blueThreshold(cam.streamVid()))));

	x = cam.getX();
	for(int i=0; i<x.size(); i++)
	{
	    if(x.at(i) < 250){		//TODO careful for NaN
		wesley.rotateLeft();
	    }
	    else if(x.at(i) > 350){
		wesley.rotateRight();
	    }
	    else if(x.at(i) >= 250 && x.at(i) <= 350) {
		printf("move forward\n");
		wesley.stop();
		wesley.goForward();
	    }
	    else {
	      wesley.stop();
	    }

	}
	if(waitKey(30) >= 0)
	    break;
    }
}