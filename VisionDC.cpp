#include "VisionDC.h"

#define INIT 0
#define DO_TIME_MS 1
#define DO_TIME_MICROS 2
#define GO_FORWARD 3
#define GOING_FORWARD 4
#define GO_BACKWARD 5
#define GOING_BACKWARD 6
#define STOP 7
#define TOGGLE_2 8
#define TOGGLE_3 9

const int delayBetweenTogglesInMs = 40;

void VisionDC::init()
{
  forwardDirection = LOW;
}

void VisionDC::initPins(int forwardPin, int backwardPin)
{
  this->backwardPin = backwardPin;
  pinMode(backwardPin, OUTPUT);
  digitalWrite(backwardPin, LOW);
  
  this->forwardPin = forwardPin;
  pinMode(forwardPin, OUTPUT);
  digitalWrite(forwardPin, LOW);
  
  motorState = INIT;
}

void VisionDC::initStepCmRatio(float stepCmRatio)
{
  this->stepCmRatio = stepCmRatio;
}

void VisionDC::initDirectionForward(boolean forward)
{
  forwardDirection = forward;
}

void VisionDC::setDirectionForward()
{
  currentDirection = forwardDirection;
  if (!isOff())
    go();
}

void VisionDC::setDirectionBackward()
{
  currentDirection = !forwardDirection;
  if (!isOff())
    go();
}

void VisionDC::toggleDirection()
{
  currentDirection = !currentDirection;
  if (!isOff())
    go();
}

void VisionDC::doLoop()
{
  switch (motorState)
  {
    case DO_TIME_MS:
      go();
      motorState.wait(timeMs, STOP);
      break;
    case DO_TIME_MICROS:
      motorState.waitMicros(timeMicros, STOP);
      break;
    case GO_FORWARD:
      setDirectionForward();
      go();
      motorState = GOING_FORWARD;
      break;
    case GOING_FORWARD:
      break;
    case GO_BACKWARD:
      setDirectionForward();
      go();
      motorState = GOING_BACKWARD;
      break;
    case GOING_BACKWARD:
      break;
    case STOP:
      stop();
      motorState = STATE_STOP;
    default:
      motorState.doLoop();
      break;
  }
}

void VisionDC::doSteps(unsigned long stepNumber)
{
//  stepsRemaining = stepNumber;
  motorState = GO_FORWARD;
}

void VisionDC::doDistanceInCm(float distance)
{
  doSteps(getStepsFromDistance(distance));
}

unsigned long VisionDC::getStepsFromDistance(float distance)
{
  return distance * stepCmRatio;
}

void VisionDC::doTimeMs(unsigned long time)
{
  timeMs = time;
  motorState = DO_TIME_MS;
}

void VisionDC::doTimeMicros(unsigned long time)
{
  timeMicros = time;
  motorState = DO_TIME_MICROS;
}

void VisionDC::stopNow()
{
  motorState = STOP;
}

void VisionDC::go()
{
  digitalWrite(forwardPin, currentDirection);
  digitalWrite(forwardPin, !currentDirection);
}

void VisionDC::stop()
{
  digitalWrite(forwardPin, LOW);
  digitalWrite(forwardPin, LOW);
}

boolean VisionDC::isOff()
{
  return motorState == STATE_STOP;
}
