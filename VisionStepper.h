#ifndef VisionStepper_h
#define VisionStepper_h

#include "Arduino.h"
#include <elapsedMillis.h>

class VisionStepper {
  public:
    void init();
    void initPins(int enablePin, int directionPin, int stepPin);
    void initDirectionForward(boolean forward);
    void initDelays(int startSpeedDelay, int highPhaseDelay, int maxSpeedDelay);
    void initSizes(float wheelDiameter, int wheelRevolutionSteps, float distanceBetweenWheels);
    void initStepCmRatio(float stepCmRatio);
    void doLoop();
    void toggleDirection();
    void setDirectionForward();
    void setDirectionBackward();
    void setTargetDelay(int targetDelay);
    boolean isOff();
    boolean isAtTargetSpeed();
    void doSteps(int stepNumber);
    void doDistanceInCm(float distance);
    void doRotationInAngle(float angle);
    void emergencyStop();
    void setMaxSpeed();
    void pause();
    void unpause();
    void setSpecial();
    void resetSpecial();
  private:
    void doSetup();
  private:
    int enablePin, directionPin, stepPin;
    int enablePinState, directionPinState, stepPinState;
    boolean forwardDirection;
    boolean special;
    int globalState;
    unsigned long stepsMadeSoFar, stepsRemaining;
    float maxSpeedDelay, startSpeedDelay, currentDelay, targetDelay, highPhaseDelay;
    int stepSpeedCounter;
    int old_state;
    boolean foundTargetSpeed, raiseSpeed;
    elapsedMicros stepTimer;
    elapsedMillis stopTimer;
    
    float stepCmRatio; // steps for a cm
    float degreeStepRatio; //steps for a degree turn;
};

#endif

