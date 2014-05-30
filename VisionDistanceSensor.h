#ifndef VisionSensor_h
#define VisionSensor_h

#include "Arduino.h"

class VisionSensor {
  public:
    void initPin(int sensorPin);
    void initInductivePosition(int inductivePosition);
    void isBelow(int inductivePosition);
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
  public:
    int inductivePosition;
};

#endif
