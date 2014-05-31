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
const int horizontalArmPin = 15;

//Vertical arm
const int verticalArmForwardPin = 22;
const int verticalArmBackwardPin = 24;
const int verticalArmEncoderPin = 999;

//Claw
const int clawMotorForwardPin = 26;
const int clawMotorBackwardPin = 28;
const int clawMotorEncoderPin = 46;
//const int clawMotorEncoderBPin = 48;

//OTHERS

const int basketPin = 16;
const int flipperPin = 17;
const int sensorScannerPin = 14;

#endif
