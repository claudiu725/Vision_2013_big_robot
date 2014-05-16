#ifndef VisionSensor_h
#define VisionSensor_h

#include "Arduino.h"

class VisionSensor {
  public:
    void initPin(int sensorPin);
    void setAsPullup();
    boolean detect();
    void enable();
    void disable();
    void toggleNegate();
    boolean isEnabled();
  private:
    int sensorPin;
    boolean pullup;
    boolean enabled;
    boolean negate;
};

#endif
