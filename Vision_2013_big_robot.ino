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

VisionState baseState, armState, robotState, clawState;
VisionSensor enableSensor;
int colorRedStartState, colorYellowStartState, testStartState, onePointStartState, color;
boolean fruitWasDetected;
boolean robotRunning;

void setup()
{
  base.init();
  arm.init();
  enableSensor.initPin(enablePin);
  enableSensor.setAsPullup();
  ignoreSensors = false;

  robotState = 0;
  baseState = STATE_STOP;
  armState.waitFor(armVerticalStop, STATE_STOP);
  clawState = STATE_STOP;
  
  colorRedStartState = 100;
  colorYellowStartState = 0;
  testStartState = 300;
  onePointStartState = 200;
  color = ONEPOINT; // RED YELLOW TEST ONEPOINT
}

#define RETRIEVE_A 60
#define RETRIEVE_B 100

int retrieveOption;

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
          base.frontFront.disable();
          break;
        case YELLOW:
          baseState.wait(5000,colorYellowStartState);
          base.frontFront.disable();
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
      base.moveForward(23,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 5:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 6:
      arm.moveHorizontal(17, FORWARD);
      baseState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 7:
      arm.moveHorizontal(17, BACKWARD);
      baseState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 8:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 9:
      base.moveForward(47,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 10:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 11:
      base.moveForward(60,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 12:
      arm.moveHorizontal(17, FORWARD);
      baseState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 13:
      base.moveBackward(20,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 14:
      arm.moveHorizontal(17, BACKWARD);
      baseState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
      
    case 50:
      base.moveForward(50, veryFastSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 51:
      base.moveBackward(50, veryFastSpeedDelay);
      baseState.waitFor(baseStop, 50);
      break;

    case 100:
      base.moveForward(39,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 101:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 102:
      base.moveBackward(15,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 103:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 104:
      base.moveForward(23,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 105:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 106:
      arm.moveHorizontal(17, FORWARD);
      baseState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 107:
      arm.moveHorizontal(17, BACKWARD);
      baseState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 108:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 109:
      base.moveForward(47,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 110:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 111:
      base.moveBackward(60,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 112:
      arm.moveHorizontal(17, FORWARD);
      baseState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 113:
      base.moveForward(20,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 114:
      arm.moveHorizontal(17, BACKWARD);
      baseState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
      
    case 200:
      base.moveForward(39,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 201:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 202:
      base.moveBackward(7,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 203:
      base.turnRight(180);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 204:
      base.moveForward(200,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;


    case 300:
      base.moveForward(39,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 301:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 302:
      base.moveBackward(27,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 303:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 304:
      base.moveForward(23,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 305:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 306:
      arm.moveHorizontal(17, FORWARD);
      baseState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 307:
      arm.moveHorizontal(17, BACKWARD);
      baseState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 308:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 309:
      base.moveForward(47,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    default:
      baseState.doLoop();
  }
  
  //*************************************************************************//
  switch (armState)
  {
    // arm open routine
    case 0:
      arm.clawRelease();
      arm.moveHorizontal(17, FORWARD);
      armState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 1:
      retrieveOption = RETRIEVE_B;
      clawState = 0;
      armState.save();
      armState = STATE_STOP;
      break;
    case 2:
      baseState.restore();
      armState = STATE_STOP;
      break;
      
    case 10:
      arm.clawRelease();
      arm.moveHorizontal(7, FORWARD);
      armState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 11:
      retrieveOption = RETRIEVE_A;
      clawState = 0;
      armState.save();
      armState = STATE_STOP;
      break;
    case 12:
      baseState.restore();
      armState = STATE_STOP;
      break;
    default:
      armState.doLoop();
  }
  
  switch (clawState)
  {
    //arm is already extended, check mid and low, stay lowered
    case 0:
      arm.moveVertical(arm.sensorBottom);
      clawState.waitFor(armVerticalStop, STATE_NEXT);
      break;
    case 1:
      if (fruitDetect())
        clawState.call(20);
      else
        clawState = STATE_NEXT;
      break;
    case 2:
      arm.moveVertical(arm.sensorMiddle);
      clawState.waitFor(armVerticalStop, STATE_NEXT);
      break;
    case 3:
      //if (fruitDetect())
        clawState.call(20);
      //else
        //clawState = STATE_NEXT;
      break;
    case 4:
      clawState = 40;//didn't find it
      break;

    //found something, grab it and check it to be purple
    case 20:
      arm.clawGrab();
      clawState.wait(500, STATE_NEXT);
      break;
    case 21:
      if (seePurple() || seeClear())
        clawState = retrieveOption; //got it
      else
      {
        arm.clawRelease();
        clawState.restore(); //return to case 0+
      }
      break;

    //failure, did not detect anything
    case 40:
      arm.moveVertical(arm.sensorTop);
      clawState.waitFor(armVerticalStop, STATE_NEXT);
      break;
    case 41:
      clawState = 80; //done
      break;      

    //found something, claw grabbed fruit and checked it to be purple
    //now take it up, pull it back and drop it in the cup
    // RETRIEVE FRUIT A
    case 60:
      arm.moveVertical(arm.sensorTop);
      clawState.waitFor(armVerticalStop, 62);
      break;
    case 62:
      arm.moveHorizontal(8, BACKWARD);
      clawState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 63:
      base.turnLeft(45);
      clawState.waitFor(baseStop, 65);
      break;
    case 65:
      arm.clawRelease();
      clawState.wait(1000, STATE_NEXT);
      break;
    case 66:
      arm.moveHorizontal(17, BACKWARD);
      clawState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 67:
      base.turnRight(45);
      clawState.waitFor(baseStop, STATE_NEXT);
      break;
    case 68:
      clawState = 81;
      break;
      
    //done
    case 80:
      arm.moveHorizontal(17, BACKWARD);
      clawState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 81:
      armState.restore();
      clawState = STATE_STOP;
      break;

    // RETRIEVE FRUIT B    
    case 100:
      arm.moveVertical(arm.sensorTop);
      clawState.waitFor(armVerticalStop, STATE_NEXT);
      break;
    case 101:
      arm.moveHorizontal(10, BACKWARD);
      clawState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 102:
      base.turnLeft(90);
      clawState.waitFor(baseStop,104);
      break;
      /*
    case 103:
      arm.moveHorizontal(7, FORWARD);
      clawState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
      */
    case 104:
      arm.clawRelease();
      arm.moveHorizontal(17, BACKWARD);
      clawState.waitFor(armHorizontalStop, STATE_NEXT);
      break;
    case 105:
      base.turnRight(90);
      clawState.waitFor(baseStop,STATE_NEXT);
      break;
    case 106:
      clawState = 81;
      break;

    default:
      clawState.doLoop();
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
    if (arm.fruitBarrier.detect())
      fruitWasDetected = true;

    if (arm.horizontalLimiter.detect() && !arm.horizontalMotor.isOff() && arm.horizontalDirection == BACKWARD)
      arm.horizontalMotor.setRemainingDistance(0.05);
    if (arm.horizontalAntiSlip.detect() && arm.horizontalMotor.isOff())
      arm.moveHorizontal(1, BACKWARD);

    base.doLoop();
    arm.doLoop();
  }
}

boolean seePurple()
{
  return arm.fruitColor.isPurple();
}

boolean seeBlack()
{
  return arm.fruitColor.isBlack();
}

boolean seeClear()
{
  return arm.fruitColor.isClear();
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

boolean armHorizontalStop()
{
  return arm.horizontalMotor.isOff();
}

boolean armHorizontalStopOrFruit()
{
  return arm.horizontalMotor.isOff() || arm.fruitBarrier.detect();
}

boolean fruitDetect()
{
  return arm.fruitBarrier.detect();
}

boolean verticalLimiterTrigger()
{
  return arm.verticalLimiter.detect();
}

boolean horizontalLimiterTrigger()
{
  return arm.horizontalLimiter.detect();
}

boolean horizontalAntiSlipTrigger()
{
  return arm.horizontalAntiSlip.detect();
}

boolean horizontalSensor()
{
  return arm.horizontalAntiSlip.detect() || arm.horizontalLimiter.detect();
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
  arm.disable();
  baseState = STATE_STOP;
  armState = STATE_STOP;
  clawState = STATE_STOP;
}
