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

VisionBase base;
VisionArm arm;
VisionLance lance;
boolean ignoreSensors = false;

VisionState baseState, armState;

void setup()
{
  //Serial.begin(9600);
  //Serial.println("setup");

  base.init();
  arm.init();
  lance.init();
  ignoreSensors = false;
  
  baseState.wait(1000, 0);
  armState.wait(1000, 0);
}

void loop()
{ 
  switch (baseState)
  {
    case 0:
      base.moveForward(45,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 1:
      base.moveBackward(45,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 2:
      break;
    case 3:
      break;
    default:
      baseState.doLoop();
  }
  
  //*************************************************************************//
  
  switch (armState)            // arm switch
  {
    case 0:
      arm.clawRelease();
      break;
    case 1:
      arm.moveHorizontal(5, FORWARD);
      armState.waitFor(armStop, STATE_NEXT);
      break;
    case 2:   
      arm.moveVertical(3, DOWN);
      armState.waitFor(fruitDetect, STATE_NEXT);
      break;      
    case 3:
      arm.verticalMotor.stopNow();
      arm.clawGrab();
      armState++;
      break;
    case 4:
      arm.moveHorizontal(20, BACKWARD);
      arm.moveVertical(50, DOWN);
      armState.waitFor(armStop, STATE_NEXT);
      break;
    case 5:
      arm.clawRelease();
      armState.waitFor(armStop, STATE_STOP);
      break;
    case 6:
      if (arm.fruit.detect())
      {
        arm.clawGrab();
        armState.wait(1000, 7);
      }
      break;
    case 7:
      arm.clawRelease();
      armState.wait(250, 6);
      break;
    case 8:
      arm.moveVertical(3, DOWN);
      armState.waitFor(verticalLimiterTrigger, STATE_NEXT);
      break;
    case 9:
      arm.verticalMotor.stopNow();
      armState.wait(100, 10);
      break;
    case 10:
      arm.moveVertical(9.5, UP);
      armState = STATE_STOP;
      break;
    default:
      armState.doLoop();
  }

  //*************************************************************************//

  base.checkObstructions();
  if(base.obstructionDetected == true && ignoreSensors == false)
    base.pause();
  else
    base.unpause();

  base.doLoop();
  arm.doLoop();
  lance.doLoop();
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
