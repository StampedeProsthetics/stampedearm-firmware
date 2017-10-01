#include <Servo.h>
#include "stepper.h"
#include "stampede.h"
int stepper1[] = {32, 31, 30, 29};
int stepper2[] = {28, 27, 26, 25};
int stepper3[] = {12, 11, 10, 9};
int stepper4[] = {13, 14, 15, 16};
int stepper5[] = {17, 18, 19, 20};
arm stampede = arm(stepper1, stepper2, stepper3, stepper4, stepper5);

void setup()
{
  stampede.btsetup();
  stampede.setupfingers();
  stampede.fingerattach(2, 3, 4, 5, 6);

}

void loop()
{
  stampede.btloop();
}
