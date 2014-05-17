#ifndef VisionBrushless_h
#define VisionBrushless_h

#include "Arduino.h"
#include "VisionSensor.h"
#include "VisionState.h"
#include <Servo.h>

class VisionBrushless {
  public:
    void init();
    void initPins(int brushlessPin, int relayPin);
    void initPwms(int stopPwm, int normalPwm);
    void initTopBottom(VisionSensor& front, VisionSensor& back);
    void initDirectionForward(boolean forward);
    void setDirectionForward();
    void setDirectionBackward();
    void toggleDirection();
    void doLoop();
    void setNormalPwm(int normalPwm);
    void doTimeMs(unsigned long time);
    void doTimeMicros(unsigned long time);
    void moveTo(VisionSensor& sensor);
    void stopNow();
    boolean isOff();
  private:
    void stopIfSensorDetect(VisionSensor& sensor);
  private:
    VisionState motorState;
    VisionSensor *front, *back, *sensorToGoTo;
    int currentInductivePosition;
    Servo brushless;
    boolean forwardDirection, directionPinState;
    int stopPwm, normalPwm;
    int brushlessPin, relayPin;
    unsigned long timeMs, timeMicros;
};

#endif
