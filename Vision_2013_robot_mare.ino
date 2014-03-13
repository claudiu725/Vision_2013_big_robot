#include <Servo.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>

#include <elapsedMillis.h>
#include "VisionStepper.h"

#define DELAY_BETWEEN_TOGGLE 500

int enablePin = 5;
int directionPin = 6;
int stepPin = 7;

int buttonTestPin = 40;

int enablePin2 = 9;
int directionPin2 = 10;
int stepPin2 = 11;

int led = 13;

elapsedMillis time;
bool wait;
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
  wait = false;
}

void loop()
{
  if (digitalRead(buttonTestPin) == LOW)
    digitalWrite(led, HIGH);
  else
    digitalWrite(led, LOW);
  
  if (wait)
  {
    if (time > DELAY_BETWEEN_TOGGLE)
    {
        motorA.toggleDirection();
        motorA.doSteps(5000);
        
        //motorB.toggleDirection();
        //motorB.doSteps(10000);
        
        wait = false;
        //digitalWrite(led, HIGH);
        //Serial.println("starting");
    }
  }
  else
  {
    if (motorA.isOff() && motorB.isOff())
    {
      time = 0;
      wait = true;
      //digitalWrite(led, LOW);
      //Serial.println("stopped");
    }
  }
  
  motorA.doLoop();
  motorB.doLoop();
}


