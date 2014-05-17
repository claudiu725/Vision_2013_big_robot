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
  base.init();
  arm.init();
  ignoreSensors = true;
  baseState.wait(1000, STATE_STOP);
  armState.wait(7000, 3);
  robotState.wait(NINETYSECONDS, 0);
  clawState.wait(1000, STATE_STOP);
}

void loop()
{
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
/*
  if (!arm.verticalMotor.isOff() && !arm.verticalLimiter.detect())
  {
    arm.verticalMotor.doLoop();
    return;
  }*/
  switch (baseState)
  {
    case 99:
      base.moveBackward(50,veryFastSpeedDelay);
      baseState.waitFor(baseStop, STATE_STOP);
      break;
    
    case 0:
      base.moveForward(30,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 1:
      base.turnRight(10);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 2:
      base.moveForward(11,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 3:
      base.turnRight(30);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 4:
      base.moveForward(10,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 5:
      base.turnLeft(40);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 6:
      base.moveForward(50,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 7:
      armState = 11;
      baseState = STATE_STOP;
      break;
    default:
      baseState.doLoop();
  }
  
  //*************************************************************************//
  switch (armState)            // arm switch
  {
    case 0:
      break;
      
    // vertical test 3-6
    case 3:
      arm.verticalMotor.setDirectionBackward();
      arm.verticalMotor.setNormalPwm(25);
      armState = 4;
      break;
    case 4:      
      arm.verticalMotor.doTimeMs(2000);
      armState.waitFor(fruitBarrierDetect, STATE_NEXT);
      break;
    case 5:
      arm.verticalMotor.toggleDirection();
      arm.verticalMotor.stopNow();
      armState.wait(4000, 4);
      break;
    case 6:
      armState.wait(250, 5);
      break;
      
    // horizontal test 7-10
    case 7:
      arm.moveHorizontal(10, FORWARD);
      armState.waitFor(armStop, STATE_NEXT);
      break;
    case 8:
      armState.wait(1000, STATE_NEXT);
      break;
    case 9:
      arm.moveHorizontal(14, BACKWARD);
      armState.waitFor(armStop, STATE_NEXT);
      break;
    case 10:
      armState.wait(1000,7);
      break;

    // arm open routine
    case 11:
      arm.moveHorizontal(17, FORWARD);
      armState.waitFor(armStop, STATE_NEXT);
      break;
    case 12:
      arm.clawRelease();
      clawState.wait(1000, 20);
      armState.wait(1000, STATE_STOP);
      break;

    case 30:
      armState.wait(1000, STATE_NEXT);
      break;
    case 31:
      armState.wait(1000, STATE_NEXT);
      break;
    case 32:
      arm.moveHorizontal(15, BACKWARD);
      armState.waitFor(armStop, STATE_NEXT);
      break;
    case 33:
      armState.wait(1000, 0);
      break;
    
    // shake routine
    case 59:
      arm.basketOpen();
      armState = STATE_NEXT;
      armState.save();
      armState.wait(1000, 60);
      break;
    case 60:
      base.turnLeft(10);
      armState.waitFor(baseStop, STATE_NEXT);
      break;
    case 61:
      base.turnRight(10);
      armState.waitFor(baseStop, STATE_NEXT);
      break;
    case 62:
      base.turnLeft(10);
      armState.waitFor(baseStop, STATE_NEXT);
      break;
    case 63:
      base.turnRight(10);
      armState.waitFor(baseStop, STATE_NEXT);
      break;
    case 64:
      base.turnLeft(10);
      armState.waitFor(baseStop, STATE_NEXT);
      break;
    case 65:
      arm.moveHorizontal(10, BACKWARD);
      base.turnRight(10);
      armState.waitFor(baseStop, STATE_NEXT);
      break;
    case 66:
      base.turnLeft(10);
      armState.waitFor(baseStop, STATE_NEXT);
      break;
    case 67:
      arm.moveHorizontal(10, FORWARD);
      base.turnRight(10);
      armState.waitFor(baseStop, STATE_NEXT);
      break;
    case 68:
      arm.basketOpen();
      armState.restore();
      break;

    case 80:
      break;
    default:
      armState.doLoop();
  }
  
  switch (clawState)
  {
    case 0:
      arm.moveHorizontal(10, FORWARD);
      clawState.waitFor(armStop, STATE_NEXT);
      break;
    case 1:
      arm.clawRelease();
      arm.moveHorizontal(5, FORWARD);
      clawState.waitFor(armStop, STATE_NEXT);
      break;
    case 2:
      arm.moveVertical(99, DOWN);
      clawState.waitFor(armStop, STATE_NEXT);
      break;
    case 3:
      arm.clawGrab();
      clawState.wait(500, STATE_NEXT);
      break;
    case 4:
      arm.moveHorizontal(7, BACKWARD);
      base.turnLeft(90);
      clawState.waitFor(armStop, STATE_NEXT);
      break;
    case 5:
      clawState.waitFor(baseStop, STATE_NEXT);
      break;
    case 6:
      arm.clawRelease();
      base.turnRight(90);
      clawState.waitFor(baseStop, STATE_NEXT);
      break;
      
    case 10:
      arm.moveHorizontal(15, BACKWARD);
      clawState.wait(500, STATE_NEXT);
      break;
    case 11:
      if (arm.fruitColor.isPurple())
        clawState = 3;
      else
        clawState = 4;
      break;
    case 12:
      arm.moveHorizontal(10, BACKWARD);
      break;
    case 13:
      break;
    case 14:
      arm.clawRelease();
      clawState.wait(1000, 0);
      break;

    case 20:
      arm.moveVertical(5, DOWN);
      clawState.waitFor(armStop, STATE_NEXT);
      break;
    case 21:
      if (fruitBarrierDetect())
      {
         arm.clawGrab();
         arm.moveUp();
         clawState = STATE_STOP;
      }
      else
         clawState.wait(100, STATE_NEXT);
      break;
    case 22:
      arm.moveVertical(2.5, DOWN);
      clawState.waitFor(armStop, STATE_NEXT);
      break;
    case 23:
      if (fruitBarrierDetect())
      {
         arm.clawGrab();
      }
      arm.moveUp();
      clawState = STATE_STOP;
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
  if (arm.verticalLimiter.detect() && arm.verticalDirection == DOWN)
    arm.verticalMotor.stopNow();
  if (arm.verticalLimiter.detect() && !arm.verticalMotor.isOff() && arm.verticalDirection == DOWN)
  {
    arm.verticalMotor.stopNow();
    arm.upDistance = 8.0;
  }

  base.doLoop();
  arm.doLoop();
}

boolean baseStop()
{
  return base.isStopped();
}

boolean armStop()
{
  return arm.isStopped();
}

boolean fruitBarrierDetect()
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
