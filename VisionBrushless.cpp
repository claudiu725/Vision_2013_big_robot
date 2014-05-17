#include "VisionBrushless.h"

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

void VisionBrushless::init()
{
  forwardDirection = LOW;
  currentInductivePosition = 0;
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

void VisionBrushless::initTopBottom(VisionSensor& front, VisionSensor& back)
{
  this->front = &front;
  this->back = &back;
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
    case GO_TO_FORWARD:
      brushless.write(normalPwm);
      setDirectionForward();
      motorState = WAIT_FOR_SENSOR_FORWARD;
      break;
    case WAIT_FOR_SENSOR_FORWARD:
      stopIfSensorDetect(*sensorToGoTo);
      stopIfSensorDetect(*front); 
      break;
    case GO_TO_BACKWARD:
      brushless.write(normalPwm);
      motorState = WAIT_FOR_SENSOR_BACKWARD;
      break;
    case WAIT_FOR_SENSOR_BACKWARD:
      stopIfSensorDetect(*sensorToGoTo);
      stopIfSensorDetect(*back);
      break;
    case STOP:
      toggleDirection();
      brushless.write(stopPwm);
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

void VisionBrushless::stopIfSensorDetect(VisionSensor& sensor)
{
  if (sensor.detect())
  {
    currentInductivePosition = sensor.inductivePosition;
    motorState = STOP;
  }
}

void VisionBrushless::moveTo(VisionSensor& sensor)
{
  if (!isOff())
    return;
  if (currentInductivePosition == front->inductivePosition)
    motorState = GO_TO_BACKWARD;
  if (currentInductivePosition == back->inductivePosition)
    motorState = GO_TO_FORWARD;
  if (sensor.inductivePosition == currentInductivePosition)
    motorState = GO_TO_FORWARD;
  if (sensor.inductivePosition > currentInductivePosition)
    motorState = GO_TO_FORWARD;
  else
    motorState = GO_TO_BACKWARD;
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

void VisionBrushless::stopNow()
{
  motorState = STOP;
}

boolean VisionBrushless::isOff()
{
  return motorState == STATE_STOP;
}
