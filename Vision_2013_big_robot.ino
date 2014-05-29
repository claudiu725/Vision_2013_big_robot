#include <Servo.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>
#include <Wire.h>
#include <ADJDS311.h>

#include <elapsedMillis.h>
#include "VisionStepper.h"
#include "VisionBase.h"
#include "VisionArm.h"
#include "VisionState.h"
#include "VisionBrushless.h"
#include "pins_big_robot.h"
#include "big_robot_constants.h"

#define NINETYSECONDS 88000L

#define RED 0
#define YELLOW 1
#define TEST 2
#define ONEPOINT 3

VisionBase base;
VisionArm arm;
boolean ignoreSensors = false;
elapsedMillis enemyTimer;
int baseStartState;

VisionState baseState, robotState;
VisionSensor enableSensor;
int colorRedStartState, colorYellowStartState, testStartState, onePointStartState, color;
boolean robotRunning;
Servo servos[3];
int states[3], current, increment;

void setup()
{
  Serial.begin(9600);
  current = 0;
  servos[0].attach(30);
  servos[1].attach(40);
  servos[2].attach(50);
  states[0] = 45;
  states[1] = 45;
  states[2] = 45;
  increment = 5;
  servos[0].write(states[0]);
  servos[1].write(states[1]);
  servos[2].write(states[2]);
  
  base.init();
  arm.init();
  enableSensor.initPin(enablePin);
  enableSensor.setAsPullup();
  ignoreSensors = false;
  robotState = 0;
  baseState = STATE_STOP;

  colorRedStartState = 100;
  colorYellowStartState = 0;
  testStartState = 300;
  onePointStartState = 200;
  color = YELLOW; // RED YELLOW TEST ONEPOINT
}

void serialEvent()
{
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    switch (inChar) {
      case 'z':
        current = 0;
        break;
      case 'x':
        current = 1;
        break;
      case 'c':
        current = 2;
        break;
      case '`':
        states[current] = 45;
        servos[current].write(states[current]);
        break;
      case '1':
        states[current] = 5;
        servos[current].write(states[current]);
        break;
      case '2':
        states[current] = 15;
        servos[current].write(states[current]);
        break;
      case '3':
        states[current] = 25;
        servos[current].write(states[current]);
        break;
      case '4':
        states[current] = 35;
        servos[current].write(states[current]);
        break;
      case '5':
        states[current] = 45;
        servos[current].write(states[current]);
        break;
      case '6':
        states[current] = 55;
        servos[current].write(states[current]);
        break;
      case '7':
        states[current] = 65;
        servos[current].write(states[current]);
        break;
      case '8':
        states[current] = 75;
        servos[current].write(states[current]);
        break;
      case '9':
        states[current] = 85;
        servos[current].write(states[current]);
        break;
      case 'q':
        states[current] = 95;
        servos[current].write(states[current]);
        break;
      case 'w':
        states[current] = 105;
        servos[current].write(states[current]);
        break;
      case 'e':
        states[current] = 115;
        servos[current].write(states[current]);
        break;
      case 'r':
        states[current] = 125;
        servos[current].write(states[current]);
        break;
      case 't':
        states[current] = 135;
        servos[current].write(states[current]);
        break;
      case 'y':
        states[current] = 145;
        servos[current].write(states[current]);
        break;
      case 'u':
        states[current] = 155;
        servos[current].write(states[current]);
        break;
      case 'i':
        states[current] = 165;
        servos[current].write(states[current]);
        break;
      case 'o':
        states[current] = 175;
        servos[current].write(states[current]);
        break;
      case '+':
        states[current] += 5;
        servos[current].write(states[current]);
        break;
      case '-':
        states[current] -= 5;
        servos[current].write(states[current]);
        break;
    }
  }
}

void loop()
{
  switch (robotState)
  {
    case 0:
      robotRunning = false;
      robotState.waitFor(enableSensorDetect, STATE_NEXT);
      break;
    case 1:
      switch (color)
      {
        case RED:
          baseState.wait(5000,colorRedStartState);
          break;
        case YELLOW:
          baseState.wait(5000,colorYellowStartState);
          break;
        case TEST:
          baseState = testStartState;
          break;
        case ONEPOINT:
          baseState = onePointStartState;
          break;
      }
      robotRunning = true;
      robotState.wait(NINETYSECONDS, STATE_NEXT);
      break;
    case 2:
      robotRunning = false;
      timeIsUpStopEverything();
      robotState = STATE_STOP;
      break;
    default:
      robotState.doLoop();
      break;
  }

  switch (baseState)
  {
    case 0:
      base.moveForward(39,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 1:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 2:
      base.moveBackward(27,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 3:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 4:
      base.frontFront.disable();
      base.moveForward(23,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 5:
      base.frontFront.enable();
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;

    default:
      baseState.doLoop();
  }

  //*************************************************************************//
  if (robotRunning)
  {
    base.checkObstructions();
    if (!base.isStopped())
    {
      if (base.obstructionDetected == true && ignoreSensors == false)
      {
        base.pause();
        enemyTimer = 0;
      }
      else if (enemyTimer > 3000)
        base.unpause();
    }

    base.doLoop();
    arm.doLoop();
  }
}

boolean baseStop()
{
  return base.isStopped();
}

boolean armStop()
{
  return arm.isStopped();
}

boolean armVerticalStop()
{
  return arm.verticalMotor.isOff();
}

boolean enableSensorDetect()
{
  return enableSensor.detect();
}

void timeIsUpStopEverything()
{
  base.stopNow();
  arm.stopNow();
  base.disable();
  baseState = STATE_STOP;
}
