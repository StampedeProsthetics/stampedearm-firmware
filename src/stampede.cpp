/*
  stampede.cpp - Library for The Stampede Arm prosthetic arm.
  Created by Chase Cromwell, (C) 2016-18.
  All Rights Reserved
  thestampedearm.org
*/


#include "Arduino.h"
#include <SoftwareSerial.h>
#include <Servo.h>
#include "stampede.h"
#include "stepper.h"
// Constructor
arm::arm(int board, int *pins1, int *pins2, int *pins3, int *pins4, int *pins5, boolean enableFeedbackin)
{
  motor1 = new StepperMotor(pins1[0], pins1[1], pins1[2], pins1[3]);
  motor2 = new StepperMotor(pins2[0], pins2[1], pins2[2], pins2[3]);
  motor3 = new StepperMotor(pins3[0], pins3[1], pins3[2], pins3[3]);
  motor4 = new StepperMotor(pins4[0], pins4[1], pins4[2], pins4[3]);
  motor5 = new StepperMotor(pins5[0], pins5[1], pins5[2], pins5[3]);
  enableFeedback = enableFeedbackin;
  board = board;

}
// Initalize Servo Objects
void arm::setupfingers()
{
  Servo finger1;
  Servo finger2;
  Servo finger3;
  Servo finger4;
  Servo finger5;
}
// Attach Servo Objects to Corresponding Pins
void arm::fingerattach(int finger1pin, int finger2pin, int finger3pin, int finger4pin, int finger5pin)
{
  finger1.attach(finger1pin);
  finger2.attach(finger2pin);
  finger3.attach(finger3pin);
  finger4.attach(finger4pin);
  finger5.attach(finger5pin);
}
// Move finger x by x degrees and back to the starting point
// Switch/Case loop run to determine finger to move.
// The whole setup for the servos and then looping to select the right one is probably inefficent and could be optimized, but it works for now.
void arm::fingermove(int finger, int fingermovevalue)
{

  int i = 0;
  switch (finger) {
    case 1:
      // The pair of for loops moves the finger to the instructed value and back, smoothly
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

// "Preset" finger function for demonstrations
// Servos can only be moved one at a time so,
// This function moves finger1 a degree, then finger2 a degree etc and loops until they're at the instructed value.
// Then the whole thing is run again backwards
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
// Begin the serial for the bluetooth and send initalization info. (This is where one time diagnostic info to be sent at startup should be.
void arm::btsetup() {
  //bt.begin(115200);
  bt.begin(57600);
  bt.println("This demo expects 2 pieces of data - text, and a int decimal");
  bt.println("Enter data in this style <HelloWorld, 24.7>  ");
  bt.println();
}

// Battery Testing Code
// Simply loops all motors back and forth until power cuts out.
void arm::batloop() {
    int i = 0;
  for (i = 0; i <= 180; i += 5) {
    finger1.write(i);
    finger2.write(i);
    finger3.write(i);
    finger4.write(i);
    finger5.write(i);
        motor1->movedegree(i);
    motor2->movedegree(i);
    motor3->movedegree(i);
    motor4->movedegree(i);
    motor5->movedegree(i);
    delay(15);
  }
  for (i = 180; i >= 0; i -= 5) {
    finger1.write(i);
    finger2.write(i);
    finger3.write(i);
    finger4.write(i);
    finger5.write(i);
        motor1->movedegree(i);
    motor2->movedegree(i);
    motor3->movedegree(i);
    motor4->movedegree(i);
    motor5->movedegree(i);
    delay(15);
  }
}

// Bluetooth Code for the Loop
// Recieves Info, parses info if its new, and then sets newinfo = false
void arm::btloop() {
  btrecvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    // this temporary copy is necessary to protect the original data
    // because strtok() used in parseData() replaces the commas with \0
    btparseData();
    btshowParsedData();
    newData = false;
  }
}

// Heart of the signal loop. Recieves data and marks segments
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

// Spinal Cord of Signal Loop. Separates components of input into separate variables.
void arm::btparseData() {      // split the data into its parts

  char * strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(tempChars, ",");     // get the first part - the string
  strcpy(messageForArm, strtokIndx); // copy it to messageForArm

  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  intForArm = atof(strtokIndx);     // convert this part to int

}

// Sends response data on the latest movement if feedback is enabled in setup. (Debug tool)
void arm::feedback(String messageForArm, int intForArm = 0) {
  if (enableFeedback) {
    if (intForArm != 0){
      bt.print("The message was '");
      bt.print(messageForArm);
      bt.print("' and the int was '");
      bt.print(intForArm);
      bt.println("'");
    } else {
      bt.print(messageForArm);
    }
  } else {
    //Feedback disabled
  }
}

// Brain of the Signal Loop. Takes separated values and decides what to do with them.
/*
This function could be refactored in a number of ways. Using strings and a series of if statements uses a lot of data and likely takes way too much time (oportunity for testing)

It's possible to use a switch and have functions as integers, saving data. The biggest downside to that is decreased readability.

But, all of this is unnececary in the long run if we're really end game focused once we switch to a electroencephalogram based system. My biggest concern is figuring out how to get that much programming power. Switching to a RPi or x64 based system will increase latency and decrease dependability. The biggest problem is finding a way to effectively process that much data in a fast, reliable fashion with low power and heat requirements. (Forgetting cost for the moment)

I need a simple CNN framework for the RasPi Zero. The zero's cost and added wireless functionality make it an ideal candidate. The CNN framework should be front and center for the next year. None of this C#/Arduino code means anything if it is just getting ported to the Pi platform.

*/

void arm::btshowParsedData() {
  messageForArmStr = String(messageForArm);
  if (intForArm == 1234.56) {
    bt.print("\n");
    bt.println("This demo expects 2 pieces of data - text, and a int decimal");
    bt.println("Enter data in this style <HelloWorld, 24.7>  ");
    bt.println();
  } else if (messageForArmStr == "this") {
  } else if (messageForArmStr == "f1") {
    feedback(messageForArm, intForArm);
    fingermove(1, intForArm);
  } else if (messageForArmStr == "f2") {
    feedback(messageForArm, intForArm);
    fingermove(2, intForArm);
  } else if (messageForArmStr == "f3") {
    feedback(messageForArm, intForArm);
    fingermove(3, intForArm);
  } else if (messageForArmStr == "f4") {
    feedback(messageForArm, intForArm);
    fingermove(4, intForArm);
  } else if (messageForArmStr == "f5") {
    feedback(messageForArm, intForArm);
    fingermove(5, intForArm);
  } else if (messageForArmStr == "wiggle") {
    feedback(messageForArm, intForArm);
    wiggle(180);
  } else if (messageForArmStr == "stepper1") {
    feedback(messageForArm, intForArm);
    motor1->movedegree(intForArm);
  } else if (messageForArmStr == "stepper2") {
    feedback(messageForArm, intForArm);
    motor2->movedegree(intForArm);
  } else if (messageForArmStr == "steppers") {
    feedback(messageForArm, intForArm);
    motor1->movedegree(intForArm);
    motor2->movedegree(intForArm);
  }
}

void arm::report(String requestinput) {
  request = requestinput.toLowerCase();
  if (request == "battery"){
    //feedback("Battery Status is at: " + "*testing value*");
  } else if (request == ""){
    feedback("There was no request payload!");
  } else if (request == "version"){
    feedback("The current Stampede Arm Firmware Version is: " + version);
	feedback("Last update was " + updated);
	feedback("(c) 2018 Chase Cromwell");
  } else if (request == "uptime"){
	  //Possible use of some sort of clock to update?
  } else if (request == "settings"){
	  //Print out detailed settings list
	  //Values of config settings, pin numberings, etc etc
	  feedback(("Version:" + version));
	  feedback(("This version was last updated: " + updated));
	  feedback(("This board was compiled for the " + board + "board"));
	  feedback(("This board is fully supported: " + boardBt));
	  feedback(("This board has bluetooth enabled: " + boardBt));
	  feedback("Uptime is: "."*testing*");
	  feedback(("Feedback enabled: " + enableFeedback));
	  feedback("(c) 2018 Chase Cromwell");

  }


}
