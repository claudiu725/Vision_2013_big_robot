#include "VisionSensorsArm.h"
#include "pins_big_robot.h"

#define STOPPED 0
#define STOPPING 1
#define RUNNING 2
#define STARTING 3

boolean frontDetected = false;
boolean leftDetected = false;
boolean rightDetected = false;
boolean backDetected = false;
boolean blackLineDetected = false;
    
const int delayActions = 4000;
    
void sensors_and_arm::init()
{
  pinMode(1, INPUT);
  attachInterrupt(BackSenzorPin, SenzorBack, CHANGE);
  
  pinMode(0, INPUT);
  attachInterrupt(FrontSenzorPin, SenzorFront, CHANGE);
  
  pinMode(4, INPUT);
  attachInterrupt(LeftSenzorPin, SenzorLeft, CHANGE);
  
  pinMode(5, INPUT);
  attachInterrupt(RightSenzorPin, SenzorRight, CHANGE);
}

void sensors_and_arm::SenzorFront()
{
  frontDetected = !frontDetected;
}

boolean sensors_and_arm::detectFront()
{
  return digitalRead(FrontSenzorPin);
}

void sensors_and_arm::SenzorBack()
{
  backDetected = !backDetected;
}

boolean sensors_and_arm::detectBack()
{
  return digitalRead(BackSenzorPin);
}

boolean sensors_and_arm::detectLeft()
{
  return digitalRead(LeftSenzorPin);
}

boolean sensors_and_arm::detectRight()
{
  return digitalRead(RightSenzorPin);
}

void sensors_and_arm::SenzorLeft()
{
  leftDetected = !leftDetected;
}

void sensors_and_arm::SenzorRight()
{
  rightDetected = !rightDetected;
}
