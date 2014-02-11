#include "VisionStepper.h"

#define STOPPED 0
#define STOPPING 1
#define RUNNING 2
#define STARTING 3

VisionStepper::VisionStepper(int enablePin, int directionPin, int stepPin)
{
  this->enablePin = enablePin;
  this->directionPin = directionPin;
  this->stepPin = stepPin;
  directionPinState = HIGH;
  enablePinState = LOW;
  stepPinState = LOW;
  stepsMadeSoFar = 0;
  stepsRemaining = 0;
  numberOfAccelerationSteps = 1000;
  numberOfDeaccelerationSteps = 1;
  highSpeedDelay = 90;
  lowSpeedDelay = 1000;
  lowPhaseDelay = 90;
  accelerationDelayIncrement = (highSpeedDelay - lowSpeedDelay) / numberOfAccelerationSteps;
  deaccelerationDelayIncrement = (lowSpeedDelay - highSpeedDelay) / numberOfDeaccelerationSteps;
  
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
      if (((stepPinState == HIGH) && (stepTimer > currentStepDelay)) ||
          ((stepPinState == LOW) && (stepTimer > lowPhaseDelay))
          )
      {
        stepTimer = 0;
        stepsMadeSoFar++;
        stepsRemaining--;
        stepPinState = !stepPinState;
        digitalWrite(stepPin, stepPinState);
        if (stepsRemaining == 0)
          globalState = STOPPING;
        break;
        
        if (stepsMadeSoFar < numberOfAccelerationSteps)
          currentStepDelay += accelerationDelayIncrement;
        if (stepsRemaining < numberOfDeaccelerationSteps)
          currentStepDelay += deaccelerationDelayIncrement;
      }
      break;
    case STARTING:
      enablePinState = HIGH;
      digitalWrite(enablePin, enablePinState);
      globalState = RUNNING;
      stepTimer = 0;
      currentStepDelay = lowSpeedDelay;
      break;
  }
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
