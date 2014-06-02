#include "VisionDC.h"

#define INIT 0
#define DO_TIME_MS 1
#define DO_TIME_MICROS 2
#define GO 3
#define GOING 4
#define STOP 7

const int delayBetweenTogglesInMs = 40;

void VisionDC::init()
{
  forwardDirection = LOW;
}

void VisionDC::initPins(int forwardPin, int backwardPin)
{
  this->backwardPin = backwardPin;
  pinMode(backwardPin, OUTPUT);
  
  this->forwardPin = forwardPin;
  pinMode(forwardPin, OUTPUT);

  stopNow();
}

void VisionDC::initTurnCmRatio(float turnCmRatio)
{
  this->turnCmRatio = turnCmRatio;
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
    case GO:
      go();
      encoder.reset();
      motorState = GOING;
      break;
    case GOING:
      if (encoder.count() > targetSteps)
        motorState = STOP;
      break;
    case STOP:
      stop();
      motorState = STATE_STOP;
      break;
    default:
      motorState.doLoop();
      break;
  }
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

void VisionDC::doSteps(long stepNumber)
{
  if (stepNumber < 0)
  {
    setDirectionBackward();
    stepNumber = -stepNumber;
  } else {
    setDirectionForward();
  }
  targetSteps = stepNumber;
  motorState = GO;
}

void VisionDC::doTurns(float turns)
{
  doSteps(turns * encoder.revolutionSteps);
}

void VisionDC::doCm(float distance)
{
  doTurns(distance * turnCmRatio);
}

void VisionDC::stopNow()
{
  stop();
  motorState = STATE_STOP;
}

void VisionDC::go()
{
  digitalWrite(forwardPin, currentDirection);
  digitalWrite(backwardPin, !currentDirection);
}

void VisionDC::stop()
{
  digitalWrite(forwardPin, LOW);
  digitalWrite(backwardPin, LOW);
}

boolean VisionDC::isOff()
{
  return motorState == STATE_STOP;
}
