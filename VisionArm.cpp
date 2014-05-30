#include "VisionArm.h"
#include "pins_big_robot.h"
#include "big_robot_constants.h"
    
const int delayActions = 4000;
int clawPos = 20;
int leftEncoderState;
int rightEncoderState;
int leftStepCount = 0;
int rightStepCount = 0;

void VisionArm::init()
{
//  verticalMotor.init();
//  verticalMotor.initDirectionForward(LOW);
//  verticalMotor.initPins(verticalArmBrushlessPin, verticalArmDirectionRelayPin);
//  verticalMotor.initTopBottom(sensorTop, sensorBottom);
//  verticalMotor.initPosition(sensorTop.inductivePosition);
//  verticalMotor.initPwms(verticalArmStopPwm, verticalArmNormalPwm);
  
//  claw.attach(clawServoPin);
  //clawRelease();
  
  basket.attach(clawBasketPin);
  basketClose();
}

void VisionArm::moveVertical(VisionSensor& sensor)
{
//  verticalMotor.moveTo(sensor);
}

void VisionArm::clawRelease()
{
  claw.write(130);
}

void VisionArm::clawGrab()
{
  claw.write(17);
}

void VisionArm::basketClose()
{
  basket.write(3);
}

void VisionArm::basketOpen()
{
  basket.write(86);
}

boolean VisionArm::isStopped()
{
  return 0;//verticalMotor.isOff();
}

void VisionArm::doLoop()
{
//  verticalMotor.doLoop();
}

void VisionArm::stopNow()
{
//  verticalMotor.stopNow();
}
