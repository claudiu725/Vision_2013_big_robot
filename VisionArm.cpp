#include "VisionArm.h"
#include "pins_big_robot.h"
#include "big_robot_constants.h"
    
const int delayActions = 4000;
int clawPos = 20;
int leftEncoderState;
int rightEncoderState;
int leftStepCount = 0;
int rightStepCount = 0;

void VisionArm::init()
{
  fruitBarrier.initPin(fruitBarrierPin);
  fruitBarrier.toggleNegate();
  //fruitColor.initPin(fruitColorSenzorPin);
  
  horizontalLimiter.initPin(horizontalArmLimiterPin);
  horizontalLimiter.setAsPullup();
  horizontalAntiSlip.initPin(horizontalArmAntiSlipPin);
  horizontalAntiSlip.setAsPullup();
  
  horizontalMotor.init();
  horizontalMotor.initPins(horizontalArmEnablePin, horizontalArmDirectionPin, horizontalArmStepPin);
  horizontalMotor.initDelays(horizontalArmStartSpeedDelay, highPhaseDelay, pauseSpeedDelay, delayBeforeTurnOff, horizontalArmStepSpeedCounterAcceleration, horizontalArmStepSpeedCounterSlowing);
  horizontalMotor.initStepCmRatio(horizontalArmCmStepRatio);
  
  verticalLimiter.initPin(verticalArmLimiterPin);
  verticalLimiter.setAsPullup();
  
  verticalMotor.init();
  verticalMotor.initPins(verticalArmEnablePin, verticalArmDirectionPin, verticalArmStepPin);
  verticalMotor.initDelays(verticalArmStartSpeedDelay, verticalArmHighPhaseDelay, pauseSpeedDelay, delayBeforeTurnOff, verticalArmStepSpeedCounterAcceleration, verticalArmStepSpeedCounterSlowing);
  verticalMotor.initStepCmRatio(verticalArmCmStepRatio);
  
  claw.attach(clawServoPin);
  clawGrab();
  
  basket.attach(clawBasketPin);
  basketClose();
  
  lance.attach(lanceServoPin);
  lanceRaise();
}

void VisionArm::moveHorizontal(float distance, int side)
{     
  horizontalMotor.setDirectionForward();
  horizontalDirection = side;
  if(side == BACKWARD)
    horizontalMotor.toggleDirection();      
  horizontalMotor.setTargetDelay(horizontalArmSpeedDelay);
  horizontalMotor.doDistanceInCm(distance);
}

void VisionArm::moveVertical(float distance, int side)
{      
  verticalMotor.setDirectionForward();
  verticalDirection = side;
  if(side == DOWN)
    verticalMotor.toggleDirection();      
  verticalMotor.setTargetDelay(verticalArmSpeedDelay);
  verticalMotor.doDistanceInCm(distance);
}

void VisionArm::clawRelease()
{
  claw.write(140);
}

void VisionArm::clawGrab()
{
  claw.write(35);
}

void VisionArm::basketClose()
{
  basket.write(0);
}

void VisionArm::basketOpen()
{
  basket.write(89);
}

void VisionArm::lanceRaise()
{
  lance.write(0);
}

void VisionArm::lanceLower()
{
  lance.write(89);
}

boolean VisionArm::isStopped()
{
  return horizontalMotor.isOff() && verticalMotor.isOff();
}

void VisionArm::doLoop()
{
  if (verticalLimiter.detect() && verticalDirection == DOWN)
    verticalMotor.stopNow();
  horizontalMotor.doLoop();
  verticalMotor.doLoop();
}

void VisionArm::stopNow()
{
  horizontalMotor.stopNow();
  verticalMotor.stopNow();
}
