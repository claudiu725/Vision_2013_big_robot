#include "VisionLance.h"

void VisionLance::init(int pin)
{
  lance.attach(pin);
}

void VisionLance::lower()
{
  lance.write(0);
}
void VisionLance::bringTo(float degree)
{
  lance.write(degree);
}

void VisionLance::raise()
{
  lance.write(180);
}

