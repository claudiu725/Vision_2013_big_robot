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
  numberOfDeaccelerationSteps = 500;
  highSpeedDelay = 300;
  lowSpeedDelay = 2100;
  lowHighSpeedDelayDifference = lowSpeedDelay - highSpeedDelay;
  highPhaseDelay = 100;
  accelerationDelayIncrement = (highSpeedDelay - lowSpeedDelay) / numberOfAccelerationSteps;
  deaccelerationDelayIncrement = (lowSpeedDelay - highSpeedDelay) / numberOfDeaccelerationSteps;
  //Serial.print("acc delay:");
  //Serial.println(highSpeedDelay - lowSpeedDelay);
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
      if (((stepPinState == LOW) && (stepTimer > currentStepDelay)) ||
          ((stepPinState == HIGH) && (stepTimer > highPhaseDelay))
          )
      {
        stepTimer = 0;
        accelPercent = float(stepsMadeSoFar) / numberOfAccelerationSteps;
        accelPercent = constrain(accelPercent, 0, 1);
        //Serial.println(accelPercent);
        currentStepDelay = highSpeedDelay + lowHighSpeedDelayDifference * (1 - accelPercent) * (1 - accelPercent);
        //Serial.println(currentStepDelay);
        
        //if (stepsMadeSoFar < numberOfAccelerationSteps)
        //  currentStepDelay += accelerationDelayIncrement;
        //if (stepsRemaining < numberOfDeaccelerationSteps)
        //  currentStepDelay += deaccelerationDelayIncrement;
        stepsMadeSoFar++;
        stepsRemaining--;
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
