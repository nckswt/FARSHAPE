#!/usr/bin/python

### Note: you will need to SSH tunnel X for this to work
###       as it pops open a window

# import opencv
import cv
import time

cv.NamedWindow('a_window', 320)

capture1=cv.CaptureFromCAM(0)

def repeat():
	image=cv.QueryFrame(capture1)
	cv.ShowImage('a_window', image) #Show window

while True:
	repeat()
	time.sleep(10)
