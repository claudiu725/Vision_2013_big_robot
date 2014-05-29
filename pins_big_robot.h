#ifndef pins_big_robot_h
#define pins_big_robot_h

const int enablePin = 33;

//LOWER LEVEL

//Front
const int frontLeftSenzorPin = 11; // inversed
const int frontFrontSenzorPin = 12;
const int frontRightSenzorPin = 10;
//Sides
const int leftSenzorPin = 42;
const int rightSenzorPin = 48;
//Back
const int backLeftSenzorPin = 50;
const int backBackSenzorPin = 40;
const int backRightSenzorPin = 38;

//Left motor
const int leftMotorDirectionPin = 5;
const int leftMotorStepPin = 6;
const int leftMotorEnablePin = 7;

//Right motor
const int rightMotorDirectionPin = 2;
const int rightMotorStepPin = 3;
const int rightMotorEnablePin = 4;

//UPPER LEVEL

//Lance
const int lanceServoPin = 15;

//Horizontal arm
const int horizontalArmLimiterPin = 6;
const int horizontalArmAntiSlipPin = 39;
const int horizontalArmDirectionPin = 45;
const int horizontalArmStepPin = 47;
const int horizontalArmEnablePin = 49;

//Vertical arm
const int verticalArmLimiterPin = A15;
const int verticalArmDirectionRelayPin = 53;
const int verticalArmBrushlessPin = 51;
const int inductiveStartSensorPin = A6;
const int inductiveFruitHighSensorPin = A7;
const int inductiveFruitLowSensorPin = A5;

//Claw

const int clawServoPin = 999; //servo
const int clawBasketPin = 999; //servo

#endif
