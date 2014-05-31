#include "VisionEncoder.h"

void VisionEncoder::initPin(int sensorPin)
{
  sensor.initPin(sensorPin);
  state = sensor.detect();
  stepCount = 0;
  revolutionSteps = 1;
}

void VisionEncoder::initRevolutionSteps(long revolutionSteps)
{
  this->revolutionSteps = revolutionSteps;
}

void VisionEncoder::reset()
{
  stepCount = 0;
}

long VisionEncoder::count()
{
  return stepCount;
}

void VisionEncoder::doLoop()
{
  boolean currentState = sensor.detect();
  if (state != currentState)
  {
    stepCount++;
    state = currentState;
  }
}

