#include "VisionState.h"

void VisionState::wait(unsigned long timeInMs, int nextState)
{
  stateToSetAfterWait = processSpecialStates(nextState);
  *this = STATE_WAIT;
  time = 0;
  timeToWait = timeInMs;
}

void VisionState::waitMicros(unsigned long timeInMicros, int nextState)
{
  stateToSetAfterWait = processSpecialStates(nextState);
  *this = STATE_WAIT_MICROS;
  timeMicros = 0;
  timeToWaitInMicros = timeInMicros;
}

void VisionState::waitFor(boolean (*functionToTestFor)(), int nextState)
{
  stateToSetAfterWait = processSpecialStates(nextState);
  testFunction = functionToTestFor;
  *this = STATE_WAIT_FOR;
}

void VisionState::waitFor(boolean (*functionTestA)(), int nextStateA, boolean (*functionTestB)(), int nextStateB)
{
  stateToSetAfterWait = processSpecialStates(nextStateA);
  testFunction = functionTestA;
  stateToSetAfterWaitB = processSpecialStates(nextStateB);
  testFunctionB = functionTestB;
  *this = STATE_WAIT_FOR_BRANCH_SS;
}

int VisionState::processSpecialStates(int nextState)
{
  if (nextState == STATE_NEXT)
    return *this + 1;
  else if (nextState == STATE_LAST)
    return *this - 1;
  else return nextState;
}

void VisionState::doLoop()
{
  switch (state)
  {
    case STATE_STOP:
      break;
    case STATE_WAIT:
      if (time > timeToWait)
        *this = stateToSetAfterWait;
      break;
    case STATE_WAIT_MICROS:
      if (timeMicros > timeToWaitInMicros)
        *this = stateToSetAfterWait;
      break;
    case STATE_WAIT_FOR:
      if (testFunction())
        *this = stateToSetAfterWait;
      break;
    case STATE_WAIT_FOR_BRANCH_SS:
      if (testFunction())
        *this = stateToSetAfterWait;
      if (testFunctionB())
        *this = stateToSetAfterWaitB;
      break;
    case STATE_WAIT_FOR_BRANCH_TS:
      if (timeMicros > timeToWaitInMicros)
        *this = stateToSetAfterWait;
      if (testFunction())
        *this = stateToSetAfterWait;
      break;
  }
}

void VisionState::save()
{
  saveState = *this + 1;
}

void VisionState::restore()
{
  *this = saveState;
}

void VisionState::call(const int state)
{
  save();
  *this = state;
}

VisionState::operator int()
{
  return state;
}

int VisionState::operator =(const int state)
{
  if (state == STATE_NEXT)
    return ++(this->state);
  return (this->state = state);
}

VisionState& VisionState::operator+=(const int val) {
  *this = *this + val;
}

VisionState& VisionState::operator++() {
  *this += 1;
  return *this;
}

VisionState VisionState::operator++(int) {
  VisionState tmp(*this); // copy
  operator++(); // pre-increment
  return tmp;   // return old value
}
