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
#include "VisionLance.h"
#include "VisionState.h"
#include "pins_big_robot.h"
#include "big_robot_constants.h"

#define NINETYSECONDS 88000L

VisionBase base;
VisionArm arm;
VisionLance lance;
VisionLance basket;
boolean ignoreSensors = false;

VisionState baseState, armState, robotState;

void setup()
{
  base.init();
  arm.init();
  lance.init(lanceServoPin);
  basket.init(clawBasketPin);
  ignoreSensors = true;
  
  baseState.wait(1000, 0);
  armState.wait(1000, STATE_STOP);
  robotState.wait(NINETYSECONDS, 0);
}

void loop()
{
  switch (baseState)
  {
    case 0:
      base.moveForward(250,fastSpeedDelay);
      baseState.waitFor(baseStop, 2);
      break;
    case 1:
      base.moveBackward(250,fastSpeedDelay);
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
      basket.bringTo(140);
      lance.bringTo(140);
      armState.wait(1000, STATE_NEXT);
      break;
    case 1:
      arm.moveHorizontal(5, FORWARD);
      arm.moveVertical(5, DOWN);
      armState.waitFor(armStop, STATE_NEXT);
      break;
    case 2:
      arm.moveHorizontal(5, BACKWARD);
      arm.moveVertical(5, UP);
      armState.waitFor(armStop, STATE_NEXT);
      break;      
    case 3:
      arm.clawGrab();
      basket.bringTo(35);
      lance.bringTo(35);
      armState.wait(1000, 0);
      break;
    case 4:
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
    */
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
      break;
    default:
      armState.doLoop();
  }
  
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

  //*************************************************************************//

  base.checkObstructions();
  if (base.obstructionDetected == true && ignoreSensors == false)
    base.pause();
  else
    base.unpause();

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

boolean fruitDetect()
{
  return arm.fruit.detect();
}

boolean verticalLimiterTrigger()
{
  return arm.verticalLimiter.detect();
}

void timeIsUpStopEverything()
{
  base.stopNow();
  arm.stopNow();
  baseState = STATE_STOP;
  armState = STATE_STOP;
}
