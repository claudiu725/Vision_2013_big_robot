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

VisionBase base;
VisionArm arm;
boolean ignoreSensors = false;

VisionState baseState, armState, robotState, clawState;

boolean fruitWasDetected;

void setup()
{
  Serial.begin(9600);
  base.init();
  arm.init();
  ignoreSensors = true;
  
  baseState.wait(1000, STATE_STOP);
  armState.waitFor(armVerticalStop, STATE_STOP);
  robotState.wait(NINETYSECONDS, STATE_STOP);
  clawState.wait(1000, STATE_STOP);
}

#define RETRIEVE_A 60
#define RETRIEVE_B 100

int retrieveOption;

void loop()
{
  Serial.print(base.frontLeft.detect());
  Serial.print(base.frontFront.detect());
  Serial.print(base.frontRight.detect());
  Serial.print(base.left.detect());
  Serial.print(base.right.detect());
  Serial.print(base.backLeft.detect());
  Serial.print(base.backBack.detect());
  Serial.println(base.backRight.detect());
  switch (robotState)
  {
    case 0:
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
      base.moveForward(36,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 1:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 2:
      base.moveBackward(13,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 3:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 4:
      base.moveForward(60,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 5: //PRIMUL FRUCT
      armState = 0;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 6:
      base.moveForward(7,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 7: //AL DOILEA FRUCT
      armState = 10;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 8:  
      base.moveForward(10, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 9: //AL TREILEA FRUCT
      armState = 10;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 10:
      base.moveForward(27 , mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 11:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 12:
      base.moveForward(22, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 13: //primul fruct
      armState = 0;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 14:
      base.moveForward(7,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 15: //AL DOILEA FRUCT
      armState = 10;
      baseState.save();
      baseState = STATE_STOP;
      break;
    case 16:  
      base.moveForward(10, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 17: //AL TREILEA FRUCT
      armState = 10;
      baseState.save();
      baseState = STATE_STOP;
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

  base.checkObstructions();
  if (!base.isStopped())
  {
    if (base.obstructionDetected == true && ignoreSensors == false)
      base.pause();
    else
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

void timeIsUpStopEverything()
{
  base.stopNow();
  arm.stopNow();
  baseState = STATE_STOP;
  armState = STATE_STOP;
}
