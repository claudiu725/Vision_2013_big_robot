#ifndef VisionSensor_h
#define VisionSensor_h

#include "Arduino.h"

class VisionSensor {
  public:
    void initPin(int sensorPin);
    boolean detect();
    void enable();
    void disable();
    boolean isEnabled();
  private:
    int sensorPin;
    boolean enabled;
};

#endif
