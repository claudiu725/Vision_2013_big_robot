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
    boolean leftDetected();
    boolean rightDetected();
    boolean backDetected();
    void checkObstructions();
    void turnLeft(int angle);
    void turnRight(int angle);
    void pause();
    void unpause();
    void doLoop();
    boolean isStopped();
  public:
    VisionStepper leftMotor, rightMotor;
    VisionEncoder leftEncoder, rightEncoder;
    VisionSensor backLeft, backBack, backRight, left, right, frontLeft, frontFront, frontRight;
    int directionMovement;
    boolean obstructionDetected;
};

#endif

