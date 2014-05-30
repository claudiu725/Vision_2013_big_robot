#ifndef VisionDC_h
#define VisionDC_h

#include "Arduino.h"
#include "VisionSensor.h"
#include "VisionState.h"
#include <Servo.h>

class VisionDC {
  public:
    void init();
    void initPins(int brushlessPin, int relayPin);
    void initTopBottom(VisionSensor& front, VisionSensor& back);
    void initPosition(int inductivePosition);
    void initDirectionForward(boolean forward);
    void setDirectionForward();
    void setDirectionBackward();
    void toggleDirection();
    void doLoop();
    void doTimeMs(unsigned long time);
    void doTimeMicros(unsigned long time);
    void moveTo(VisionSensor& sensor);
    void stopNow();
    boolean isOff();
  private:
    void stopIfSensorDetect(VisionSensor& sensor);
  private:
    VisionState motorState;
//    VisionDistanceSensor positionSensor;
    int targetPosition;
    boolean forwardDirection, directionPinState;
    int DCMotorPin, relayPin;
    unsigned long timeMs, timeMicros;
};

#endif
