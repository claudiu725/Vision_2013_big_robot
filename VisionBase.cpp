#include "VisionBase.h"

void VisionBase::init()
{
  frontLeft.initPin(frontLeftSenzorPin);
  frontFront.initPin(frontFrontSenzorPin);
  frontRight.initPin(frontRightSenzorPin);
  
  left.initPin(leftSenzorPin);
  right.initPin(rightSenzorPin);
  
  backLeft.initPin(backLeftSenzorPin);
  backBack.initPin(backBackSenzorPin);
  backRight.initPin(backRightSenzorPin);
  
  leftEncoder.initPin(leftMotorEncoderPin);
  leftMotor.init();
  leftMotor.initDirectionForward(HIGH);
  leftMotor.initPins(leftMotorEnablePinLeft, leftMotorDirectionPinLeft, leftMotorStepPinLeft);
  leftMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, maxSpeedDelay, pauseSpeedDelay); 
  leftMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  rightEncoder.initPin(rightMotorEncoderPin);
  rightMotor.init();
  rightMotor.initDirectionForward(LOW);
  rightMotor.initPins(rightMotorEnablePin, rightMotorDirectionPin, rightMotorStepPin);
  rightMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, maxSpeedDelay, pauseSpeedDelay); 
  rightMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  directionMovement = NONE;
  obstructionDetected = false;
}

void VisionBase::moveForward(float distance, int step_delay)
{       
  directionMovement = FRONT;
  leftMotor.setTargetDelay(step_delay);         
  rightMotor.setTargetDelay(step_delay);
  leftMotor.setDirectionForward();
  rightMotor.setDirectionForward();
  leftMotor.doDistanceInCm(distance);
  rightMotor.doDistanceInCm(distance);
}

void VisionBase::moveBackward(float distance, int step_delay)
{    
  directionMovement = BACK;
  leftMotor.setTargetDelay(step_delay);         
  rightMotor.setTargetDelay(step_delay);
  leftMotor.setDirectionBackward();
  rightMotor.setDirectionBackward();
  leftMotor.doDistanceInCm(distance);
  rightMotor.doDistanceInCm(distance);
}

void VisionBase::turnLeft(int angle)
{
  directionMovement = LEFT;
  leftMotor.setDirectionBackward();
  rightMotor.setDirectionForward();
  leftMotor.doRotationInAngle(angle);
  rightMotor.doRotationInAngle(angle); 
}

void VisionBase::turnRight(int angle)
{  
  directionMovement = RIGHT;
  leftMotor.setDirectionForward();
  rightMotor.setDirectionBackward();
  leftMotor.doRotationInAngle(angle);
  rightMotor.doRotationInAngle(angle);
}

void VisionBase::pause()
{
  leftMotor.pause();
  rightMotor.pause();
}

void VisionBase::unpause()
{
  leftMotor.unpause();
  rightMotor.unpause();
}

void VisionBase::stopNow()
{
  leftMotor.stopNow();
  rightMotor.stopNow();
}

boolean VisionBase::frontDetected()
{
  return frontLeft.detect() && frontFront.detect() && frontRight.detect();
}

boolean VisionBase::leftDetected()
{
  return left.detect();
}

boolean VisionBase::rightDetected()
{
  return right.detect();
}

boolean VisionBase::backDetected()
{
  return backLeft.detect() && backBack.detect() && backRight.detect();
}

boolean VisionBase::isStopped()
{
  return leftMotor.isOff() && rightMotor.isOff();
}

void VisionBase::checkObstructions()
{
  obstructionDetected = false;
  if (frontDetected() && directionMovement == FRONT)
    obstructionDetected = true;
  if (leftDetected() && directionMovement == LEFT)
    obstructionDetected = true;
  if (rightDetected() && directionMovement == RIGHT)
    obstructionDetected = true;
  if (backDetected() && directionMovement == BACK)
    obstructionDetected = true;
}

void VisionBase::doLoop()
{
  leftEncoder.doLoop();
  leftMotor.doLoop();
  rightEncoder.doLoop();
  rightMotor.doLoop();
}
