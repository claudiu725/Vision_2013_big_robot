#ifndef VisionLance_h
#define VisionLance_h

#include "Arduino.h"
#include "big_robot_constants.h"
#include "pins_big_robot.h"
#include <elapsedMillis.h>

class VisionLance {
  public:
    void init();
    void drop();
    void raise();
    void doLoop();
  private:
    elapsedMillis acuatorTimer;
};

#endif
