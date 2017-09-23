#include <Servo.h>
#include "stampede.h"

arm stampede = arm();

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
