#ifndef VisionStepper_h
#define VisionStepper_h

#include "Arduino.h"
#include <elapsedMillis.h>

class VisionStepper {
  public:
    VisionStepper(int enablePin, int directionPin, int stepPin);
    void doLoop();
    void toggleDirection();
    bool isOff();
    void doSteps(int stepNumber);
  private:
    void doSetup();
  private:
    int enablePin, directionPin, stepPin;
    int enablePinState, directionPinState, stepPinState;
    int globalState;
    unsigned long stepsMadeSoFar, stepsRemaining;
    unsigned long numberOfAccelerationSteps, numberOfDeaccelerationSteps;
    unsigned long accelerationDelayIncrement, deaccelerationDelayIncrement;
    unsigned long lowSpeedDelay, highSpeedDelay, currentStepDelay, lowPhaseDelay;
    elapsedMicros stepTimer;
};

#endif
