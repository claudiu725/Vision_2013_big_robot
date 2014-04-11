#include "VisionSensor.h"

void VisionSensor::initPin(int sensorPin)
{
  this->sensorPin = sensorPin;
  enabled = true;
  pinMode(sensorPin, INPUT);
}

boolean VisionSensor::detect()
{
  return enabled && digitalRead(sensorPin);
}

void VisionSensor::enable()
{
  enabled = true;
}

void VisionSensor::disable()
{
  enabled = false;
}

boolean VisionSensor::isEnabled()
{
  return enabled;
}
