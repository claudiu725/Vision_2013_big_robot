#include <Servo.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>

#include <elapsedMillis.h>
#include "VisionStepper.h"

#define DELAY_BETWEEN_TOGGLE 500
#define STATE_TEST_A 0
#define STATE_TEST_B 1
#define STATE_TEST_C 2
#define STATE_TEST_D 3
#define STATE_TEST_E 4
#define STATE_TEST_F 5
#define STATE_TEST_G 6

int enablePin = 7;
int stepPin = 6;
int directionPin = 5;

int buttonTestPin = 40;

int enablePin2 = 4;
int stepPin2 = 3;
int directionPin2 = 2;

int led = 13;

elapsedMillis time, test;
boolean wait;
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
  time = 0;
  state = 0;
  wait = false;
}

void loop()
{ 
  switch (state)
  {
    case STATE_TEST_A:
      //move forward
      motorA.setMaxSpeed();
      motorA.setDirectionForward();
      
      motorB.setMaxSpeed();
      motorB.setDirectionForward();
      motorB.toggleDirection();
      
      motorA.doDistanceInCm(100);
      motorB.doDistanceInCm(100);
      
      state = STATE_TEST_B;
      break;
    case STATE_TEST_B:
      //wait to complete
      if (motorA.isOff() && motorB.isOff())
        state = STATE_TEST_C;
      break;
    case STATE_TEST_C:
      //move left opposing
      delay(1000);
      state = STATE_TEST_D;
      break;
    case STATE_TEST_D:
      //wait to complete
      motorA.toggleDirection();
      motorB.toggleDirection();
      
      motorA.setMaxSpeed();
      motorB.setMaxSpeed();
      
      motorA.doDistanceInCm(100);
      motorB.doDistanceInCm(100);
      
      state = STATE_TEST_E;
      break;
    case STATE_TEST_E:
      //move left curved
      if (motorA.isOff() && motorB.isOff())
        state = STATE_TEST_F;
      break;
    case STATE_TEST_F:
      //wait to complete
      delay(1000);
      state = STATE_TEST_A;
      break;
    case STATE_TEST_G:
      //stop
      break;
  }
  
  motorA.doLoop();
  motorB.doLoop();
}


