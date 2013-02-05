#!/usr/bin/python

### Note: you will need to SSH tunnel X for this to work
###       as it pops open a window

import cv
capture=cv.CaptureFromCAM(1)

#Load the haar cascade
#hc = cv.Load("/opt/ros/fuerte/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml")

def repeat():
	image=cv.QueryFrame(capture)
	#Detect face in image
#	face = cv.HaarDetectObjects(image, hc, cv.CreateMemStorage())
#	for (x,y,w,h),n in face:
#		cv.Rectangle(image, (x,y), (x+w,y+h), 255)
#		print 'face found at: '+str(w)+','+str(h)
	cv.ShowImage('Image_Window',image)
	cv.WaitKey(50)


while True:
	repeat()

