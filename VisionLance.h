#ifndef VisionLance_h
#define VisionLance_h

#include "Arduino.h"
#include "big_robot_constants.h"
#include "pins_big_robot.h"
#include <elapsedMillis.h>
#include <Servo.h>

class VisionLance {
    Servo lance;
  public:
    void init(int pin);  
    void lower();
    void bringTo(float degree);
    void raise();
};

#endif
