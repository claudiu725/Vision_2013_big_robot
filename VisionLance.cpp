#include "VisionLance.h"

void VisionLance::init()
{
  lance.attach(lanceServoPin);
}

void VisionLance::lower()
{
  lance.write(0);
}

void VisionLance::raise()
{
  lance.write(180);
}

