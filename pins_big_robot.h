#ifndef pins_big_robot_h
#define pins_big_robot_h

//LOWER LEVEL

//Front
const int frontLeftSenzorPin = 50;
const int frontFrontSenzorPin = 48;
const int frontRightSenzorPin = 42;
//Sides
const int leftSenzorPin = 40;
const int rightSenzorPin = 38;
//Back
const int backLeftSenzorPin = 10;
const int backBackSenzorPin = 11;
const int backRightSenzorPin = 12;

//Left motor
const int leftMotorEncoderPin = 9;
const int leftMotorDirectionPinLeft = 26;
const int leftMotorStepPinLeft = 34;
const int leftMotorEnablePinLeft = 36;

//Right motor
const int rightMotorEncoderPin = 46;
const int rightMotorDirectionPin = 30;
const int rightMotorStepPin = 32;
const int rightMotorEnablePin = 28;

//UPPER LEVEL

//Lance
//const int lanceEnablePin = A0;
//const int lanceDirectionPinA = A1;
//const int lanceDirectionPinB = A2;

//Horizontal arm
//const int horizontalArmLimiterPin = 999;
//const int horizontalArmAntiSlipPin = 999;
const int horizontalArmDirectionPin = 45;
const int horizontalArmStepPin = 47;
const int horizontalArmEnablePin = 49;

//Vertical arm
const int verticalArmLimiterPin = 999;
const int verticalArmDirectionPin = 53;
const int verticalArmStepPin = 51;
const int verticalArmEnablePin = 43;

//Claw
const int fruitSenzorPin = 14; //(bariera optica)
const int fruitColorSenzorPin = 2; //led
//SCL = 21 !!! doesn't change
//SDA = 20 !!! doesn't change
//const int fruitDistancePin = 6;
const int clawPin = 999;

#endif
