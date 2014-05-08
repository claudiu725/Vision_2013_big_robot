#include "VisionLance.h"

void VisionLance::init()
{
  pinMode(lanceActatorPin, OUTPUT);
  digitalWrite(lanceActatorPin, HIGH);
  pinMode(lanceEnablePin, OUTPUT);
  digitalWrite(lanceEnablePin, LOW);
  pinMode(lanceDirectionAPin, OUTPUT);
  digitalWrite(lanceDirectionAPin, HIGH);
  pinMode(lanceDirectionBPin, OUTPUT);
  digitalWrite(lanceDirectionBPin, LOW);
}

void VisionLance::drop()
{
  digitalWrite(lanceActatorPin, LOW);
  digitalWrite(lanceDirectionAPin, HIGH);
  digitalWrite(lanceDirectionBPin, LOW);
  digitalWrite(lanceEnablePin, HIGH);
  acuatorTimer = 0;
}

void VisionLance::raise()
{
  digitalWrite(lanceActatorPin, LOW);
  digitalWrite(lanceDirectionAPin, LOW);
  digitalWrite(lanceDirectionBPin, HIGH);
  digitalWrite(lanceEnablePin, HIGH);
  acuatorTimer = 0;
}

void VisionLance::doLoop()
{
  if (acuatorTimer > acuatorWaitTime)
  {
    digitalWrite(lanceActatorPin, LOW);
    digitalWrite(lanceEnablePin, LOW);
  }
}
