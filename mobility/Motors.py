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
import math

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

    #output to motor control file
    def setMotors(self,leftMotorSpeed,rightMotorSpeed):
        f = open("/dev/servoblaster", "w")
        f.write(str(self.leftMotor) + "=" + str(150 + leftMotorSpeed) + "\n")
        f.write(str(self.rightMotor) + "=" + str(150 - rightMotorSpeed) + "\n")
        f.close()

    #move a set distance (positive for forward, negative for backward)
    def goDistance(self, speed, distance):
        if (distance > 0):
            self.setMotors(speed,speed)
        else:
            self.setMotors(-speed,-speed)
        time.sleep(math.fabs(distance))
        self.setMotors(0,0)

    #rotate a set angle (positive for counterclockwise, right for clockwise)
    def rotateAngle(self, speed, angle):
        if (angle > 0):
            self.setMotors(-speed,speed)
        else:
            self.setMotors(speed,-speed)
        time.sleep(math.fabs(angle))
        self.setMotors(0,0)

    #Go straight ahead
    def goForward(self, speed):
        self.setMotors(speed,speed)

    #Go straight backwards
    def goBackward(self, speed):
        self.setMotors(-speed,-speed)

    #Turn left while moving forward
    def turnLeft(self, speed):
        self.setMotors(speed/3,speed)

    #Turn right while moving forward
    def turnRight(self, speed):
        self.setMotors(speed,speed/3)

    #Pivot about left wheel
    def pivotLeft(self, speed):
        self.setMotors(0,speed)

    #Pivot about right wheel
    def pivotRight(self, speed):
        self.setMotors(speed,0)

    #Rotate left (CCW) about center of rotation
    def rotateLeft(self, speed):
        self.setMotors(-speed,speed)

    #Rotate right (CW) about center of rotation
    def rotateRight(self, speed):
        self.setMotors(speed,-speed)

    #Rotate right (CW) about center of rotation
    def brake(self):
        self.setMotors(0,0)

motors = Motors()
motors.setup()
timestep = 0.1 # seconds

# test motor commands
for i in range(50):
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
    time.sleep(timestep)

#move back and forth
motors.goForward(30)
time.sleep(1)
motors.goBackward(30)
time.sleep(1)
motors.brake()

#move distance and angle
motors.goDistance(15,1)
motors.goDistance(15,-1)
motors.rotateAngle(15,3)
motors.rotateAngle(15,-3)

motors.teardown()
