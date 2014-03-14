#include "VisionStepper.h"

#define STOPPED 0
#define STOPPING 1
#define RUNNING 2
#define STARTING 3

void VisionStepper::init(int enablePin, int directionPin, int stepPin)
{
  this->enablePin = enablePin;
  this->directionPin = directionPin;
  this->stepPin = stepPin;
  directionPinState = HIGH;
  enablePinState = LOW;
  stepPinState = LOW;
  stepsMadeSoFar = 0;
  stepsRemaining = 0;
  numberOfAccelerationSteps = 2000;
  numberOfDeaccelerationSteps = 1000;
  maxSpeedDelay = 300;
  startSpeedDelay = 2000;
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
        if (stepSpeedCounter < 0)
        {
          stepSpeedCounter = 0;
          digitalWrite(13, HIGH);
        }
        currentDelay = startSpeedDelay / sqrt(0.025 * stepSpeedCounter + 1);
        if (!foundTargetSpeed)
          if ((!raiseSpeed && currentDelay > targetDelay) ||
              (raiseSpeed && currentDelay < targetDelay))
              foundTargetSpeed = true;
        //Serial.println(currentDelay);
        stepsMadeSoFar++;
        stepsRemaining--;
        if (stepsRemaining <= stepSpeedCounter)
          setTargetDelay(startSpeedDelay);
        stepPinState = !stepPinState;
        digitalWrite(stepPin, stepPinState);
        if (stepsRemaining == 0)
        {
          globalState = STOPPING;
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
  if (stepsRemaining > numberOfDeaccelerationSteps)
    stepsRemaining = numberOfDeaccelerationSteps;
}

void VisionStepper::setTargetDelay(int targetDelay)
{
  this->targetDelay = targetDelay;
  foundTargetSpeed = false;
}

void VisionStepper::toggleDirection()
{
  directionPinState = !directionPinState;
  digitalWrite(directionPin, directionPinState);
}

bool VisionStepper::isOff()
{
  return globalState == STOPPED;
}

void VisionStepper::doSteps(int stepNumber)
{
  stepsMadeSoFar = 0;
  stepsRemaining = stepNumber;
  globalState = STARTING;
}
