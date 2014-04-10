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
    
const int delayActions = 4000;
int clawPos = 20;
    
void sensors_and_arm::init()
{
  pinMode(BackSenzorPin, INPUT);
  pinMode(FrontSenzorPin, INPUT);
  pinMode(LeftSenzorPin, INPUT);
  pinMode(RightSenzorPin, INPUT);
  pinMode(FruitSenzorPin, INPUT);
  
  horizontalArmMotor.init();
  horizontalArmMotor.initPins(horizontalArmEnablePin, horizontalArmDirectionPin, horizontalArmStepPin);
  horizontalArmMotor.initDelays(horizontalArmSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  horizontalArmMotor.initStepCmRatio(horizontalArmCmStepRatio);
  
  verticalArmMotor.init();
  verticalArmMotor.initPins(verticalArmEnablePin, verticalArmDirectionPin, verticalArmStepPin);
  verticalArmMotor.initDelays(verticalArmSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  verticalArmMotor.initStepCmRatio(verticalArmCmStepRatio);
  
  claw.attach(clawPin);
  
}

boolean sensors_and_arm::detectFront()
{
  return digitalRead(FrontSenzorPin);
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

boolean sensors_and_arm::detectFruit()
{
  return digitalRead(FruitSenzorPin);
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
