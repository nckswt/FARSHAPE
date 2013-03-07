#!/usr/bin/python

### Note: you will need to SSH tunnel X for this to work
###       as it pops open a window

import cv

capture=cv.CaptureFromCAM(0)
count=0
#Load the haar cascade
#hc = cv.Load("/opt/ros/fuerte/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml")
#hc = cv.Load("/usr/share/opencv/haarcascades/haarcascade_frontalface_default.xml")

def repeat():
	global count
	image=cv.QueryFrame(capture)
	#Detect face in image
#	face = cv.HaarDetectObjects(image, hc, cv.CreateMemStorage())
#	for (x,y,w,h),n in face:
#		cv.Rectangle(image, (x,y), (x+w,y+h), 255)
#		print 'face found at: '+str(w)+','+str(h)
#	cv.ShowImage('Image_Window',image)
	cv.SaveImage('image_' + str(count) + '.jpeg', image)
	cv.WaitKey(250)
	count+=1


while True:
	repeat()
