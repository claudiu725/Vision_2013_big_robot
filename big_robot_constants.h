#ifndef big_robot_constants_h
#define big_robot_constants_h

const unsigned long defaultStartSpeedDelay = 50000L;
const unsigned long pauseSpeedDelay = 50000L;
const unsigned long highPhaseDelay = 50;
const float stepSpeedCounterAcceleration = 7;
const float stepSpeedCounterSlowing = 15;
const unsigned long delayBeforeTurnOff = 1000;

const unsigned long slowSpeedDelay = 7000;
const unsigned long fastSpeedDelay = 1000;
const unsigned long veryFastSpeedDelay = 300;
const unsigned long mediumSpeedDelay = 3000;
const unsigned long ultraSlowSpeedDelay = 24000;

const unsigned long turnSpeedDelay = 5000;

const float wheelDiameter = 12.3;
const float distanceBetweenWheels = 34.5; //valoare interioara
const int wheelRevolutionSteps = 200;

const int horizontalArmInsideAngle = 40;
const int horizontalArmOutsideAngle = 160;

const float verticalMotorStepsPerCm = 500;
const unsigned long verticalEncoderRevolutionSteps = 1600;

const float clawMotorStepsPerCm = 500;
const unsigned long clawEncoderRevolutionSteps = 1600;

const int flipperInAngle = 120;
const int flipperOutAngle = 45;

const int basketOpenAngle = 5;
const int basketSemiAngle = 80;
const int basketClosedAngle = 175;

const int sensorScannerLeft = 90;
const int sensorScannerMiddle = 130;
const int sensorScannerRight = 170;
const unsigned long sensorScannerToggleInterval = 350;

#endif
