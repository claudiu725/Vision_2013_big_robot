#include <Servo.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>

#include <elapsedMillis.h>
#include "VisionStepper.h"

#define DELAY_BETWEEN_TOGGLE 1000

int enablePin = 5;
int directionPin = 6;
int stepPin = 7;
elapsedMillis time;
bool wait;
VisionStepper motorA(enablePin, directionPin, stepPin);

void setup()
{
  delay(4000);
  time = 0;
  wait = false;
}

void loop()
{
  if (wait)
  {
    if (time > DELAY_BETWEEN_TOGGLE)
    {
        motorA.toggleDirection();
        motorA.doSteps(20000);
        wait = false;
    }
  }
  else
  {
    if (motorA.isOff())
    {
      time = 0;
      wait = true;
    }
  }
  
  motorA.doLoop();
}


