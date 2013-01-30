#!/usr/bin/env python

#Richard G. Hirst's ServoBlaster available at https://github.com/richardghirst/PiBits/tree/master/ServoBlaster
#pulsewidth: 1000microseconds for full speed ahead, 
#            2000microseconds for full speed reverse, 
#            1500microseconds for idling and 
#            200microseconds for brake
#note that Servoblast provides 10microsecond pulse widths, so writing "100" to /dev/servoblaster gives full speed ahead


import time
import os
import subprocess
import re

class Motors:
    "Motor Drivers using Servoblaster for Vex Robotics 393 motors via Vex Motor Controller 29's"
    def __init__(self):
        self.leftMotor = 2 #pin 2 in servoblaster, GPIO 18 and Pin P1-12 on the RPi
        self.rightMotor = 1 #pin 1 in servoblaster, GPIO 17 and Pin P1-11 on the RPi
        speed = 0 #from 0-50 in arbitrary units <>TODO: TRANSLATE TO ACTUAL SPEED


    #make sure Servoblaster daemon is running
    def setup(self):
        s = subprocess.Popen(["ps","axw"],stdout=subprocess.PIPE)
        for x in s.stdout:
            if re.search("servod",x):
                print "Servoblaster Daemon is already running"
            else:
                cmd = "sudo ./ServoBlaster/servod"
                os.system(cmd)
                break

    #close Servoblaster daemon
    def teardown(self):
        s = subprocess.Popen(["ps","axw"],stdout=subprocess.PIPE)
        for x in s.stdout:
            if re.search("servod",x):
                print "Servoblaster Daemon has been shut down"
                os.system("sudo killall servod")
                break


    #Go straight ahead
    def goForward(self, speed):
        cmd = "echo " + str(self.leftMotor) + "=" + str(150 + speed) + " > /dev/servoblaster"
        os.system(cmd)
        cmd = "echo " + str(self.rightMotor) + "=" + str(150 - speed) + " > /dev/servoblaster"
        os.system(cmd)

    #Go straight backwards
    def goBackward(self, speed):
        cmd = "echo " + str(self.leftMotor) + "=" + str(150 - speed) + " > /dev/servoblaster"
        os.system(cmd)
        cmd = "echo " + str(self.rightMotor) + "=" + str(150 + speed) + " > /dev/servoblaster"
        os.system(cmd)

    #Turn left while moving forward
    def turnLeft(self, speed):
        cmd = "echo " + str(self.leftMotor) + "=" + str(150 + speed/3) + " > /dev/servoblaster"
        os.system(cmd)
        cmd = "echo " + str(self.rightMotor) + "=" + str(150 - speed) + " > /dev/servoblaster"
        os.system(cmd)

    #Turn right while moving forward
    def turnRight(self, speed):
        cmd = "echo " + str(self.leftMotor) + "=" + str(150 + speed) + " > /dev/servoblaster"
        os.system(cmd)
        cmd = "echo " + str(self.rightMotor) + "=" + str(150 - speed/3) + " > /dev/servoblaster"
        os.system(cmd)

    #Pivot about left wheel
    def pivotLeft(self, speed):
        cmd = "echo " + str(self.leftMotor) + "=" + str(150) + " > /dev/servoblaster"
        os.system(cmd)
        cmd = "echo " + str(self.rightMotor) + "=" + str(150 - speed) + " > /dev/servoblaster"
        os.system(cmd)

    #Pivot about right wheel
    def pivotRight(self, speed):
        cmd = "echo " + str(self.leftMotor) + "=" + str(150 + speed) + " > /dev/servoblaster"
        os.system(cmd)
        cmd = "echo " + str(self.rightMotor) + "=" + str(150) + " > /dev/servoblaster"
        os.system(cmd)

    #Rotate left (CCW) about center of rotation
    def rotateLeft(self, speed):
        cmd = "echo " + str(self.leftMotor) + "=" + str(150 - speed) + " > /dev/servoblaster"
        os.system(cmd)
        cmd = "echo " + str(self.rightMotor) + "=" + str(150 - speed) + " > /dev/servoblaster"
        os.system(cmd)

    #Rotate right (CW) about center of rotation
    def rotateRight(self, speed):
        cmd = "echo " + str(self.leftMotor) + "=" + str(150 + speed) + " > /dev/servoblaster"
        os.system(cmd)
        cmd = "echo " + str(self.rightMotor) + "=" + str(150 + speed) + " > /dev/servoblaster"
        os.system(cmd)

    #Rotate right (CW) about center of rotation
    def brake(self):
        cmd = "echo " + str(self.leftMotor) + "=" + str(20) + " > /dev/servoblaster"
        os.system(cmd)
        cmd = "echo " + str(self.rightMotor) + "=" + str(20) + " > /dev/servoblaster"
        os.system(cmd)

motors = Motors()
motors.setup()
timestep = 0.1 # seconds
"""for i in range(50):
    motors.goForward(i)
    time.sleep(timestep)

for i in range(50):
    motors.goBackward(i)
    time.sleep(timestep)

for i in range(50):
    motors.turnRight(i)
    time.sleep(timestep)

for i in range(50):
    motors.turnLeft(i)
    time.sleep(timestep)

for i in range(50):
    motors.pivotRight(i)
    time.sleep(timestep)

for i in range(50):
    motors.pivotLeft(i)
    time.sleep(timestep)

for i in range(50):
    motors.rotateRight(i)
    time.sleep(timestep)

for i in range(50):
    motors.rotateLeft(i)
    time.sleep(timestep)"""

motors.goForward(30)
time.sleep(1)
motors.goBackward(30)
time.sleep(1)
motors.brake()

motors.teardown()
