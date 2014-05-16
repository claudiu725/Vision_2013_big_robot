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
    Servo claw, basket, lance;
    VisionSensor fruitBarrier, fruitDistance;
    VisionColorSensor fruitColor;
    VisionSensor verticalLimiter, horizontalLimiter, horizontalAntiSlip;
    VisionStepper horizontalMotor;
    VisionStepper verticalMotor;
    int verticalDirection, horizontalDirection;
    void init();
    void clawGrab();
    void clawRelease();
    void basketClose();
    void basketOpen();
    void lanceRaise();
    void lanceLower();
    void moveHorizontal(float, int);
    void moveVertical(float, int);
    void doLoop();
    void stopNow();
    boolean isStopped();
};

#endif
