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
#include "VisionDC.h"
#include "pins_big_robot.h"
#include "big_robot_constants.h"

#define NINETYSECONDS 85000L

#define RED 0
#define YELLOW 1
#define TEST 2
#define ONEPOINT 3
#define YELLOW_FAST 4
#define RED_FAST 5

VisionBase base;
VisionArm arm;
boolean ignoreSensors = false;
elapsedMillis enemyTimer, totalPauseTimer;
int baseStartState;

VisionState baseState, robotState;
VisionSensor enableSensor;
int colorRedStartState, colorYellowStartState, testStartState, onePointStartState, color;
boolean robotRunning, avoidToTheLeft;

void setup()
{
  Serial.begin(19200);
  base.init();
  arm.init();
  enableSensor.initPin(enablePin);
  enableSensor.setAsPullup();
  ignoreSensors = false;
  avoidToTheLeft = true;
  robotState = 0;
  baseState = STATE_STOP;

  colorRedStartState = 100;
  colorYellowStartState = 0;
  testStartState = 300;
  onePointStartState = 100;
  color = YELLOW; // RED RED_FAST YELLOW YELLOW_FAST TEST ONEPOINT
}

void serialEvent()
{
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    double x;
    switch (inChar) {
      case 'w':
        x = Serial.parseFloat();
        x = x==0 ? 10 : x;
        base.moveForward(x,mediumSpeedDelay);
        Serial.print("Move forward:");
        Serial.println(x);
        break;
      case 's':
        x = Serial.parseFloat();
        x = x==0 ? 10 : x;
        base.moveBackward(x,mediumSpeedDelay);
        Serial.print("Move backward:");
        Serial.println(x);
        break;
      case 'a':
        x = Serial.parseFloat();
        x = x==0 ? 90 : x;
        base.turnLeft(x);
        Serial.print("Turn left:");
        Serial.println(x);
        break;
      case 'd':
        x = Serial.parseFloat();
        x = x==0 ? 90 : x;
        base.turnRight(x);
        Serial.print("Turn right:");
        Serial.println(x);
        break;
      case 't':
        arm.horizIn();
        Serial.println("horizIn");
        break;
      case 'g':
        arm.horizOut();
        Serial.println("horizOut");
        break;
      case 'y':
        arm.flipIn();
        Serial.println("flipIn");
        break;
      case 'h':
        arm.flipOut();
        Serial.println("flipOut");
        break;
      case 'u':
        arm.basketClose();
        Serial.println("basketClose");
        break;
      case 'j':
        arm.basketOpen();
        Serial.println("basketOpen");
        break;
      case 'i':
        robotRunning = false;
        Serial.println("robotRunning false");
        break;
      case 'k':
        robotRunning = true;
        Serial.println("robotRunning true");
        break;
        
      case 'z':
        arm.verticalMotor.go();
        Serial.println("verticalMotor go");
        break;
      case 'x':
        arm.verticalMotor.stopNow();
        Serial.println("verticalMotor stop");
        break;
      case 'c':
        arm.verticalMotor.toggleDirection();
        Serial.println("verticalMotor toggle");
        break;
      case 'Z':
        x = Serial.parseFloat();
        x = x==0 ? 1 : x;
        arm.verticalMotor.doTurns(x);
        Serial.print("verticalMotor doTurns ");
        Serial.println(x);
        break;
        
      case 'v':
        arm.clawMotor.go();
        Serial.println("clawMotor go");
        break;
      case 'b':
        arm.clawMotor.stopNow();
        Serial.println("clawMotor stop");
        break;
      case 'n':
        arm.clawMotor.toggleDirection();
        Serial.println("clawMotor toggleDirection");
        break;
      case 'V':
        x = Serial.parseFloat();
        x = x==0 ? 1 : x;
        arm.clawMotor.doTurns(x);
        Serial.print("clawMotor doTurns ");
        Serial.println(x);
        break;
        

      case ',':
        Serial.print("verticalMotor encoder ");
        Serial.println(arm.verticalMotor.encoder.count());
        break;
      case '.':
        Serial.print("clawMotor encoder ");
        Serial.println(arm.clawMotor.encoder.count());
        break;
      case '/':
        Serial.print("sensors ");
        Serial.print("backLeft ");
        Serial.print(base.backLeft.detect());
        Serial.print("backRight ");
        Serial.print(base.backRight.detect());
        Serial.print("frontLeft ");
        Serial.print(base.frontLeft.detect());
        Serial.print("frontRight ");
        Serial.println(base.frontRight.detect());
        break;
      default:
        Serial.print(inChar);
    }
  }
}

void loop()
{
  switch (robotState)
  {
    case 0:
      robotRunning = false;
      if (base.frontAll.detect())
        base.frontAll.disable();
      robotState.waitFor(enableSensorDetect, STATE_NEXT);
      break;
    case 1:
      switch (color)
      {
        case RED_FAST:
        case RED:
          baseState.wait(2000,colorRedStartState);
          break;
        case YELLOW_FAST:
        case YELLOW:
          baseState.wait(2000,colorYellowStartState);
          break;
        case TEST:
          baseState.wait(4000,testStartState);
          break;
        case ONEPOINT:
          baseState.wait(15000,onePointStartState);
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
      ignoreSensors = true;
      base.moveBackward(3,slowSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 1:
      base.moveForward(35,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 2:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 3:
      base.moveBackward(4.5,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 4:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 5:
      arm.basketSemi();
      base.moveForward(103,1000);
      baseState.waitFor(baseHalf, STATE_NEXT);
      break;
    case 6:
      arm.basketClose();
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 7:
      base.moveBackward(23,1000);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 8:
      ignoreSensors = false;
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 9:
      base.moveBackward(40,2000);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 10:
      arm.horizOut();
      base.turnLeft(90, 500);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 11:
      arm.horizIn();
      base.turnRight(90 + 35);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 12:
      base.moveBackward(30,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 13:
      base.turnLeft(35);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 14:
      base.moveBackward(20,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 15:
      arm.horizOut();
      base.turnLeft(360, 500);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 16:
      arm.horizIn();
      baseState.wait(500, STATE_NEXT);
      break;
    case 17:
      base.moveForward(85,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 18:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 19:
      base.moveForward(42,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 20:
      base.turnRight(180);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 21:
      arm.horizOut();
      base.turnRight(180);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 22:
      arm.horizIn();
      baseState.wait(0, STATE_NEXT);
      break;

    //RED
    
      
    case 200:
      base.moveBackward(80, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_STOP);
      break;
      
    case 300:
      arm.horizOut();
      baseState.wait(2500, STATE_NEXT);
      break;
    case 301:
      base.turnLeft(270, 500);
      baseState.wait(2500, STATE_NEXT);
      break;
    case 302:
      base.turnRight(270, 500);
      baseState.wait(2500, 301);
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
        if (!base.isPausing())
          totalPauseTimer = 0;
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

boolean waitedEnough()
{
  return base.isPausing() && (totalPauseTimer > 5000);
}

boolean baseStop()
{
  return base.isStopped();
}

boolean baseHalf()
{
  return base.leftMotor.stepsRemaining < base.leftMotor.stepsMadeSoFar;
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
