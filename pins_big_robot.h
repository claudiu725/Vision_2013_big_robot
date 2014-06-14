#ifndef pins_big_robot_h
#define pins_big_robot_h

#include "Arduino.h"

const int enablePin = 52;

//SENSOR LEVEL

//Front
const int frontLeftSenzorPin = A0;
const int frontRightSenzorPin = A3;
//Back
const int backLeftSenzorPin = A2;
const int backRightSenzorPin = A1;

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
const int verticalArmForwardPin = 22;
const int verticalArmBackwardPin = 24;
const int verticalArmEncoderPin = 18;

//Claw
const int clawMotorForwardPin = 26;
const int clawMotorBackwardPin = 28;
const int clawMotorEncoderPin = 46;
//const int clawMotorEncoderBPin = 48;

//OTHERS

const int basketPin = 35;
const int flipperPin = 37;
const int sensorScannerPin = 31;

#endif
