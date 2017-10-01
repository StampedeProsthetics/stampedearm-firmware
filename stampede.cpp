/*
  stampede.cpp - Library for The Stampede Arm prosthetic arm.
  Created by Chase Cromwell, Fall 2017.
  thestampedearm.org
*/

#include "Arduino.h"
#include <Servo.h>
#include "stampede.h"
#include "stepper.h"
// Constructor
arm::arm(int *pins1, int *pins2, int *pins3, int *pins4, int *pins5)
{
  motor1 = new StepperMotor(pins1[0], pins1[1], pins1[2], pins1[3]);
  motor2 = new StepperMotor(pins2[0], pins2[1], pins2[2], pins2[3]);
  motor3 = new StepperMotor(pins3[0], pins3[1], pins3[2], pins3[3]);
  motor4 = new StepperMotor(pins4[0], pins4[1], pins4[2], pins4[3]);
  motor5 = new StepperMotor(pins5[0], pins5[1], pins5[2], pins5[3]);

}
void arm::setupfingers()
{
  Servo finger1;
  Servo finger2;
  Servo finger3;
  Servo finger4;
  Servo finger5;
}
void arm::fingerattach(int finger1pin, int finger2pin, int finger3pin, int finger4pin, int finger5pin)
{
  finger1.attach(finger1pin);
  finger2.attach(finger2pin);
  finger3.attach(finger3pin);
  finger4.attach(finger4pin);
  finger5.attach(finger5pin);
}

void arm::fingermove(int finger, int fingermovevalue)
{

  int i = 0;
  switch (finger) {
    case 1:
      for (i = 0; i <= fingermovevalue; i += 1) {
        finger1.write(i);
        delay(15);
      }
      for (i = fingermovevalue; i >= 0; i -= 1) {
        finger1.write(i);
        delay(15);
      }
      break;
    case 2:
      for (i = 0; i <= fingermovevalue; i += 1) {
        finger2.write(i);
        delay(15);
      }
      for (i = fingermovevalue; i >= 0; i -= 1) {
        finger2.write(i);
        delay(15);
      }
      break;
    case 3:
      for (i = 0; i <= fingermovevalue; i += 1) {
        finger3.write(i);
        delay(15);
      }
      for (i = fingermovevalue; i >= 0; i -= 1) {
        finger3.write(i);
        delay(15);
      }
      break;
    case 4:
      for (i = 0; i <= fingermovevalue; i += 1) {
        finger4.write(i);
        delay(15);
      }
      for (i = fingermovevalue; i >= 0; i -= 1) {
        finger4.write(i);
        delay(15);
      }
      break;
    case 5:
      for (i = 0; i <= fingermovevalue; i += 1) {
        finger5.write(i);
        delay(15);
      }
      for (i = fingermovevalue; i >= 0; i -= 1) {
        finger5.write(i);
        delay(15);
      }
      break;
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
    motor1->movedegree(intForArm);
  } else if (messageForArmStr == "stepper2") {
    bt.print("The message was '");
    bt.print(messageForArm);
    bt.print("' and the int was '");
    bt.print(intForArm);
    bt.println("'");
    motor2->movedegree(intForArm);
  } else if (messageForArmStr == "steppers") {
    bt.print("The message was '");
    bt.print(messageForArm);
    bt.print("' and the int was '");
    bt.print(intForArm);
    bt.println("'");
    motor1->movedegree(intForArm);
    motor2->movedegree(intForArm);
  }
}
