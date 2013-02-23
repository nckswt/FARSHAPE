#!/usr/bin/env python

import time
import os

STEP = 1
DELAY = 0.5

def pwm(pin, value):
    print "motor[" + str(pin) + "][" + str(value) + "]"
    cmd = "echo " + str(pin) + "=" + str(value) + " > /dev/servoblaster"
    os.system(cmd)
 
while True:
    for j in range(100, 200, STEP):
        pwm(1,j)   
        pwm(2,300-j)
	time.sleep(DELAY)
    for j in range(100, 200, (STEP*-1)):
        pwm(1,j)
        pwm(2,300-j)
        time.sleep(DELAY)
