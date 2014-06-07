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
  testStartState = 0;
  onePointStartState = 100;
  color = YELLOW_FAST; // RED RED_FAST YELLOW YELLOW_FAST TEST ONEPOINT
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
      ignoreSensors = false;
      arm.basketOpen();
      base.moveForward(103,1000);
      baseState.waitFor(baseHalf, STATE_NEXT);
      break;
    case 6:
      arm.basketClose();
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 7:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 8:
      arm.horizOut();
      base.moveBackward(40,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 9:
      arm.horizIn();
      baseState.wait(1000, STATE_NEXT);
      break;
    case 10:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 11:
      base.frontAll.disable();
      base.moveForward(20,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 12:
      base.frontAll.enable();
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 13:
      base.moveForward(35,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 14:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 15:
      base.moveBackward(15,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 16:
      base.moveForward(30,mediumSpeedDelay);
      baseState.waitFor(baseStop, 20);
      break;
      
    case 20:
      base.turnRight(90);
      if (color == YELLOW_FAST)
        baseState.waitFor(baseStop, 60);
      else
        baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 21:
      base.moveBackward(73, 3000);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 22:
      arm.horizOut();
      baseState.wait(500, STATE_NEXT);
      break;
    case 23:
      base.moveBackward(10, 3000);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 24:
      arm.horizIn();
      base.moveForward(18, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 25:
      base.moveBackward(51, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 26:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 27:
      base.moveForward(25,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 28:
      base.turnRight(87);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 29:
      base.moveForward(20,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 30:
      base.turnRight(45);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 31:
      base.moveForward(40,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 32:
      base.turnRight(45);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 33:
      base.moveForward(50,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 34:
      base.turnRight(45);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 35:
      base.moveForward(50,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 36:
      base.turnRight(45);
      baseState.waitFor(baseStop, 40);
      break;
      
    case 40:
      base.moveForward(10,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 41:
      base.moveBackward(10,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 42:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 43:
      base.moveForward(40,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 44:
      arm.basketOpen();
      baseState.wait(1000, STATE_NEXT);
      break;
    case 45:
      base.moveBackward(50,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    
    case 60:
      arm.horizOut();
      base.moveBackward(80,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 61:
      arm.horizIn();
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 62:
      base.moveForward(90,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      baseState.waitFor(waitedEnough, 80);
      break;
    case 63:
      if (avoidToTheLeft)
        base.turnLeft(90);
      else
        base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 64:
      base.moveForward(40,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 65:
      base.backLeft.disable();
      base.moveBackward(20,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 66:
      base.backLeft.enable();
      if (avoidToTheLeft)
        base.turnRight(90);
      else
        base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 67:
      base.moveForward(30,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 68:
      arm.basketOpen();
      baseState.wait(1000, STATE_NEXT);
      break;
    case 69:
      base.moveBackward(50,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
      
    case 80:
      base.undo(mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 81:
      if (avoidToTheLeft)
      {
        base.turnLeft(90);
        avoidToTheLeft = false;
      }
      else
      {
        base.turnRight(90);
        avoidToTheLeft = true;
      }
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 82:
      base.moveForward(50,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 83:
      if (avoidToTheLeft)
        base.turnLeft(90);
      else
        base.turnRight(90);
      baseState.waitFor(baseStop, 62);
      break;

    //RED
    case 100:
      ignoreSensors = true;
      base.moveBackward(3,slowSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 101:
      base.moveForward(35,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 102:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 103:
      base.moveBackward(4.5,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 104:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 105:
      ignoreSensors = false;
      arm.basketOpen();
      base.moveForward(103,1000);
      baseState.waitFor(baseHalf, STATE_NEXT);
      break;
    case 106:
      arm.basketClose();
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 107:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 108:
      arm.horizOut();
      base.moveBackward(40,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 109:
      arm.horizIn();
      baseState.wait(1000, STATE_NEXT);
      break;
    case 110:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 111:
      base.frontAll.disable();
      base.moveForward(20,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 112:
      base.frontAll.disable();
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 113:
      base.moveForward(35,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 114:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 115:
      base.moveBackward(15,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 116:
      base.moveForward(30,mediumSpeedDelay);
      baseState.waitFor(baseStop, 120);
      break;
      
    case 120:
      base.turnLeft(90);
      if (color == RED_FAST)
        baseState.waitFor(baseStop, 160);
      else
        baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 121:
      base.moveBackward(73, 3000);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 122:
      arm.horizOut();
      baseState.wait(500, STATE_NEXT);
      break;
    case 123:
      base.moveBackward(10, 3000);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 124:
      arm.horizIn();
      base.moveForward(18, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 125:
      base.moveBackward(51, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 126:
      base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 127:
      base.moveForward(25,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 128:
      base.turnLeft(87);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 129:
      base.moveForward(20,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 130:
      base.turnLeft(45);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 131:
      base.moveForward(40,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 132:
      base.turnLeft(45);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 133:
      base.moveForward(50,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 134:
      base.turnLeft(45);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 135:
      base.moveForward(50,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 136:
      base.turnLeft(45);
      baseState.waitFor(baseStop, 140);
      break;
      
    case 140:
      base.moveForward(10,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 141:
      base.moveBackward(10,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 142:
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 143:
      base.moveForward(40,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 144:
      arm.basketOpen();
      baseState.wait(1000, STATE_NEXT);
      break;
    case 145:
      base.moveBackward(50,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
      
    
    case 160:
      arm.horizOut();
      base.moveBackward(80,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 161:
      arm.horizIn();
      base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 162:
      base.moveForward(90,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      baseState.waitFor(waitedEnough, 180);
      break;
    case 163:
      if (avoidToTheLeft)
        base.turnRight(90);
      else
        base.turnLeft(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 164:
      base.moveForward(40,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 165:
      base.backLeft.disable();
      base.moveBackward(20,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 166:
      base.backLeft.enable();
      if (avoidToTheLeft)
        base.turnLeft(90);
      else
        base.turnRight(90);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 167:
      base.moveForward(30,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 168:
      arm.basketOpen();
      baseState.wait(1000, STATE_NEXT);
      break;
    case 169:
      base.moveBackward(50,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
      
    case 180:
      base.undo(mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 181:
      if (avoidToTheLeft)
      {
        base.turnRight(90);
        avoidToTheLeft = false;
      }
      else
      {
        base.turnLeft(90);
        avoidToTheLeft = true;
      }
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 182:
      base.moveForward(50,mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_NEXT);
      break;
    case 183:
      if (avoidToTheLeft)
        base.turnRight(90);
      else
        base.turnLeft(90);
      baseState.waitFor(baseStop, 62);
      break;
      
    case 200:
      base.moveBackward(80, mediumSpeedDelay);
      baseState.waitFor(baseStop, STATE_STOP);
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
