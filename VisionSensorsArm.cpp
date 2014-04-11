#include "VisionSensorsArm.h"
#include "VisionSensor.h"
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
  back.initPin(BackSenzorPin);
  front.initPin(FrontSenzorPin);
  left.initPin(LeftSenzorPin);
  right.initPin(RightSenzorPin);
  fruit.initPin(FruitSenzorPin);
  verticalArmLimiter.initPin(verticalArmLimiterPin);
  verticalArmLimiter.setAsPullup();
  
  pinMode(LeftEncoderPin, INPUT_PULLUP);
  pinMode(RightEncoderPin, INPUT_PULLUP);
  leftEncoderState = digitalRead(LeftEncoderPin);
  rightEncoderState = digitalRead(RightEncoderPin);
  
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
