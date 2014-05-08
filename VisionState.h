#ifndef VisionState_h
#define VisionState_h

#include "Arduino.h"
#include "VisionBase.h"
#include <elapsedMillis.h>

#define STATE_STOP -1
#define STATE_WAIT -2
#define STATE_WAIT_FOR -3
#define STATE_NEXT -4

class VisionState {
  public:
    void wait(int time_in_ms, int state_after);
    void waitFor(boolean (*functionToTestFor)(), int nextState);
    void doLoop();
    operator int();
    int operator =(const int state);
    VisionState& operator+=(const int val);
    VisionState& operator++();
    VisionState operator++(int);
  public:
    int state, stateToSetAfterWait;
    long timeToWait;
    elapsedMillis time;
    boolean (*testFunction)();
};

#endif
