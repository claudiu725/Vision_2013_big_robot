#ifndef VisionSensorsArm_h
#define VisionSensorsArm_h

#include "Arduino.h"
#include "VisionStepper.h"
#include <elapsedMillis.h>
#include <Servo.h>
 

class sensors_and_arm {
  public:    
    Servo claw;  
    VisionStepper horizontalArmMotor;
    VisionStepper verticalArmMotor;
    void init();  
    void clawGrab();
    void clawRelease();
    void moveArmHorizontal(float, int);
    void moveArmVertical(float, int);
    int leftStepCounter();
    int rightStepCounter();
    boolean detectFront();
    boolean detectBack();
    boolean detectLeft();
    boolean detectRight();
    boolean detectFruit();
};

#endif
