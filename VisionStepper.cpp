#include "VisionStepper.h"

// motorState states
#define STARTING 0
#define RUNNING 1
#define PAUSING_SLOWING 2
#define PAUSING 3
#define PAUSED 4
#define RESUME 5
#define STOPPING_SLOWING_WHEN_ARRIVING 6
#define STOP_IF_ARRIVED 7
#define STOPPING 8
#define STOPPED 9

// enableState states
#define TURN_ON_DELAYED 0
#define TURN_ON_START 1
#define TURN_ON 2
#define ON 3
#define DELAYED_TURN_OFF 4
#define TURN_OFF 5
#define OFF 6

// speedState states
#define ACCELERATING 0
#define SLOWING 1
#define CONSTANT 2
#define UNDETERMINED 3
#define START 4
#define STOP 5

// stepState states
#define STEP_LOW 0
#define STEP_HIGH 1

const unsigned long waitBeforeTurningOn = 500;
const unsigned long waitBeforeTurningOff = 500;

void VisionStepper::init()
{
  stepsMadeSoFar = 0;
  stepsRemaining = 0;
  motorState = STOPPED;
  speedState = STATE_STOP;
  enableState = OFF;
  stepState = STATE_STOP;
  forwardDirection = HIGH;
  fullStep = false;
}

void VisionStepper::setFullStep()
{
  fullStep = true;
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
  
  special = false;
}

void VisionStepper::initDelays(unsigned long startSpeedDelay, unsigned long highPhaseDelay, unsigned long pauseSpeedDelay, unsigned long delayBeforeTurnOff, float stepSpeedCounterAcceleration, float stepSpeedCounterSlowing)
{
  this->startSpeedDelay = startSpeedDelay;
  this->highPhaseDelay = highPhaseDelay;
  this->pauseSpeedDelay = pauseSpeedDelay;
  this->delayBeforeTurnOff = delayBeforeTurnOff;
  this->stepSpeedCounterAcceleration = stepSpeedCounterAcceleration;
  this->stepSpeedCounterSlowing = stepSpeedCounterSlowing;
  currentDelay = startSpeedDelay;
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
  switch (motorState)
  {
    case STARTING:
      motorState = RUNNING;
      enableState = TURN_ON_DELAYED;
      break;
    case RUNNING:
      break;
    case PAUSING_SLOWING:
      savedWhenPausingDelay = targetDelay;
      setTargetDelay(pauseSpeedDelay);
      motorState = PAUSING;
      break;
    case PAUSING:
      if (speedState == CONSTANT)
      {
        enableState = DELAYED_TURN_OFF;
        motorState = PAUSED;
      }
      break;
    case PAUSED:
      break;
    case RESUME:
      setTargetDelay(savedWhenPausingDelay);
      if (enableState != ON)
        enableState = TURN_ON_DELAYED;
      motorState = RUNNING;
      break;
    case STOPPING_SLOWING_WHEN_ARRIVING:
      setTargetDelay(startSpeedDelay);
      motorState = STOP_IF_ARRIVED;
      break;
    case STOP_IF_ARRIVED:
      if (stepsRemaining == 0)
         motorState = STOPPING;
      break;
    case STOPPING:
      enableState = DELAYED_TURN_OFF;
      motorState = STOPPED;
      break;
    case STOPPED:
      break;
    default:
      motorState.doLoop();
  }
  switch (enableState)
  {
    case TURN_ON_DELAYED:
      enablePinState = HIGH;
      digitalWrite(enablePin, enablePinState);
      enableState.wait(waitBeforeTurningOn, TURN_ON_START);
      break;
    case TURN_ON_START:
      speedState = START;
      enableState = ON;
      break;
    case TURN_ON:
      speedState = START;
      enablePinState = HIGH;
      digitalWrite(enablePin, enablePinState);
      enableState = ON;
      break;
    case ON:
      break;
    case DELAYED_TURN_OFF:
      speedState = STOP;
      enablePinState = LOW;
      enableState.wait(delayBeforeTurnOff, TURN_OFF);
      break;
    case TURN_OFF:
      digitalWrite(enablePin, enablePinState);
      enableState = OFF;
      break;
    case OFF:
      break;
    default:
      enableState.doLoop();
  }
  switch (speedState)
  {
    case ACCELERATING:
      if (currentDelay <= targetDelay)
        speedState = CONSTANT;
      break;
    case SLOWING:
      if (currentDelay >= targetDelay)
        speedState = CONSTANT;
      break;
    case CONSTANT:
      break;
    case UNDETERMINED:
      if (currentDelay < targetDelay)
        speedState = SLOWING;
      else if (currentDelay > targetDelay)
        speedState = ACCELERATING;
      else
        speedState = CONSTANT;
      break;
    case START:
      stepSpeedCounter = 0;
      speedState = UNDETERMINED;
      stepState = STEP_LOW;
      break;
    case STOP:
      stepState = STATE_STOP;
      break;
    default:
      speedState.doLoop();
  }
  switch (stepState) {
    case STEP_LOW:
      stepPinState = LOW;
      digitalWrite(stepPin, stepPinState);
      stepState.waitMicros(currentDelay, STEP_HIGH);
      
      stepsMadeSoFar++;
      stepsRemaining--;
      if (speedState == ACCELERATING)
        stepSpeedCounter += stepSpeedCounterAcceleration;
      else if (speedState == SLOWING)
      {
        stepSpeedCounter -= stepSpeedCounterSlowing;
        if (stepSpeedCounter < 1)
            stepSpeedCounter = 1;
      }
      currentDelay = startSpeedDelay / sqrt(stepSpeedCounter);
      if (stepsRemaining <= stepSpeedCounter / stepSpeedCounterSlowing)
        motorState = STOPPING_SLOWING_WHEN_ARRIVING;
      break;
    case STEP_HIGH:
      stepPinState = HIGH;
      digitalWrite(stepPin, stepPinState);
      stepState.waitMicros(highPhaseDelay, STEP_LOW);
      break;
    default:
      stepState.doLoop();
  }
}

void VisionStepper::pause()
{
  if (motorState == RUNNING)
    motorState = PAUSING_SLOWING;
}

void VisionStepper::unpause()
{
  if (motorState == PAUSING_SLOWING || motorState == PAUSING || motorState == PAUSED)
    motorState = RESUME;
}

void VisionStepper::stopNow()
{
  motorState = STOPPING;
}

void VisionStepper::setTargetDelay(unsigned long targetDelay)
{
  if (this->targetDelay == targetDelay)
    return;
  this->targetDelay = targetDelay;
  speedState = UNDETERMINED;
}

boolean VisionStepper::isOff()
{
  return motorState == STOPPED;
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
  return motorState == RUNNING;
}

void VisionStepper::doSteps(unsigned long stepNumber)
{
  stepsMadeSoFar = 0;
  stepsRemaining = stepNumber;
  if (fullStep)
    stepsRemaining /= 2;
  motorState = STARTING;
}

void VisionStepper::doDistanceInCm(float distance)
{
  doSteps(getStepsFromDistance(distance));
}

void VisionStepper::setRemainingDistance(float distance)
{
  stepsRemaining = getStepsFromDistance(distance);
}

unsigned long VisionStepper::getStepsFromDistance(float distance)
{
  return distance * stepCmRatio;
}

void VisionStepper::doRotationInAngle(float angle)
{
  doSteps(angle * degreeStepRatio);
}

void VisionStepper::setSpecial()
{
  special = true;
}
void VisionStepper::resetSpecial()
{
  special = false;
}


