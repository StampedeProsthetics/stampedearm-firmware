/*
  stampedearm-firmware.ino - Library for The Stampede Arm prosthetic arm.
  Created by Chase Cromwell, (C) Fall 2016-18.
  All Rights Reserved
  thestampedearm.org
*/

#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include "stepper.h"
#include "stampede.h"
int stepper1[] = {32, 31, 30, 29};
int stepper2[] = {28, 27, 26, 25};
int stepper3[] = {11, 10, 9, 8};
int stepper4[] = {14, 15, 16, 17};
int stepper5[] = {18, 19, 20, 21};
arm stampede = arm(stepper1, stepper2, stepper3, stepper4, stepper5, true);

void setup()
{
  stampede.btsetup();
  stampede.setupfingers();
  stampede.fingerattach(2, 3, 4, 5, 6);

}

void loop()
{
  stampede.btloop();
  //stampede.batloop();
}
