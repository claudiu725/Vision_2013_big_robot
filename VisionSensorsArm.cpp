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
int leftEncoderState;
int rightEncoderState;
int leftStepCount = 0;
int rightStepCount = 0;

void sensors_and_arm::init()
{
  frontLeft.initPin(frontLeftSenzorPin);
  frontFront.initPin(frontFrontSenzorPin);
  frontRight.initPin(frontRightSenzorPin);
  left.initPin(leftSenzorPin);
  right.initPin(rightSenzorPin);
  backLeft.initPin(backLeftSenzorPin);
  backBack.initPin(backBackSenzorPin);
  backRight.initPin(backRightSenzorPin);
  fruit.initPin(fruitSenzorPin);
  fruitColor.initPin(fruitColorSenzorPin);
  verticalArmLimiter.initPin(verticalArmLimiterPin);
  verticalArmLimiter.setAsPullup();
  
  pinMode(leftMotorEncoderPin, INPUT_PULLUP);
  pinMode(rightMotorEncoderPin, INPUT_PULLUP);
  leftEncoderState = digitalRead(leftMotorEncoderPin);
  rightEncoderState = digitalRead(rightMotorEncoderPin);
  
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
  claw.write(140);
}

void sensors_and_arm::clawGrab()
{
  claw.write(35);
}

boolean sensors_and_arm::frontDetected()
{
  return frontLeft.detect() && frontFront.detect() && frontRight.detect();
}

boolean sensors_and_arm::leftDetected()
{
  return left.detect();
}

boolean sensors_and_arm::rightDetected()
{
  return right.detect();
}

boolean sensors_and_arm::backDetected()
{
  return backLeft.detect() && backBack.detect() && backRight.detect();
}
