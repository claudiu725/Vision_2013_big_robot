#ifndef big_robot_constants_h
#define big_robot_constants_h

const unsigned long defaultStartSpeedDelay = 100000L;
const unsigned long highPhaseDelay = 100;
const unsigned long maxSpeedDelay = 400;

const unsigned long verticalArmSpeedDelay = 2000;
const unsigned long horizontalArmSpeedDelay = 5000;

const unsigned long slowSpeedDelay = 7000;
const unsigned long fastSpeedDelay = 1000;
const unsigned long mediumSpeedDelay = 3000;
const unsigned long ultraSlowSpeedDelay = 24000;

const unsigned long pauseSpeedDelay = 10000;

const float wheelDiameter = 12.3;
const float distanceBetweenWheels = 24.1; //valoare interioara
const int wheelRevolutionSteps = 200;

const float horizontalArmCmStepRatio = 30.86; // 16.2cm per 500 steps
const float verticalArmCmStepRatio = 4000.0; //2.5cm per 10000 steps

const float encoderRevolutionSteps = 24;

const int acuatorWaitTime = 1000;

#endif
