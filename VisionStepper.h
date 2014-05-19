  #ifndef VisionStepper_h
  #define VisionStepper_h
  
  #include "Arduino.h"
  #include <elapsedMillis.h>
  #include "VisionState.h"
  
  class VisionStepper {
    public:
      void init();
      void initPins(int enablePin, int directionPin, int stepPin);
      void setFullStep();
      void initDirectionForward(boolean forward);
      void initDelays(unsigned long startSpeedDelay, unsigned long highPhaseDelay, unsigned long maxSpeedDelay, unsigned long pauseSpeedDelay, float stepSpeedCounterAcceleration, float stepSpeedCounterSlowing);
      void initSizes(float wheelDiameter, int wheelRevolutionSteps, float distanceBetweenWheels);
      void initStepCmRatio(float stepCmRatio);
      void doLoop();
      void toggleDirection();
      void setDirectionForward();
      void setDirectionBackward();
      void setTargetDelay(unsigned long targetDelay);
      boolean isOff();
      boolean isAtTargetSpeed();
      void doSteps(unsigned long stepNumber);
      void doDistanceInCm(float distance);
      void doRotationInAngle(float angle);
      void setRemainingDistance(float distance);
      unsigned long getStepsFromDistance(float distance);
      void stopNow();
      void setMaxSpeed();
      void pause();
      void unpause();
      void setSpecial();
      void resetSpecial();
      void disable();
    private:
      void doSetup();
    private:
      int enablePin, directionPin, stepPin;
      int enablePinState, directionPinState, stepPinState;
      boolean forwardDirection;
      VisionState motorState, enableState, speedState, stepState;
      long stepsMadeSoFar, stepsRemaining;
      float stepSpeedCounter, stepSpeedCounterAcceleration, stepSpeedCounterSlowing;
      float startSpeedDelay, currentDelay, targetDelay, pauseSpeedDelay, delayBeforeTurnOff, highPhaseDelay, savedWhenPausingDelay;
      int old_state;
      elapsedMicros stepTimer;
      elapsedMillis stopTimer, pauseTurnOff;
      boolean special;
      boolean fullStep;
      
      float stepCmRatio; // steps for a cm
      float degreeStepRatio; //steps for a degree turn;
    private:
      unsigned long waitBeforeTurningOn;
      unsigned long waitBeforeTurningOff;
  };
  
  #endif

