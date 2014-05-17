#ifndef VisionBrushless_h
#define VisionBrushless_h

#include "Arduino.h"
#include "VisionState.h"
#include <Servo.h>

class VisionBrushless {
  public:
    void init();
    void initPins(int brushlessPin, int relayPin);
    void initPwms(int stopPwm, int normalPwm);
    void initTimeCmRatio(float verticalArmCmTimeRatio);
    void initDirectionForward(boolean forward);
    void setDirectionForward();
    void setDirectionBackward();
    void toggleDirection();
    void doLoop();
    void doDistanceInCm(float distance);
    void setNormalPwm(int normalPwm);
    void doTimeMs(unsigned long time);
    void doTimeMicros(unsigned long time);
    void stopNow();
    boolean isOff();
  public:
    VisionState motorState;
    Servo brushless;
    boolean forwardDirection, directionPinState;
    int stopPwm, normalPwm;
    int brushlessPin, relayPin;
    unsigned long timeMs, timeMicros;
    float verticalArmCmTimeRatio;
};

#endif
