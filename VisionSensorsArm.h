#ifndef VisionSensorsArm_h
#define VisionSensorsArm_h

#include "Arduino.h"
#include <elapsedMillis.h>

class sensors_and_arm {
  public:    
    void init();
    static void SenzorFront();
    static void SenzorLeft();
    static void SenzorRight();
    static void SenzorBack();
    boolean detectFront();
    boolean detectBack();
    boolean detectLeft();
    boolean detectRight();
};

#endif
