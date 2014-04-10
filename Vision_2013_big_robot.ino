#include <Servo.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>

#include <elapsedMillis.h>
#include "VisionStepper.h"
#include "VisionSensorsArm.h"
#include "pins_big_robot.h"
#include "big_robot_constants.h"

#define STATE_STOP -1
#define STATE_WAIT -2
#define STATE_WAIT_MOTORS_STOP -3

#define UP 10
#define DOWN 11
#define FORWARD 12
#define BACKWARD 13

#define FRONT 1
#define BACK 2
#define LEFT 3
#define RIGHT 4

elapsedMillis wait_time;
int time_to_wait, state_to_set_after_wait, armState_to_set_after_wait;
VisionStepper motorLeft;
VisionStepper motorRight;
sensors_and_arm SnA;
boolean obstructionDetected = false;
boolean motorsPaused = false;
boolean ignoreSensors = false;

int armState = 0;
int state = 0;
int shotBalls = 0;
int directionMovement = 0;

void setup()
{
  //Serial.begin(9600);
  //Serial.println("setup
  SnA.init();
  
  motorLeft.init();
  motorLeft.initPins(enablePinLeft, directionPinLeft, stepPinLeft);
  motorLeft.initDelays(startSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  motorLeft.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  motorRight.init();
  motorRight.initPins(enablePinRight, directionPinRight, stepPinRight);
  motorRight.initDelays(startSpeedDelay, highPhaseDelay, maxSpeedDelay); 
  motorRight.initSizes(wheelDiameter, wheelRevolutionSteps,distanceBetweenWheels);
  
  //pinMode(buttonTestPin, INPUT_PULLUP);
  obstructionDetected = false;
  motorsPaused = false;
  ignoreSensors = false;
  delay(1000);
  state = 1;
  armState = 2;
}

void loop()
{ 
  switch (state)            // movement switch
  {
    case 0:
      MoveForward(45,mediumSpeedDelay);
      waitForMotorsStop(state + 2);
      break;
    case 1:     
      break;      
    case 2:
      break;      
    case 3:    
      break;
    case STATE_WAIT:
      if (wait_time > time_to_wait)
      {
        state = state_to_set_after_wait;
      }
      break;
    case STATE_WAIT_MOTORS_STOP:
      if (motorLeft.isOff() && motorRight.isOff())
      {
        state = state_to_set_after_wait;
      }
  }
  
  //*************************************************************************//
  
  switch (armState)            // arm switch
  {    
    case 0:    
      break;
    case 1:
      SnA.moveArmHorizontal(5, FORWARD);
      waitForArmMotorsStop(armState + 1);
      break;
    case 2:   
      SnA.moveArmVertical(3, DOWN);
      //armState++;
      armState=0;
      break;      
    case 3:
      if(SnA.detectFruit())
      {
        SnA.verticalArmMotor.pause();
        SnA.clawGrab();
        armState++;
      }
      break;      
    case 4:    
      SnA.moveArmHorizontal(20, BACKWARD);
      SnA.moveArmVertical(50, DOWN);
      waitForArmMotorsStop(armState + 1);
      break;
    case 5:
      SnA.clawRelease();
      waitForArmMotorsStop(STATE_STOP);
      break;
    case STATE_STOP:   //stop
      break;
    case STATE_WAIT:
      if (wait_time > time_to_wait)
      {
        armState = armState_to_set_after_wait;
      }
      break;
    case STATE_WAIT_MOTORS_STOP:
      if (SnA.horizontalArmMotor.isOff() && SnA.verticalArmMotor.isOff())
      {
        armState = armState_to_set_after_wait;
      }
      break;
  }
  
  //*************************************************************************//
  
  obstructionDetected = false;
  if (SnA.detectFront() && directionMovement == FRONT)
  {
    //Serial.println("Front detected!");
    obstructionDetected = true;
  }
  if (SnA.detectBack() && directionMovement == BACK)
  {
    //Serial.println("Back detected!");
    obstructionDetected = true;
  }
  if (SnA.detectLeft() && directionMovement == LEFT)
  {
    //Serial.println("Left detected!");
    obstructionDetected = true;
  }
  if (SnA.detectRight() && directionMovement == RIGHT)
  {
    //Serial.println("Right detected!");
    obstructionDetected = true;
  }
  
  if(obstructionDetected == true && ignoreSensors == false)
  {
    motorLeft.pause();
    motorRight.pause();
    motorsPaused = true;
  }
  else
  {
    if(motorsPaused == true){
      motorLeft.unpause();
      motorRight.unpause();
      motorsPaused = false;
    }
  }

  motorLeft.doLoop();
  motorRight.doLoop();
  SnA.horizontalArmMotor.doLoop();
  SnA.verticalArmMotor.doLoop();
}

void wait(int time_in_ms, int state_after)
{
  state = STATE_WAIT;
  wait_time = 0;
  time_to_wait = time_in_ms;
  state_to_set_after_wait = state_after;
}

void waitForMotorsStop(int state_after)
{
  state = STATE_WAIT_MOTORS_STOP;
  state_to_set_after_wait = state_after;
}

void waitForArmMotorsStop(int state_after)
{
  armState = STATE_WAIT_MOTORS_STOP;
  armState_to_set_after_wait = state_after;
}

void MoveForward(float distance, int step_delay)
{       
  directionMovement = FRONT;
  motorLeft.setTargetDelay(step_delay);         
  motorRight.setTargetDelay(step_delay);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorRight.toggleDirection(); 
  motorLeft.doDistanceInCm(distance);
  motorRight.doDistanceInCm(distance);
}

void MoveBackward(float distance, int step_delay)
{       
  directionMovement = BACK;
  motorLeft.setTargetDelay(step_delay);         
  motorRight.setTargetDelay(step_delay);
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorLeft.toggleDirection();    
  motorLeft.doDistanceInCm(distance);
  motorRight.doDistanceInCm(distance);
}

void TurnLeft(int angle)
{      
  directionMovement = LEFT; 
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorRight.toggleDirection();
  motorLeft.toggleDirection();  
  motorLeft.doRotationInAngle(angle);
  motorRight.doRotationInAngle(angle); 
}

void TurnRight(int angle)
{  
  directionMovement = RIGHT;
  motorLeft.setDirectionForward();
  motorRight.setDirectionForward();
  motorLeft.doRotationInAngle(angle);
  motorRight.doRotationInAngle(angle);
}
