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
  verticalMotor.init();//initial merge SUS
  verticalMotor.initDirectionForward(HIGH);
  verticalMotor.initPins(verticalArmForwardPin, verticalArmBackwardPin);
  
  verticalMotor.encoder.initPin(verticalArmEncoderPin);
  verticalMotor.encoder.initRevolutionSteps(verticalEncoderRevolutionSteps);
  
  clawMotor.init();//initial INFILETEAZA (deschide gheara)
  clawMotor.initDirectionForward(HIGH);
  clawMotor.initPins(clawMotorForwardPin, clawMotorBackwardPin);
  clawMotor.encoder.initPin(clawMotorEncoderPin);
  clawMotor.encoder.initRevolutionSteps(clawEncoderRevolutionSteps);

  flipper.attach(flipperPin);
  flipIn();
  
  sensorScanner.attach(sensorScannerPin);
  sensorScanner.write(sensorScannerMiddle);
  
  basket.attach(basketPin);
  basketClose();
  
  horizontalMotor.attach(horizontalArmPin);
  horizIn();
}

void VisionArm::moveVertical(VisionSensor& sensor)
{
}

void VisionArm::clawRelease()
{
}

void VisionArm::clawGrab()
{
}

void VisionArm::basketClose()
{
  basket.write(basketClosedAngle);
}

void VisionArm::basketOpen()
{
  basket.write(basketOpenAngle);
}

void VisionArm::flipIn()
{
  flipper.write(flipperInAngle);
}

void VisionArm::flipOut()
{
  flipper.write(flipperOutAngle);
}

void VisionArm::horizIn()
{
  horizontalMotor.write(horizontalArmInsideAngle);
}
void VisionArm::horizOut()
{
  horizontalMotor.write(horizontalArmOutsideAngle);
}

boolean VisionArm::isStopped()
{
  return true;//verticalMotor.isOff();
}

void VisionArm::doLoop()
{
//  verticalMotor.doLoop();
  verticalMotor.encoder.doLoop();
  clawMotor.encoder.doLoop();
  if (sensorToggleTimer > sensorScannerToggleInterval)
  {
    if (sensorScanner.read() == sensorScannerLeft)
      sensorScanner.write(sensorScannerRight);
    else
      sensorScanner.write(sensorScannerLeft);
    sensorToggleTimer = 0;
  }
}

void VisionArm::stopNow()
{
//  verticalMotor.stopNow();
}
