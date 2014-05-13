#ifndef VisionState_h
#define VisionState_h

#include "Arduino.h"
#include <elapsedMillis.h>

#define STATE_STOP -1
#define STATE_WAIT -2
#define STATE_WAIT_MICROS -3
#define STATE_WAIT_FOR -4
#define STATE_NEXT -5

class VisionState {
  public:
    void wait(unsigned long timeInMs, int nextState);
    void waitMicros(unsigned long timeInMicros, int nextState);
    void waitFor(boolean (*functionToTestFor)(), int nextState);
    void doLoop();
    operator int();
    int operator =(const int state);
    VisionState& operator+=(const int val);
    VisionState& operator++();
    VisionState operator++(int);
  public:
    int state, stateToSetAfterWait;
    unsigned long timeToWait, timeToWaitInMicros;
    elapsedMillis time;
    elapsedMicros timeInMicros;
    boolean (*testFunction)();
};

#endif
