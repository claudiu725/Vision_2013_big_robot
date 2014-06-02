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
    void initTurnCmRatio(float turnCmRatio);
    void setDirectionForward();
    void setDirectionBackward();
    void toggleDirection();
    void doLoop();
    void doTimeMs(unsigned long time);
    void doTimeMicros(unsigned long time);
    void doSteps(long stepNumber);
    void doTurns(float turns);
    void doCm(float distance);
    void stopNow();
    boolean isOff();
    void go();
    void stop();
  private:
    VisionState motorState;
    int targetPosition;
    int forwardPin, backwardPin;
    float turnCmRatio;
    long targetSteps;
    boolean forwardDirection, currentDirection;
    unsigned long timeMs, timeMicros;
  public:
    VisionEncoder encoder;
};

#endif
