#ifndef VisionArm_h
#define VisionArm_h

#include "Arduino.h"
#include "VisionStepper.h"
#include "VisionSensor.h"
#include "VisionColorSensor.h"
#include <elapsedMillis.h>
#include <Servo.h>

#define UP 10
#define DOWN 11
#define FORWARD 12
#define BACKWARD 13

class VisionArm {
  public:    
    Servo claw;
    VisionSensor fruit;
    VisionColorSensor fruitColor;
    VisionSensor verticalLimiter;
    VisionStepper horizontalMotor;
    VisionStepper verticalMotor;
    void init();  
    void clawGrab();
    void clawRelease();
    void moveHorizontal(float, int);
    void moveVertical(float, int);
    void doLoop();
    boolean isStopped();
};

#endif
