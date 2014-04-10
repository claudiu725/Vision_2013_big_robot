#include "VisionSensorsArm.h"
#include "pins_big_robot.h"
#include "big_robot_constants.h"

#define STOPPED 0
#define STOPPING 1
#define RUNNING 2
#define STARTING 3

#define UP 10
#define DOWN 11
#define FORWARD 12
#define BACKWARD 13

boolean frontDetected = false;
boolean leftDetected = false;
boolean rightDetected = false;
boolean backDetected = false;
boolean blackLineDetected = false;
    
const int delayActions = 4000;
int clawPos = 20;
    
void sensors_and_arm::init()
{
  attachInterrupt(BackSenzorPin, SenzorBack, CHANGE);
  attachInterrupt(FrontSenzorPin, SenzorFront, CHANGE);
  attachInterrupt(LeftSenzorPin, SenzorLeft, CHANGE);
  attachInterrupt(RightSenzorPin, SenzorRight, CHANGE);    
  
  horizontalArmMotor.init();
  horizontalArmMotor.initPins(horizontalArmEnablePin, horizontalArmDirectionPin, horizontalArmStepPin);
  horizontalArmMotor.initDelays(armSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  horizontalArmMotor.initSizes(horizontalArmWheelDiameter, wheelRevolutionSteps);
  
  verticalArmMotor.init();
  verticalArmMotor.initPins(verticalArmEnablePin, verticalArmDirectionPin, verticalArmStepPin);
  verticalArmMotor.initDelays(armSpeedDelay-4700, highPhaseDelay, maxSpeedDelay); 
  verticalArmMotor.initSizes(verticalArmWheelDiameter, wheelRevolutionSteps);
  
  claw.attach(clawPin);
  
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

void sensors_and_arm::sensors_and_arm::SenzorRight()
{
  rightDetected = !rightDetected;
}

void sensors_and_arm::moveArmHorizontal(float distance, int side)
{     
  horizontalArmMotor.setDirectionForward();
  if(side == BACKWARD)
    horizontalArmMotor.toggleDirection();      
  horizontalArmMotor.setTargetDelay(5000);
  horizontalArmMotor.doDistanceInCm(distance);
}

void sensors_and_arm::moveArmVertical(float distance, int side)
{      
  verticalArmMotor.setDirectionForward();
  if(side == DOWN)
    verticalArmMotor.toggleDirection();      
  verticalArmMotor.setTargetDelay(200);
  verticalArmMotor.doDistanceInCm(distance);
}

void sensors_and_arm::clawRelease()
{
  while( clawPos <= 173) 
  {                     
    claw.write(clawPos);
    clawPos += 5;    
    delay(20);                
  } 
}

void sensors_and_arm::clawGrab()
{
  while( clawPos >= 10) 
  {                     
    claw.write(clawPos);
    clawPos -= 5;    
    delay(20);                
  } 
}
