/*
  stampede.cpp - Library for The Stampede Arm prosthetic arm.
  Created by Chase Cromwell, Fall 2017.
  thestampedearm.org
*/

#include "Arduino.h"
#include <Servo.h>
#include "stampede.h"
StepperMotor::StepperMotor(int In1, int In2, int In3, int In4){
    // Record pin numbers in the inputPins array
    this->inputPins[0] = In1;
    this->inputPins[1] = In2;
    this->inputPins[2] = In3;
    this->inputPins[3] = In4;

    // Iterate through the inputPins array, setting each one to output mode
    for(int inputCount = 0; inputCount < 4; inputCount++){
        pinMode(this->inputPins[inputCount], OUTPUT);
    }
    duration = 50;
}

void StepperMotor::setStepDuration(int duration){
    this->duration = duration;
}

void StepperMotor::step(int noOfSteps){
    /*
        The following 2D array represents the sequence that must be
        used to acheive rotation. The rows correspond to each step, and
        the columns correspond to each input. L
    */
    bool sequence[][4] = {{LOW, LOW, LOW, HIGH },
                          {LOW, LOW, HIGH, HIGH},
                          {LOW, LOW, HIGH, LOW },
                          {LOW, HIGH, HIGH, LOW},
                          {LOW, HIGH, LOW, LOW },
                          {HIGH, HIGH, LOW, LOW},
                          {HIGH, LOW, LOW, LOW },
                          {HIGH, LOW, LOW, HIGH}};
                      
    int factor = abs(noOfSteps) / noOfSteps;    // If noOfSteps is +, factor = 1. If noOfSteps is -, factor = -1 
    noOfSteps = abs(noOfSteps);    // If noOfSteps was in fact negative, make positive for future operations

    /* 
        The following algorithm runs through the sequence the specified number 
        of times
    */
    for(int sequenceNum = 0;  sequenceNum <= noOfSteps/8; sequenceNum++){
        for(int position = 0; ( position < 8 ) && ( position < ( noOfSteps - sequenceNum*8 )); position++){
            delay(duration);
            for(int inputCount = 0; inputCount < 4; inputCount++){
                digitalWrite(this->inputPins[inputCount], sequence[(int)(3.5 - (3.5*factor) + (factor*position))][inputCount]);
            }
        } 
    }
}
void StepperMotor::degree(int noOfDeg) {
step((noOfDeg*4096)/360);
}
// Constructor
arm::arm()
{
HardwareSerial bt = HardwareSerial();
//Serial Input/Output Variables
static const int numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];// temporary array for use when parsing
char messageForArm[32] = {0};
int intForArm = 0.0;
boolean newData = false;
String messageForArmStr;
StepperMotor motor(32,31,30,29);
}
void arm::setupfingers()
{
    Servo finger1;
    int finger1pos = 0;
    Servo finger2;
    int finger2pos = 0;
    Servo finger3;
    int finger3pos = 0;
    Servo finger4;
    int finger4pos = 0;
    Servo finger5;
    int finger5pos = 0;
}
void arm::fingerattach(int finger1pin, int finger2pin, int finger3pin, int finger4pin, int finger5pin)
{
    finger1.attach(finger1pin);
    finger2.attach(finger2pin);
    finger3.attach(finger3pin);
    finger4.attach(finger4pin);
    finger5.attach(finger5pin);
    motor.setStepDuration(1);
}

void arm::fingermove(int finger, int fingermovevalue)
{
    if (finger == 1) {
        int i = 0;
        for (i = 0; i <= fingermovevalue; i += 1) {
            finger1.write(i);
            delay(15);
        }
        for (i = fingermovevalue; i >= 0; i -= 1) {
            finger1.write(i);
            delay(15);
        }
    }
	 if (finger == 2) {
        int i = 0;
        for (i = 0; i <= fingermovevalue; i += 1) {
            finger2.write(i);
            delay(15);
        }
        for (i = fingermovevalue; i >= 0; i -= 1) {
            finger2.write(i);
            delay(15);
        }
    }
	 if (finger == 3) {
        int i = 0;
        for (i = 0; i <= fingermovevalue; i += 1) {
            finger3.write(i);
            delay(15);
        }
        for (i = fingermovevalue; i >= 0; i -= 1) {
            finger3.write(i);
            delay(15);
        }
    }
	 if (finger == 4) {
        int i = 0;
        for (i = 0; i <= fingermovevalue; i += 1) {
            finger4.write(i);
            delay(15);
        }
        for (i = fingermovevalue; i >= 0; i -= 1) {
            finger4.write(i);
            delay(15);
        }
    }
	 if (finger == 5) {
        int i = 0;
        for (i = 0; i <= fingermovevalue; i += 1) {
            finger5.write(i);
            delay(15);
        }
        for (i = fingermovevalue; i >= 0; i -= 1) {
            finger5.write(i);
            delay(15);
        }
    }
}
void arm::wiggle(int fingermovevalue)
{
        int i = 0;
        for (i = 0; i <= fingermovevalue; i += 1) {
            finger1.write(i);
            finger2.write(i);
            finger3.write(i);
            finger4.write(i);
            finger5.write(i);			
            delay(15);
        }
        for (i = fingermovevalue; i >= 0; i -= 1) {
            finger1.write(i);
            finger2.write(i);
            finger3.write(i);
            finger4.write(i);
            finger5.write(i);
            delay(15);
        }
}
/* This point forward is all bluetooth and command parsing code. Do NOT put actual servo.write style commands beyond this line */

void arm::btsetup() {
  bt.begin(115200);
  bt.println("This demo expects 2 pieces of data - text, and a int decimal");
  bt.println("Enter data in this style <HelloWorld, 24.7>  ");
  bt.println();
}

//============

void arm::btloop() {
  btrecvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    //this temporary copy is necessary to protect the original data
    //because strtok() used in parseData() replaces the commas with \0
    btparseData();
    btshowParsedData();
    newData = false;
  }
}

//============

void arm::btrecvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (bt.available() > 0 && newData == false) {
    rc = bt.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

//============

void arm::btparseData() {      // split the data into its parts

  char * strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(tempChars, ",");     // get the first part - the string
  strcpy(messageForArm, strtokIndx); // copy it to messageForArm

  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  intForArm = atof(strtokIndx);     // convert this part to int

}

//============

void arm::btshowParsedData() {
  messageForArmStr = String(messageForArm);
  if (intForArm == 1234.56) {
    bt.print("\n");
    bt.println("This demo expects 2 pieces of data - text, and a int decimal");
    bt.println("Enter data in this style <HelloWorld, 24.7>  ");
    bt.println();
  } else if (messageForArmStr == "this") {
    bt.print("The message was '");
    bt.print(messageForArm);
    bt.print("' and the int was '");
    bt.print(intForArm);
	bt.println("'");
  } else if (messageForArmStr == "finger1") {
    bt.print("The message was '");
    bt.print(messageForArm);
    bt.print("' and the int was '");
    bt.print(intForArm);
	bt.println("'");
	fingermove(1, intForArm);
  } else if (messageForArmStr == "finger2") {
    bt.print("The message was '");
    bt.print(messageForArm);
    bt.print("' and the int was '");
    bt.print(intForArm);
	bt.println("'");
	fingermove(2, intForArm);
  } else if (messageForArmStr == "finger3") {
    bt.print("The message was '");
    bt.print(messageForArm);
    bt.print("' and the int was '");
    bt.print(intForArm);
	bt.println("'");
	fingermove(3, intForArm);
  } else if (messageForArmStr == "finger4") {
    bt.print("The message was '");
    bt.print(messageForArm);
    bt.print("' and the int was '");
    bt.print(intForArm);
	bt.println("'");
	fingermove(4, intForArm);
  } else if (messageForArmStr == "finger5") {
    bt.print("The message was '");
    bt.print(messageForArm);
    bt.print("' and the int was '");
    bt.print(intForArm);
	bt.println("'");
	fingermove(5, intForArm);
  } else if (messageForArmStr == "wiggle") {
    bt.print("The message was '");
    bt.print(messageForArm);
    bt.print("' and the int was '");
    bt.print(intForArm);
	bt.println("'");
	wiggle(180);
  } else if (messageForArmStr == "stepper1") {
    bt.print("The message was '");
    bt.print(messageForArm);
    bt.print("' and the int was '");
    bt.print(intForArm);
	bt.println("'");
	motor.degree(intForArm);
  }
}
