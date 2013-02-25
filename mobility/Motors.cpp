#include <time.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <cmath>
#include "Motors.h"

#define MICROSEC2SEC 1000000

using namespace std;

Motors::Motors(){
    leftMotor = '2'; //pin 2 in servoblaster, GPIO 18 and Pin P1-12 on the RPi
    rightMotor = '1'; //pin 1 in servoblaster, GPIO 17 and Pin P1-11 on the RPi
    if (0 == system("pidof 0x servod > /dev/null")){
        cout << "ServoBlaster Daemon is already running!" << endl;
    } else{
        system("sudo ./ServoBlaster/servod");
    }
}

Motors::~Motors(){
    if (0 == system("pidof 0x servod > /dev/null")){
        system("sudo killall servod");
        cout << "ServoBlaster Daemon has been shut down!" << endl;
    }   
}

void Motors::setMotors(int leftMotorSpeed, int rightMotorSpeed){
    ofstream f;
    f.open("/dev/servoblaster", ios::trunc | ios::out);
    f << this->leftMotor << "=" << 150 + leftMotorSpeed << '\n';
    f << this->rightMotor << "=" << 150 - rightMotorSpeed << '\n';
    f.close();
}

//Go straight ahead
void Motors::goForward(int speed){
    this->setMotors(speed,speed);
}

//Go straight backwards
void Motors::goBackward(int speed){
    this->setMotors(-speed,-speed);
}

//Turn left while moving forward
void Motors::turnLeft(int speed){
    this->setMotors(speed/3,speed);
}

//Turn right while moving forward
void Motors::turnRight(int speed){
    this->setMotors(speed,speed/3);
}
//Pivot about left wheel

void Motors::pivotLeft(int speed){
    this->setMotors(0,speed);
}

//Pivot about right wheel
void Motors::pivotRight(int speed){
    this->setMotors(speed,0);
}

//Rotate left (CCW) about center of rotation
void Motors::rotateLeft(int speed){
    this->setMotors(-speed,speed);
}

//Rotate right (CW) about center of rotation
void Motors::rotateRight(int speed){
    this->setMotors(speed,-speed);
}

//Rotate right (CW) about center of rotation
void Motors::brake(){
    this->setMotors(0,0);
}

/*int main(){
    Motors motors;
    double timestep = 0.1; // seconds

    // test motor commands
    for (int i = 0; i<50; i++){
        motors.goForward(i);
        usleep(MICROSEC2SEC *timestep);
    }

    for (int i = 0; i<50; i++){
        motors.goBackward(i);
        usleep(MICROSEC2SEC *timestep);
    }

    for (int i = 0; i<50; i++){
        motors.turnRight(i);
        usleep(MICROSEC2SEC *timestep);
    }

    for (int i = 0; i<50; i++){
        motors.turnLeft(i);
        usleep(MICROSEC2SEC *timestep);
    }

    for (int i = 0; i<50; i++){
        motors.pivotRight(i);
        usleep(MICROSEC2SEC *timestep);
    }

    for (int i = 0; i<50; i++){
        motors.pivotLeft(i);
        usleep(MICROSEC2SEC *timestep);
    }

    for (int i = 0; i<50; i++){
        motors.rotateRight(i);
        usleep(MICROSEC2SEC *timestep);
    }

    for (int i = 0; i<50; i++){
        motors.rotateLeft(i);
        usleep(MICROSEC2SEC *timestep);
    }

    //move back and forth
    motors.goForward(30);
    usleep(MICROSEC2SEC *1);
    motors.goBackward(30);
    usleep(MICROSEC2SEC *1);
    motors.brake();

    //move distance and angle
    motors.goDistance(15,1);
    motors.goDistance(15,-1);
    motors.rotateAngle(15,3);
    motors.rotateAngle(15,-3);
    motors.goDistance(20,1);
    motors.goDistance(20,-1);
    motors.rotateAngle(20,1);
    motors.rotateAngle(20,-2);
    motors.rotateAngle(20,1);

    return 0;
}*/
