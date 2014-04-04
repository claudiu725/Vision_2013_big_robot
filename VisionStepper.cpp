#include "VisionStepper.h"

#define STOPPED 0
#define STOPPING 1
#define STOPPING_ENABLE_ON 2
#define RUNNING 3
#define PAUSE 4
#define STARTING 5

void VisionStepper::init()
{
  stepsMadeSoFar = 0;
  stepsRemaining = 0;
  globalState = STOPPED;
}

void VisionStepper::initPins(int enablePin, int directionPin, int stepPin)
{
  this->enablePin = enablePin;
  this->directionPin = directionPin;
  this->stepPin = stepPin;
  
  pinMode(directionPin, OUTPUT);
  directionPinState = HIGH;
  digitalWrite(directionPin, HIGH);
  
  pinMode(enablePin, OUTPUT);
  enablePinState = LOW;
  digitalWrite(enablePin, enablePinState);
  
  pinMode(stepPin, OUTPUT);
  stepPinState = LOW;
  digitalWrite(stepPin, stepPinState);
}

void VisionStepper::initDelays(int startSpeedDelay, int highPhaseDelay, int maxSpeedDelay)
{
  this->maxSpeedDelay = maxSpeedDelay;
  this->startSpeedDelay = startSpeedDelay;
  this->highPhaseDelay = highPhaseDelay;
}

void VisionStepper::initSizes(int wheelDiameter, int wheelRevolutionSteps)
{
  int wheelCircumference = wheelDiameter * PI;
  stepCmRatio = wheelRevolutionSteps / wheelCircumference;
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
        currentDelay = startSpeedDelay * 10 / sqrt(0.010 * stepSpeedCounter + 100);
        if (!foundTargetSpeed)
          if ((!raiseSpeed && currentDelay > targetDelay) ||
              (raiseSpeed && currentDelay < targetDelay))
              foundTargetSpeed = true;
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
    case PAUSE:
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

void VisionStepper::pause()
{
  if (globalState == PAUSE)
    return;
  old_state = globalState;
  globalState = PAUSE;
}

void VisionStepper::unpause()
{
  globalState = old_state;
}

void VisionStepper::emergencyStop()
{
  /*
  if (stepsRemaining > numberOfDeaccelerationSteps)
    stepsRemaining = numberOfDeaccelerationSteps;
    */
}

void VisionStepper::setMaxSpeed()
{
  setTargetDelay(maxSpeedDelay);
}

void VisionStepper::setTargetDelay(int targetDelay)
{
  if (this->targetDelay == targetDelay)
    return;
  this->targetDelay = targetDelay;
  foundTargetSpeed = false;
}

boolean VisionStepper::isOff()
{
  return globalState == STOPPED;
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
