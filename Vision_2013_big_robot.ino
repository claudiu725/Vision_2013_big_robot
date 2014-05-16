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
#include "pins_big_robot.h"
#include "big_robot_constants.h"

#define NINETYSECONDS 88000L

VisionBase base;
VisionArm arm;
boolean ignoreSensors = false;

VisionState baseState, armState, robotState, clawState;

void setup()
{
  base.init();
  arm.init();
  ignoreSensors = true;
  
  baseState.wait(1000, STATE_STOP);
  armState.wait(4000, 50);//STATE_STOP);//3);
  robotState.wait(NINETYSECONDS, 0);
  clawState.wait(1000, 2);
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
  
  if (!arm.verticalMotor.isOff() && !arm.verticalLimiter.detect())
  {
    arm.verticalMotor.doLoop();
    return;
  }
  switch (baseState)
  {
    case 0:
      base.moveForward(120,highPhaseDelay);
      baseState.waitFor(baseStop, 2);
      break;
    case 1:
      base.moveBackward(120,highPhaseDelay);
      baseState.waitFor(baseStop, 3);
      break;
    case 2:
      baseState.wait(1000, 1);
      break;
    case 3:
      baseState.wait(1000, 0);
      break;
    default:
      baseState.doLoop();
  }
  
  //*************************************************************************//
  switch (armState)            // arm switch
  {
    case 0:
      arm.clawRelease();
      armState.wait(1000, STATE_NEXT);
      break;
    case 1:
      arm.moveHorizontal(15, FORWARD);
      armState.waitFor(armStop, STATE_NEXT);
      break;
    case 2:
      armState.wait(1000, STATE_NEXT);
      break;
      
    // vertical test 3-6
    case 3:
      //arm.clawGrab();
      //arm.clawRelease();
      arm.moveVertical(99, DOWN);
      armState.waitFor(armStop, STATE_NEXT);
      break;
    case 4:
      armState.wait(1000, STATE_NEXT);
      break;
    case 5:
      //basket.bringTo(35);
      //lance.bringTo(35);
      //arm.clawRelease();
      arm.moveVertical(8.0, UP);
      armState.waitFor(armStop, STATE_NEXT);
      break;
    case 6:
      armState.wait(1000, 3);
      break;
      
    // horizontal test 7-9
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
    
    case 11:
      arm.moveHorizontal(10, FORWARD);
      armState.waitFor(armStop, STATE_NEXT);
      break;
    case 12:
      armState.wait(1000, STATE_NEXT);
      break;
    case 13:
      arm.moveVertical(99, DOWN);
      armState.waitFor(armStop, STATE_NEXT);
      break;
    case 14:
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
    case 50:
      arm.basketOpen();
      armState = STATE_NEXT;
      armState.save();
      armState.wait(1000, 60);
      break;
    case 51:
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
      arm.moveHorizontal(2, BACKWARD);
      base.turnRight(10);
      armState.waitFor(baseStop, STATE_NEXT);
      break;
    case 66:
      base.turnLeft(10);
      armState.waitFor(baseStop, STATE_NEXT);
      break;
    case 67:
      arm.moveHorizontal(2, FORWARD);
      base.turnRight(10);
      armState.waitFor(baseStop, STATE_NEXT);
      break;
    case 68:
      arm.basketOpen();
      armState.restore();
      break;

    /*
    basket.bringTo(90);
    if(ok)
    {
      if(value>70)
      {
        basket.bringTo(value++);
        lance.bringTo(value++);
      }
      else
        ok = false;
    }
    else
    {
      if(value<140)
      {
          basket.bringTo(value--);
          lance.bringTo(value--);
      }
      else
        ok = true;
    }
      break;
    case 5:
      arm.clawRelease();
      armState.waitFor(fruitDetect, STATE_NEXT);
      break;
    case 6:
      arm.clawGrab();
      armState.wait(1000, STATE_NEXT);
      break;
    case 7:
      arm.clawRelease();
      armState.wait(250, STATE_NEXT);
      break;
    case 8:
      arm.moveVertical(3, DOWN);
      armState.waitFor(verticalLimiterTrigger, STATE_NEXT);
      break;
    case 9:
      arm.verticalMotor.stopNow();
      armState.wait(100, STATE_NEXT);
      break;
    case 10:
      arm.moveVertical(9.5, UP);
      armState = STATE_STOP;
      break;*/
    default:
      armState.doLoop();
  }
  
  switch (clawState)
  {
    case 0:
      arm.clawRelease();
      clawState.waitFor(fruitBarrierDetect, STATE_NEXT);
      break;
    case 1:
      arm.clawGrab();
      //arm.moveVertical(99, DOWN);
      clawState.wait(1000, STATE_NEXT);
      break;
    case 2:
      arm.clawRelease();
      clawState.wait(1000, 0);
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
  
  if (arm.horizontalLimiter.detect() && !arm.horizontalMotor.isOff() && arm.horizontalDirection == BACKWARD)
    arm.horizontalMotor.setRemainingDistance(0.05);
    //arm.moveHorizontal(0.1, BACKWARD);
  if (arm.horizontalAntiSlip.detect() && arm.horizontalMotor.isOff())
    arm.moveHorizontal(0.1, BACKWARD);

  base.doLoop();
  arm.doLoop();
}

void grabOnPurple()
{
  if(arm.fruitColor.isPurple())
      arm.clawGrab();
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
