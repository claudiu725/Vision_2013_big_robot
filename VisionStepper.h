#ifndef VisionStepper_h
#define VisionStepper_h

#include "Arduino.h"
#include <elapsedMillis.h>

class VisionStepper {
  public:
    void init(int enablePin, int directionPin, int stepPin);
    void doLoop();
    void toggleDirection();
    void setTargetDelay(int targetDelay);
    bool isOff();
    void doSteps(int stepNumber);
    void emergencyStop();
  private:
    void doSetup();
  private:
    int enablePin, directionPin, stepPin;
    int enablePinState, directionPinState, stepPinState;
    int globalState;
    unsigned long stepsMadeSoFar, stepsRemaining;
    unsigned long numberOfAccelerationSteps, numberOfDeaccelerationSteps;
    float accelerationDelayIncrement, deaccelerationDelayIncrement;
    float maxSpeedDelay, startSpeedDelay, currentDelay, targetDelay, highPhaseDelay;
    int stepSpeedCounter;
    boolean foundTargetSpeed, raiseSpeed;
    elapsedMicros stepTimer;
};

#endif
