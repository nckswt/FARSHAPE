/*Richard G. Hirst's ServoBlaster available at https://github.com/richardghirst/PiBits/tree/master/ServoBlaster
pulsewidth: 1000microseconds for full speed ahead, 
            2000microseconds for full speed reverse, 
            1500microseconds for idling and 
            200microseconds for brake
note that Servoblast provides 10microsecond pulse widths, so writing "100" to /dev/servoblaster gives full speed ahead*/

class Motors{
private:
	char leftMotor; //pin 2 in servoblaster, GPIO 18 and Pin P1-12 on the RPi
	char rightMotor; //pin 1 in servoblaster, GPIO 17 and Pin P1-11 on the RPi
	void setMotors(int leftMotorSpeed, int rightMotorSpeed);

public:
	Motors();
	~Motors();
	void goDistance(int speed, double distance);
	void rotateAngle(int speed, double angle);
	void goForward(int speed);
	void goBackward(int speed);
	void rotateLeft(int speed);
	void rotateRight(int speed);
	void pivotLeft(int speed);
	void pivotRight(int speed);
	void turnLeft(int speed);
	void turnRight(int speed);
	void brake();
};

