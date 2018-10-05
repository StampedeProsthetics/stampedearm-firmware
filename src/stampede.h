/*
  stampede.h - Library for The Stampede Arm prosthetic arm.
  Created by Chase Cromwell, (C) 2016-18.
  All Rights Reserved
  thestampedearm.org
*/


#ifndef arm_h
#define arm_h

#include "Arduino.h"
#include <SoftwareSerial.h>
#include <Servo.h>
#include "stepper.h"

class arm {
  public:
	String version = "v0.0.0.1";
	String updated = "1 October 2018";
	int board;
    int i;
    arm(int *pins1, int *pins2, int *pins3, int *pins4, int *pins5, boolean enableFeedbackin = false);
    void setupfingers();
    void fingerattach(int finger1pin, int finger2pin, int finger3pin, int finger4pin, int finger5pin);
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
    #define bt Serial1
    //Serial Input/Output Variables
    static const int numChars = 32;
    char receivedChars[numChars];
    char tempChars[numChars];// temporary array for use when parsing
    char messageForArm[numChars] = {0};
    int intForArm = 0.0;
    boolean newData = false;
    String messageForArmStr;
    void batloop();
    void btsetup();
    void btloop();
    void btrecvWithStartEndMarkers();
    void btparseData();
    void btshowParsedData();
    void report(String request);
    boolean enableFeedback;
    void feedback(String messageForArmStr, int intForArm);
    StepperMotor* motor1;
    StepperMotor* motor2;
    StepperMotor* motor3;
    StepperMotor* motor4;
    StepperMotor* motor5;
	void getBoard(int bt);
	String requestinput;
	String request;
	void report(String requestinput);

};
#endif
