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
  fruitColor.initPin(fruitColorSenzorPin);
  
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
  
  sensorTop.initPin(inductiveStartSensorPin);
  sensorTop.initInductivePosition(3);
  
  sensorMiddle.initPin(inductiveFruitHighSensorPin);
  sensorMiddle.initInductivePosition(2);
  
  sensorBottom.initPin(inductiveFruitLowSensorPin);
  sensorBottom.initInductivePosition(0);
  
  verticalMotor.init();
  verticalMotor.initDirectionForward(LOW);
  verticalMotor.initPins(verticalArmBrushlessPin, verticalArmDirectionRelayPin);
  verticalMotor.initTopBottom(sensorTop, sensorBottom);
  verticalMotor.initPosition(sensorTop.inductivePosition);
  verticalMotor.initPwms(verticalArmStopPwm, verticalArmNormalPwm);
  
  claw.attach(clawServoPin);
  clawRelease();
  
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

void VisionArm::moveVertical(VisionSensor& sensor)
{
  verticalMotor.moveTo(sensor);
}

void VisionArm::clawRelease()
{
  claw.write(130);
}

void VisionArm::clawGrab()
{
  claw.write(20);
}

void VisionArm::basketClose()
{
  basket.write(3);
}

void VisionArm::basketOpen()
{
  basket.write(86);
}

void VisionArm::lanceRaise()
{
  lance.write(3);
}

void VisionArm::lanceLower()
{
  lance.write(86);
}

boolean VisionArm::isStopped()
{
  return horizontalMotor.isOff() && verticalMotor.isOff();
}

void VisionArm::doLoop()
{
  horizontalMotor.doLoop();
  verticalMotor.doLoop();
}

void VisionArm::stopNow()
{
  horizontalMotor.stopNow();
  verticalMotor.stopNow();
}
