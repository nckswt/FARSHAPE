#ifndef _MOTORS_H_
#define _MOTORS_H_

/*Richard G. Hirst's ServoBlaster available at https://github.com/richardghirst/PiBits/tree/master/ServoBlaster
pulsewidth: 1000microseconds for full speed ahead, 
            2000microseconds for full speed reverse, 
            1500microseconds for idling and 
            200microseconds for brake
note that Servoblast provides 10microsecond pulse widths, so writing "100" to /dev/servoblaster gives full speed ahead*/

#define DEFAULTSPEED 30

class Motors{
private:
	char leftMotor; //pin 2 in servoblaster, GPIO 18 and Pin P1-12 on the RPi
	char rightMotor; //pin 1 in servoblaster, GPIO 17 and Pin P1-11 on the RPi
	void setMotors(int leftMotorSpeed, int rightMotorSpeed);

public:
	Motors();
	~Motors();
	void goForward(int speed = DEFAULTSPEED);
	void goBackward(int speed = DEFAULTSPEED);
	void rotateLeft(int speed = DEFAULTSPEED);
	void rotateRight(int speed = DEFAULTSPEED);
	void pivotLeft(int speed = DEFAULTSPEED);
	void pivotRight(int speed = DEFAULTSPEED);
	void turnLeft(int speed = DEFAULTSPEED);
	void turnRight(int speed = DEFAULTSPEED);
	void brake();
	void idle();

};

#endif