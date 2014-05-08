#include "VisionEncoder.h"

void VisionEncoder::initPin(int sensorPin)
{
  sensor.initPin(sensorPin);
  sensor.setAsPullup();
  state = sensor.detect();
  stepCount = 0;
}

void VisionEncoder::reset()
{
  stepCount = 0;
}

long VisionEncoder::count()
{
  return stepCount * wheelRevolutionSteps / encoderRevolutionSteps;
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

