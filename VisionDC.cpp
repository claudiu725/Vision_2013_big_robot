#include "VisionDC.h"

#define INIT 0
#define DO_TIME_MS 1
#define DO_TIME_MICROS 2
#define GO_TO_FORWARD 3
#define WAIT_FOR_SENSOR_FORWARD 4
#define GO_TO_BACKWARD 5
#define WAIT_FOR_SENSOR_BACKWARD 6
#define STOP 7
#define TOGGLE_2 8
#define TOGGLE_3 9

const int delayBetweenTogglesInMs = 40;

void VisionDC::init()
{
  forwardDirection = LOW;
}

void VisionDC::initPins(int brushlessPin, int relayPin)
{
  this->relayPin = relayPin;
  pinMode(relayPin, OUTPUT);
  directionPinState = forwardDirection;
  digitalWrite(relayPin, directionPinState);
  motorState = INIT;
}

void VisionDC::initPosition(int inductivePosition)
{
//  currentInductivePosition = inductivePosition;
}

void VisionDC::initDirectionForward(boolean forward)
{
  forwardDirection = forward;
}

void VisionDC::setDirectionForward()
{
  directionPinState = forwardDirection;
  digitalWrite(relayPin, directionPinState);
}

void VisionDC::setDirectionBackward()
{
  directionPinState = !forwardDirection;
  digitalWrite(relayPin, directionPinState);
}

void VisionDC::toggleDirection()
{
  directionPinState = !directionPinState;
  digitalWrite(relayPin, directionPinState);
}

void VisionDC::doLoop()
{
  switch (motorState)
  {
    case INIT:
      break;
    case DO_TIME_MS:
      motorState.wait(timeMs, STOP);
      break;
    case DO_TIME_MICROS:
      motorState.waitMicros(timeMicros, STOP);
      break;
    case GO_TO_FORWARD:
      setDirectionForward();
      motorState = WAIT_FOR_SENSOR_FORWARD;
      break;
    case GO_TO_BACKWARD:
      setDirectionBackward();
      motorState = WAIT_FOR_SENSOR_BACKWARD;
      break;
    case STOP:
      toggleDirection();
      motorState.wait(delayBetweenTogglesInMs, TOGGLE_2);
      break;
    case TOGGLE_2:
      toggleDirection();
      motorState.wait(delayBetweenTogglesInMs, TOGGLE_3);
      break;
    case TOGGLE_3:
      toggleDirection();
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

void VisionDC::stopNow()
{
  motorState = STOP;
}

boolean VisionDC::isOff()
{
  return motorState == STATE_STOP;
}
