#include <Servo.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>

#include <elapsedMillis.h>
#include "VisionStepper.h"

#define DELAY_BETWEEN_TOGGLE 500
#define STATE_TEST_A0 -1
#define STATE_TEST_A1 0
#define STATE_TEST_B 1
#define STATE_TEST_C 2
#define STATE_TEST_D 3
#define STATE_TEST_E 4
#define STATE_TEST_F 5
#define STATE_TEST_G 6
#define STATE_STOP   7
#define STATE_WAIT   8

void wait(int time_in_ms, int state);
elapsedMillis wait_time;
int time_to_wait, state_to_set_after_wait;

int enablePin = 7;
int stepPin = 6;
int directionPin = 5;

int buttonTestPin = 40;

int enablePin2 = 4;
int stepPin2 = 3;
int directionPin2 = 2;

int led = 13;

int state;
VisionStepper motorA;
VisionStepper motorB;

void setup()
{
  //Serial.begin(9600);
  //Serial.println("setup");
  motorA.init(enablePin, directionPin, stepPin);
  motorB.init(enablePin2, directionPin2, stepPin2);
  pinMode(led, OUTPUT);
  pinMode(buttonTestPin, INPUT_PULLUP);
  digitalWrite(led, LOW);
  delay(1000);
  state = STATE_TEST_A0;
}

void loop()
{ 
  switch (state)
  {
    case STATE_TEST_A0:
      //move forward
      motorA.setDirectionForward();
      motorB.setDirectionForward();
      motorB.toggleDirection();
      
      wait(500, STATE_TEST_A1);
      break;
   case STATE_TEST_A1:
      motorA.setMaxSpeed();
      motorB.setMaxSpeed();
   
      //motorA.doDistanceInCm(100);
      //motorB.doDistanceInCm(100);
      motorA.doSteps(3 * 200);
      motorB.doSteps(3 * 200);

      state = STATE_TEST_B;
      break;
    case STATE_TEST_B:
      //wait to complete
      if (motorA.isOff() && motorB.isOff())
        state = STATE_TEST_C;
      break;
    case STATE_TEST_C:
      //move left opposing
      state = STATE_TEST_D;
      break;
    case STATE_TEST_D:
      //wait to complete
      motorA.toggleDirection();
      motorB.toggleDirection();
      
      wait(500, STATE_TEST_A1);
      break;
    case STATE_TEST_E:
      //move left curved
      if (motorA.isOff() && motorB.isOff())
        state = STATE_TEST_F;
      break;
    case STATE_TEST_F:
      //wait to complete
      delay(1000);
      state = STATE_TEST_A0;
      break;
    case STATE_TEST_G:
      //stop
      break;
    case STATE_STOP:
      //stop
      break;
    case STATE_WAIT:
      if (wait_time > time_to_wait)
      {
        state = state_to_set_after_wait;
      }
      break;
  }
  
  motorA.doLoop();
  motorB.doLoop();
}

void wait(int time_in_ms, int state)
{
  wait_time = 0;
  time_to_wait = time_in_ms;
  state_to_set_after_wait = state;
}


