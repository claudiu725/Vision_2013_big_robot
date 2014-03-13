#ifndef VisionStepper_h
#define VisionStepper_h

#include "Arduino.h"
#include <elapsedMillis.h>

class VisionStepper {
  public:
    void init(int enablePin, int directionPin, int stepPin);
    void doLoop();
    void toggleDirection();
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
    float lowSpeedDelay, highSpeedDelay, lowHighSpeedDelayDifference, currentStepDelay, highPhaseDelay, accelPercent, deaccelPercent;
    elapsedMicros stepTimer;
};

#endif
