#ifndef big_robot_constants_h
#define big_robot_constants_h

const unsigned long defaultStartSpeedDelay = 20000L;
const unsigned long pauseSpeedDelay = 20000L;
const unsigned long highPhaseDelay = 50;
const float stepSpeedCounterAcceleration = 0.5;
const float stepSpeedCounterSlowing = 0.5;
const unsigned long delayBeforeTurnOff = 500;

const unsigned long slowSpeedDelay = 7000;
const unsigned long fastSpeedDelay = 1000;
const unsigned long mediumSpeedDelay = 3000;
const unsigned long ultraSlowSpeedDelay = 24000;

const float horizontalArmStepSpeedCounterAcceleration = 50;
const float horizontalArmStepSpeedCounterSlowing = 25;
const unsigned long horizontalArmStartSpeedDelay = 10000;
const unsigned long horizontalArmSpeedDelay = 5000;

const float verticalArmStepSpeedCounterAcceleration = 0.2;
const float verticalArmStepSpeedCounterSlowing = 50.0;
const unsigned long verticalArmStartSpeedDelay = 1000;
const unsigned long verticalArmSpeedDelay = 270;
const unsigned long verticalArmHighPhaseDelay = 10;

const float wheelDiameter = 12.3;
const float distanceBetweenWheels = 24.1; //valoare interioara
const int wheelRevolutionSteps = 200;

const float horizontalArmCmStepRatio = 30.86; // 16.2cm per 500 steps
const float verticalArmCmStepRatio = 4000.0; //2.5cm per 10000 steps

const float encoderRevolutionSteps = 24;

const int acuatorWaitTime = 1000;

#endif
