#ifndef big_robot_constants_h
#define big_robot_constants_h

const unsigned long defaultStartSpeedDelay = 50000L;
const unsigned long pauseSpeedDelay = 50000L;
const unsigned long highPhaseDelay = 50;
const float stepSpeedCounterAcceleration = 5;
const float stepSpeedCounterSlowing = 4;
const unsigned long delayBeforeTurnOff = 10000;

const unsigned long slowSpeedDelay = 7000;
const unsigned long fastSpeedDelay = 1000;
const unsigned long veryFastSpeedDelay = 300;
const unsigned long mediumSpeedDelay = 3000;
const unsigned long ultraSlowSpeedDelay = 24000;

const unsigned long turnSpeedDelay = 5000;

const float wheelDiameter = 12.3;
const float distanceBetweenWheels = 24.1; //valoare interioara
const int wheelRevolutionSteps = 200;

const int horizontalArmInsideAngle = 65;
const int horizontalArmOutsideAngle = 120;

const float verticalMotorStepsPerCm = 500;
const unsigned long verticalEncoderRevolutionSteps = 1600;

const float clawMotorStepsPerCm = 500;
const unsigned long clawEncoderRevolutionSteps = 1600;

const int flipperInAngle = 180;
const int flipperOutAngle = 45;

const int basketOpenAngle = 0;
const int basketClosedAngle = 155;

const int sensorScannerLeft = 130;
const int sensorScannerMiddle = 90;
const int sensorScannerRight = 65;
const unsigned long sensorScannerToggleInterval = 400;

#endif
