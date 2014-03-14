#ifndef VisionStepper_h
#define VisionStepper_h

#include "Arduino.h"
#include <elapsedMillis.h>

class VisionStepper {
  public:
    void init(int enablePin, int directionPin, int stepPin);
    void doLoop();
    void toggleDirection();
    void setDirectionForward();
    void setTargetDelay(int targetDelay);
    boolean isOff();
    void doSteps(int stepNumber);
    void doDistanceInCm(float distance);
    void emergencyStop();
    void setMaxSpeed();
  private:
    void doSetup();
  private:
    int enablePin, directionPin, stepPin;
    int enablePinState, directionPinState, stepPinState;
    int globalState;
    unsigned long stepsMadeSoFar, stepsRemaining;
    float maxSpeedDelay, startSpeedDelay, currentDelay, targetDelay, highPhaseDelay;
    int stepSpeedCounter;
    boolean foundTargetSpeed, raiseSpeed;
    elapsedMicros stepTimer;
    elapsedMillis stopTimer;
    
    float wheelDiameter;
    float wheelRevolutionSteps;
    float stepCmRatio; // steps for a cm
    float degreeStepRatio;
};

#endif
