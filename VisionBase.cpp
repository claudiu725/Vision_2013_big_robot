#include "VisionBase.h"

void VisionBase::init()
{
  frontLeft.initPin(frontLeftSenzorPin);
  frontLeft.toggleNegate();
  frontFront.initPin(frontFrontSenzorPin);
  frontRight.initPin(frontRightSenzorPin);
  
  left.initPin(leftSenzorPin);
  right.initPin(rightSenzorPin);
  
  backLeft.initPin(backLeftSenzorPin);
  //backLeft.disable();
  backBack.initPin(backBackSenzorPin);
  //backBack.disable();
  backRight.initPin(backRightSenzorPin);
  //backRight.disable();
  
  //leftEncoder.initPin(leftMotorEncoderPin);
  //leftMotor.init();
  //leftMotor.initDirectionForward(LOW);
//  leftMotor.setFullStep();
  leftMotor.initPins(rightMotorEnablePin, leftMotorDirectionPin, rightMotorStepPin);
  //leftMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, pauseSpeedDelay, delayBeforeTurnOff, stepSpeedCounterAcceleration, stepSpeedCounterSlowing);
  //leftMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  //rightEncoder.initPin(rightMotorEncoderPin);
  rightMotor.init();
  rightMotor.initDirectionForward(HIGH);
//  rightMotor.setFullStep();
  rightMotor.initPins(rightMotorEnablePin, rightMotorDirectionPin, rightMotorStepPin);
  rightMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, pauseSpeedDelay, delayBeforeTurnOff, stepSpeedCounterAcceleration, stepSpeedCounterSlowing);
  rightMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  directionMovement = NONE;
  obstructionDetected = false;;
  oppositeSide /*= digitalRead(colorYellowPin)*/ = false;
}

void VisionBase::moveForward(float distance, int step_delay)
{       
  directionMovement = FRONT;
  //leftMotor.setTargetDelay(step_delay);         
  rightMotor.setTargetDelay(step_delay);
  leftMotor.setDirectionForward();
  rightMotor.setDirectionForward();
  //leftMotor.doDistanceInCm(distance);
  rightMotor.doDistanceInCm(distance);
}

void VisionBase::moveBackward(float distance, int step_delay)
{    
  directionMovement = BACK;
  //leftMotor.setTargetDelay(step_delay);         
  rightMotor.setTargetDelay(step_delay);
  leftMotor.setDirectionBackward();
  rightMotor.setDirectionBackward();
  //leftMotor.doDistanceInCm(distance);
  rightMotor.doDistanceInCm(distance);
}

void VisionBase::turnLeft(int angle)
{
  directionMovement = LEFT;
  if(!oppositeSide)
  {
    leftMotor.setDirectionBackward();
    rightMotor.setDirectionForward();
  }
  else
  {    
    leftMotor.setDirectionForward();
    rightMotor.setDirectionBackward();
  }
  //leftMotor.setTargetDelay(turnSpeedDelay);         
  rightMotor.setTargetDelay(turnSpeedDelay);
  //leftMotor.doRotationInAngle(angle);
  rightMotor.doRotationInAngle(angle); 
}

void VisionBase::turnRight(int angle)
{  
  directionMovement = RIGHT;
  if(!oppositeSide)
  {
    leftMotor.setDirectionForward();
    rightMotor.setDirectionBackward();
  }
  else
  { 
    leftMotor.setDirectionBackward();
    rightMotor.setDirectionForward();
  }
  //leftMotor.setTargetDelay(turnSpeedDelay);         
  rightMotor.setTargetDelay(turnSpeedDelay);
  //leftMotor.doRotationInAngle(angle);
  rightMotor.doRotationInAngle(angle);
}


void VisionBase::pause()
{
  //leftMotor.pause();
  rightMotor.pause();
}

void VisionBase::unpause()
{
  //leftMotor.unpause();
  rightMotor.unpause();
}

void VisionBase::stopNow()
{
  //leftMotor.stopNow();
  rightMotor.stopNow();
}

boolean VisionBase::frontDetected()
{
  return frontLeft.detect() || frontFront.detect() || frontRight.detect();
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
  return backLeft.detect() || backBack.detect() || backRight.detect();
}

boolean VisionBase::isStopped()
{
  return rightMotor.isOff();
}

void VisionBase::checkObstructions()
{
  obstructionDetected = false;
  if (frontDetected() && directionMovement == FRONT)
    obstructionDetected = true;
  //if (leftDetected() && directionMovement == LEFT)
  //  obstructionDetected = true;
  //if (rightDetected() && directionMovement == RIGHT)
  //  obstructionDetected = true;
  if (backDetected() && directionMovement == BACK)
    obstructionDetected = true;
}

void VisionBase::doLoop()
{
  //leftEncoder.doLoop();
  //leftMotor.doLoop();
  //rightEncoder.doLoop();
  rightMotor.doLoop();
}

void VisionBase::disable()
{
  rightMotor.disable();
}
