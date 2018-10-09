/*
  stampedearm-firmware.cpp - Library for The Stampede Arm prosthetic arm.
  Created by Chase Cromwell, (C) 2016-18.
  All Rights Reserved
  thestampedearm.org
*/

#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include "stepper.h"
#include "stampede.h"
int board = 1;
int stepper1[] = {32, 31, 30, 29};
int stepper2[] = {28, 27, 26, 25};
int stepper3[] = {11, 10, 9, 8};
int stepper4[] = {14, 15, 16, 17};
int stepper5[] = {18, 19, 20, 21};
arm stampede = arm(board, stepper1, stepper2, stepper3, stepper4, stepper5);

void setup()
{
	/*
	Firmware needs to be more modular
	Stop asuming pin assignments - define them in the firmware
  Problem - Bluetooth Module is defined based on the Serial1 Port
	*/
  /*
  What other practical functions can we use?

  Possible Functionality Expansion

  arm.report(value);
    Report a value (diagnostic) such as battery life or uptime

  arm.connectivity();
    Return boolean based on test of Bluetooth Module to check for connectivity

  arm.preset(value);
	Allow for a framework to add and use more presets rather than just manually add presets
    Use this instead of arm.wiggle();

	arm.preset() may also be a good place to exercise using a detailed subclass system?
	i.e. using arm.preset.wiggle(180); instead of arm.preset("wiggle", 180);
	Ideally we avoid using numerical values for things and arguments like wiggle to avoid
	turning into one long sucky table of values and conversions and then worrying about software versions

  arm.test(value, logger);
    Add a framework for running repeated tests and for saving/outputing data in a science fair setup
	Replaces and updates batloop();

  An error handler is also possibly needed? Something to catch errors and gracefully cause an exit, instead
  of just not working.
  Include more diagnostic variables for the arm.report("settings");

  */
  stampede.btsetup();
  stampede.setupfingers();
  stampede.fingerattach(2, 3, 4, 5, 6);

}

void loop()
{
  stampede.btloop();
  //stampede.batloop();
}
