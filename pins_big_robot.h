#ifndef pins_big_robot_h
#define pins_big_robot_h

const int enablePin = 52;

//SENSOR LEVEL

//Front
const int frontLeftSenzorPin = 10;
const int frontRightSenzorPin = 11;
//Back
const int backLeftSenzorPin = 12;
const int backRightSenzorPin = 13;

//BASE LEVEL

//Left motor
const int leftMotorDirectionPin = 7;
const int leftMotorStepPin = 6;
const int leftMotorEnablePin = 5;

//Right motor
const int rightMotorDirectionPin = 4;
const int rightMotorStepPin = 3;
const int rightMotorEnablePin = 2;

//CLAW LEVEL

//Horizontal arm
const int horizontalArmPin = 33;

//Vertical arm
const int verticalArmForwardPin = A8;
const int verticalArmBackwardPin = A9;
const int verticalArmEncoderPin = 18;

//Claw
const int clawMotorForwardPin = A10;
const int clawMotorBackwardPin = A11;
const int clawMotorEncoderPin = 48;

//OTHERS

const int basketPin = 35;
const int flipperPin = 37;
const int sensorScannerPin = 31;

#endif
