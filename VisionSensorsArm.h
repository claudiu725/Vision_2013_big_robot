#ifndef VisionSensorsArm_h
#define VisionSensorsArm_h

#include "Arduino.h"
#include "VisionStepper.h"
#include "VisionSensor.h"
#include "VisionColorSensor.h"
#include <elapsedMillis.h>
#include <Servo.h>
 

class sensors_and_arm {
  public:    
    Servo claw;
    VisionSensor backLeft, backBack, backRight, left, right, frontLeft, frontFront, frontRight, fruit;
    VisionColorSensor fruitColor;
    VisionSensor verticalArmLimiter;
    VisionStepper horizontalArmMotor;
    VisionStepper verticalArmMotor;
    void init();  
    void clawGrab();
    void clawRelease();
    void moveArmHorizontal(float, int);
    void moveArmVertical(float, int);
    boolean frontDetected();
    boolean leftDetected();
    boolean rightDetected();
    boolean backDetected();
};

#endif
