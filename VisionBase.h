#ifndef VisionBase_h
#define VisionBase_h

#include "Arduino.h"
#include "VisionEncoder.h"
#include "VisionStepper.h"
#include "VisionSensor.h"
#include "pins_big_robot.h"
#include "big_robot_constants.h"
#include <elapsedMillis.h>

#define NONE 0
#define FRONT 1
#define BACK 2
#define LEFT 3
#define RIGHT 4

class VisionBase {
  public:
    void init();
    void setStartDelays(int startDelay);
    void moveForward(float distance, int step_delay);
    void moveBackward(float distance, int step_delay);
    boolean frontDetected();
    boolean backDetected();
    void checkObstructions();
    void turnLeft(int angle);
    void turnRight(int angle);
    void pause();
    void unpause();
    void stopNow();
    void doLoop();
    boolean isStopped();
    boolean isPausing();
    void disable();
    void undo();
  public:
    VisionStepper leftMotor, rightMotor;
    VisionEncoder leftEncoder, rightEncoder;
    VisionSensor backLeft, backRight, frontLeft, frontRight;
    int directionMovement;
    boolean oppositeSide;
    boolean obstructionDetected;
};

#endif

