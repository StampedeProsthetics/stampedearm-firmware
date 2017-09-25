/*
  stampede.cpp - Library for The Stampede Arm prosthetic arm.
  Created by Chase Cromwell, Fall 2017.
  thestampedearm.org
*/


#ifndef arm_h
#define arm_h

#include "Arduino.h"
#include <servo.h>
class StepperMotor {
public:
    StepperMotor(int In1, int In2, int In3, int In4);    // Constructor that will set the inputs
    void setStepDuration(int duration);    // Function used to set the step duration in ms
    void step(int noOfSteps);    // Step a certain number of steps. + for one way and - for the other
	void degree(int noOfDeg);
    int duration;    // Step duration in ms
    int inputPins[4];    // The input pin numbers
};

class arm {
public:
    int i;
    arm();
    void setupfingers();
    void fingerattach(int finger1pin, int finger2pin, int finger3pin, int finger4pin, int finger5pin);
    void setupmotors();
    void fingermove(int finger, int fingermovevalue);
	void wiggle(int fingermovevalue);
    int finger1pos;
    int finger2pos;
    int finger3pos;
    int finger4pos;
    int finger5pos;
    Servo finger1;
    Servo finger2;
    Servo finger3;
    Servo finger4;
    Servo finger5;
	//Bluetooth
	HardwareSerial bt = HardwareSerial();
	//Serial Input/Output Variables
	static const int numChars = 32;
	char receivedChars[numChars];
	char tempChars[numChars];// temporary array for use when parsing
	char messageForArm[numChars] = {0};
	int intForArm = 0.0;
	boolean newData = false;
	String messageForArmStr;
	void btsetup();
	void btloop();
	void btrecvWithStartEndMarkers();
	void btparseData();
	void btshowParsedData();
  StepperMotor motor = StepperMotor(32,31,30,29);
};
#endif
