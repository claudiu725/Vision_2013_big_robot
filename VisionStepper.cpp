#include "VisionStepper.h"

#define STOPPED 0
#define STOPPING 1
#define STOPPING_ENABLE_ON 2
#define RUNNING 3
#define STARTING 4

void VisionStepper::init(int enablePin, int directionPin, int stepPin)
{
  this->enablePin = enablePin;
  this->directionPin = directionPin;
  this->stepPin = stepPin;
  
  wheelDiameter = 12.3; //cm
  wheelRevolutionSteps = 200;
  stepCmRatio = wheelRevolutionSteps/wheelDiameter;
  degreeStepRatio = 5.8;
  
  directionPinState = HIGH;
  enablePinState = LOW;
  stepPinState = LOW;
  stepsMadeSoFar = 0;
  stepsRemaining = 0;
  maxSpeedDelay = 500;
  startSpeedDelay = 4000;
  highPhaseDelay = 100;
  doSetup();
}

void VisionStepper::doSetup()
{
  pinMode(directionPin, OUTPUT);
  digitalWrite(directionPin, directionPinState);
  
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, enablePinState);
  
  pinMode(stepPin, OUTPUT);
  digitalWrite(stepPin, stepPinState);
}

void VisionStepper::doLoop()
{
  switch (globalState) {
    case STOPPED:
      break;
    case STOPPING:
      enablePinState = LOW;
      digitalWrite(enablePin, enablePinState);
      globalState = STOPPED;
      break;
    case STOPPING_ENABLE_ON:
      if (stopTimer > 100)
        globalState = STOPPING;
      break;
    case RUNNING:
      if (((stepPinState == LOW) && (stepTimer > currentDelay)) ||
          ((stepPinState == HIGH) && (stepTimer > highPhaseDelay))
          )
      {
        stepTimer = 0;
        if (!foundTargetSpeed)
        {
          if (currentDelay > targetDelay)
          {
            stepSpeedCounter++;
            raiseSpeed = true;
          }
          else if (currentDelay < targetDelay)
          {
            stepSpeedCounter--;
            raiseSpeed = false;
          }
        }
        currentDelay = startSpeedDelay * 10 / sqrt(0.025 * stepSpeedCounter + 100);
        if (!foundTargetSpeed)
          if ((!raiseSpeed && currentDelay > targetDelay) ||
              (raiseSpeed && currentDelay < targetDelay))
              foundTargetSpeed = true;
        if (foundTargetSpeed)
          digitalWrite(13, HIGH);
        else
          digitalWrite(13, LOW);
        //Serial.println(currentDelay);
        stepsMadeSoFar++;
        stepsRemaining--;
        if (stepsRemaining <= stepSpeedCounter)
          setTargetDelay(startSpeedDelay);
        stepPinState = !stepPinState;
        digitalWrite(stepPin, stepPinState);
        if (stepsRemaining == 0)
        {
          globalState = STOPPING_ENABLE_ON;
          stopTimer = 0;
          break;
        }
        //Serial.println(currentStepDelay);
      }
      break;
    case STARTING:
      enablePinState = HIGH;
      digitalWrite(enablePin, enablePinState);
      globalState = RUNNING;
      stepTimer = 0;
      stepSpeedCounter = 0;
      currentDelay = startSpeedDelay;
      foundTargetSpeed = false;
      break;
  }
}

void VisionStepper::emergencyStop()
{
  //nothing
}

void VisionStepper::setTargetDelay(int targetDelay)
{
  if (this->targetDelay == targetDelay)
    return;
  this->targetDelay = targetDelay;
  foundTargetSpeed = false;
}

void VisionStepper::setMaxSpeed()
{
  setTargetDelay(maxSpeedDelay);
}

void VisionStepper::setDirectionForward()
{
  directionPinState = HIGH;
  digitalWrite(directionPin, directionPinState);
}

void VisionStepper::toggleDirection()
{
  directionPinState = !directionPinState;
  digitalWrite(directionPin, directionPinState);
}

boolean VisionStepper::isOff()
{
  return globalState == STOPPED;
}

boolean VisionStepper::isAtTargetSpeed()
{
  return foundTargetSpeed;
}

void VisionStepper::doSteps(int stepNumber)
{
  stepsMadeSoFar = 0;
  stepsRemaining = stepNumber * 2; //a step is made out of a LOW to HIGH transition
  globalState = STARTING;
}

void VisionStepper::doDistanceInCm(float distance)
{
  doSteps(distance * stepCmRatio);
}
