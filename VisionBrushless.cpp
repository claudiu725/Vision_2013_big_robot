#include "VisionBrushless.h"

#define INIT 0
#define DO_TIME_MS 1
#define DO_TIME_MICROS 2
#define STOP 3

void VisionBrushless::init()
{
  forwardDirection = LOW;
}

void VisionBrushless::initPins(int brushlessPin, int relayPin)
{
  this->brushlessPin = brushlessPin;
  brushless.attach(brushlessPin);
  this->relayPin = relayPin;
  pinMode(relayPin, OUTPUT);
  directionPinState = forwardDirection;
  digitalWrite(relayPin, directionPinState);
  motorState = INIT;
}

void VisionBrushless::initPwms(int stopPwm, int normalPwm)
{
  this->stopPwm = stopPwm;
  this->normalPwm = normalPwm;
}

void VisionBrushless::setNormalPwm(int normalPwm)
{
  this->normalPwm = normalPwm;
}

void VisionBrushless::initDirectionForward(boolean forward)
{
  forwardDirection = forward;
}

void VisionBrushless::setDirectionForward()
{
  directionPinState = forwardDirection;
  digitalWrite(relayPin, directionPinState);
}

void VisionBrushless::setDirectionBackward()
{
  directionPinState = !forwardDirection;
  digitalWrite(relayPin, directionPinState);
}

void VisionBrushless::toggleDirection()
{
  directionPinState = !directionPinState;
  digitalWrite(relayPin, directionPinState);
}

void VisionBrushless::doLoop()
{
  switch (motorState)
  {
    case INIT:
      motorState = STOP;
      break;
    case DO_TIME_MS:
      brushless.write(normalPwm);
      motorState.wait(timeMs, STOP);
      break;
    case DO_TIME_MICROS:
      brushless.write(normalPwm);
      motorState.waitMicros(timeMicros, STOP);
      break;
    case STOP:
      brushless.write(stopPwm);
      motorState = STATE_STOP;
      break;
    default:
      motorState.doLoop();
      break;
  }
}

void VisionBrushless::doTimeMs(unsigned long time)
{
  timeMs = time;
  motorState = DO_TIME_MS;
}

void VisionBrushless::doTimeMicros(unsigned long time)
{
  timeMicros = time;
  motorState = DO_TIME_MICROS;
}

void VisionBrushless::doDistanceInCm(float distance)
{
}

void VisionBrushless::initTimeCmRatio(float verticalArmCmTimeRatio)
{
  this->verticalArmCmTimeRatio = verticalArmCmTimeRatio;
}

void VisionBrushless::stopNow()
{
  motorState = STOP;
}

boolean VisionBrushless::isOff()
{
  return motorState == STATE_STOP;
}
