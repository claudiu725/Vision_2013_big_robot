#include "VisionStepper.h"

#define STOPPED 0
#define STOPPING 1
#define WAIT_FOR_STOPPING 2
#define STOPPING_ENABLE_ON 3
#define RUNNING 4
#define PAUSE 5
#define PAUSE_OFF 6
#define STARTING 7

const unsigned long waitBeforeTurningOff = 500;

void VisionStepper::init()
{
  stepsMadeSoFar = 0;
  stepsRemaining = 0;
  globalState = STOPPED;
  special = false;
  forwardDirection = HIGH;
}

void VisionStepper::setSpecial()
{
  special = true;
}
void VisionStepper::resetSpecial()
{
  special = false;
}

void VisionStepper::initDirectionForward(boolean forward)
{
  forwardDirection = forward;
}

void VisionStepper::initPins(int enablePin, int directionPin, int stepPin)
{
  this->enablePin = enablePin;
  this->directionPin = directionPin;
  this->stepPin = stepPin;
  
  pinMode(directionPin, OUTPUT);
  directionPinState = forwardDirection;
  digitalWrite(directionPin, directionPinState);
  
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

void VisionStepper::initSizes(float wheelDiameter, int wheelRevolutionSteps, float distanceBetweenWheels)
{ 
  float wheelCircumference = wheelDiameter * PI;
  stepCmRatio = (wheelRevolutionSteps / wheelCircumference) * 2;
  float bigCircumference = PI * distanceBetweenWheels; //106.76 ;  3.14 * distanceBetweenWheels
  float degreeCmRatio = bigCircumference/360; // 0.2965;  bigCircumference/360
  degreeStepRatio = degreeCmRatio * stepCmRatio; //1.82;  degreeCmRatio * stepCmRatio
}

void VisionStepper::initStepCmRatio(float stepCmRatio)
{
  this->stepCmRatio = stepCmRatio;
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
    case WAIT_FOR_STOPPING:
      if (stopTimer > 200)
        globalState = STOPPING_ENABLE_ON;
      break;
    case STOPPING_ENABLE_ON:
      if (special)
      {
        globalState = STOPPED;
        resetSpecial();
      }
      else
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
        currentDelay = startSpeedDelay * 10 / sqrt(2000 * stepSpeedCounter + 100);
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
        if (pauseWhenFound && foundTargetSpeed)
        {
          globalState = PAUSE;
          pauseTurnOff = 0;
          break;
        }
        //Serial.println(currentStepDelay);
      }
      break;
    case PAUSE:
      if (pauseTurnOff > waitBeforeTurningOff)
      {
        enablePinState = LOW;
        digitalWrite(enablePin, enablePinState);
        globalState = PAUSE_OFF;
      }
      break;
    case PAUSE_OFF:
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
  if (pauseDelay > 10000)
    targetDelay = 40000;
  else
    targetDelay = 10000;
  foundTargetSpeed = false;
  pauseWhenFound = true;
}

void VisionStepper::unpause()
{
  targetDelay = pauseDelay;
  foundTargetSpeed = false;
  pauseWhenFound = false;
  if (globalState == PAUSE_OFF)
  {
    enablePinState = HIGH;
    digitalWrite(enablePin, enablePinState);
  }
  if (globalState == PAUSE || globalState == PAUSE_OFF)
  {
    stepSpeedCounter = 0;
    globalState = RUNNING;
  }
}

void VisionStepper::stopNow()
{
  globalState = STOPPING;
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
  directionPinState = forwardDirection;
  digitalWrite(directionPin, directionPinState);
}

void VisionStepper::setDirectionBackward()
{
  directionPinState = !forwardDirection;
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
  stepsRemaining = stepNumber * 2; //leave as-is!
  globalState = STARTING;
}

void VisionStepper::doDistanceInCm(float distance)
{
  doSteps(distance * stepCmRatio);
}

void VisionStepper::doRotationInAngle(float angle)
{
  doSteps(angle * degreeStepRatio);    // useless for arm motors
}

