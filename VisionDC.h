#ifndef VisionDC_h
#define VisionDC_h

#include "Arduino.h"
#include "VisionEncoder.h"
#include "VisionState.h"
#include <Servo.h>

class VisionDC {
  public:
    void init();
    void initPins(int forwardPin, int backwardPin);
    void initDirectionForward(boolean forward);
    void initStepCmRatio(float stepCmRatio);
    void setDirectionForward();
    void setDirectionBackward();
    void toggleDirection();
    void doLoop();
    void doTimeMs(unsigned long time);
    void doTimeMicros(unsigned long time);
    void doSteps(unsigned long stepNumber);
    void doDistanceInCm(float distance);
    unsigned long getStepsFromDistance(float distance);
    void move(unsigned long cm);
    void stopNow();
    boolean isOff();
  private:
    void go();
    void stop();
  private:
    VisionState motorState;
    int targetPosition;
    int forwardPin, backwardPin;
    float stepCmRatio;
    boolean forwardDirection, currentDirection;
    unsigned long timeMs, timeMicros;
  public:
    VisionEncoder encoder;
};

#endif
