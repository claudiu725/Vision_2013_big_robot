#ifndef big_robot_constants_h
#define big_robot_constants_h

const int defaultStartSpeedDelay = 8000;
const int ultraSlowStartSpeedDelay = 22000;
const int highPhaseDelay = 100;
const int maxSpeedDelay = 400;

const int verticalArmSpeedDelay = 800;
const int horizontalArmSpeedDelay = 5000;

const int slowSpeedDelay = 7000;
const int fastSpeedDelay = 1000;
const int mediumSpeedDelay = 3000;
const int ultraSlowSpeedDelay = 24000;

const float wheelDiameter = 12.3;
const float distanceBetweenWheels = 24.1; //valoare interioara
const int wheelRevolutionSteps = 200;

const float horizontalArmCmStepRatio = 30.86; // 16.2cm per 500 steps
const float verticalArmCmStepRatio = 4000.0; //2.5cm per 10000 steps

const float encoderRevolutionSteps = 24;

const int acuatorWaitTime = 1000;

#endif
