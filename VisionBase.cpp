#include "VisionBase.h"

void VisionBase::init()
{
  frontLeft.initPin(frontLeftSenzorPin);
  frontRight.initPin(frontRightSenzorPin);
  frontAll.initPin(frontAllSenzorPin);
  
  backLeft.initPin(backLeftSenzorPin);
  backRight.initPin(backRightSenzorPin);
  
  //leftEncoder.initPin(leftMotorEncoderPin);
  leftMotor.init();
  leftMotor.initDirectionForward(LOW);
//  leftMotor.setFullStep();
  leftMotor.initPins(leftMotorEnablePin, leftMotorDirectionPin, leftMotorStepPin);
  leftMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, pauseSpeedDelay, delayBeforeTurnOff, stepSpeedCounterAcceleration, stepSpeedCounterSlowing);
  leftMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  //rightEncoder.initPin(rightMotorEncoderPin);
  rightMotor.init();
  rightMotor.initDirectionForward(HIGH);
//  rightMotor.setFullStep();
  rightMotor.initPins(rightMotorEnablePin, rightMotorDirectionPin, rightMotorStepPin);
  rightMotor.initDelays(defaultStartSpeedDelay, highPhaseDelay, pauseSpeedDelay, delayBeforeTurnOff, stepSpeedCounterAcceleration, stepSpeedCounterSlowing);
  rightMotor.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  directionMovement = NONE;
  obstructionDetected = false;
  oppositeSide /*= digitalRead(colorYellowPin)*/ = false;
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
  turnLeft(angle, turnSpeedDelay);
}

void VisionBase::turnLeft(int angle, int turnDelay)
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
  leftMotor.setTargetDelay(turnDelay);         
  rightMotor.setTargetDelay(turnDelay);
  leftMotor.doRotationInAngle(angle);
  rightMotor.doRotationInAngle(angle); 
}

void VisionBase::turnRight(int angle)
{
  turnRight(angle, turnSpeedDelay);
}

void VisionBase::turnRight(int angle, int turnDelay)
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
  leftMotor.setTargetDelay(turnDelay);         
  rightMotor.setTargetDelay(turnDelay);
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

boolean VisionBase::isPausing()
{
  return leftMotor.isPausing() || rightMotor.isPausing();
}

void VisionBase::stopNow()
{
  leftMotor.stopNow();
  rightMotor.stopNow();
}

boolean VisionBase::frontDetected()
{
  return frontLeft.detect() || frontRight.detect() || frontAll.detect();
}

boolean VisionBase::backDetected()
{
  return backLeft.detect() || backRight.detect();
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
  leftMotor.doLoop();
  //rightEncoder.doLoop();
  rightMotor.doLoop();
}

void VisionBase::undo(int step_delay)
{
  if (directionMovement == FRONT)
    directionMovement = BACK;
  else if (directionMovement == BACK)
    directionMovement = FRONT;
  else if (directionMovement == LEFT)
    directionMovement = RIGHT;
  else if (directionMovement == RIGHT)
    directionMovement = LEFT;
  leftMotor.setTargetDelay(step_delay);
  leftMotor.undo();
  rightMotor.setTargetDelay(step_delay);
  rightMotor.undo();
}

void VisionBase::disable()
{
  leftMotor.disable();
  rightMotor.disable();
}
