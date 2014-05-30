#ifndef pins_big_robot_h
#define pins_big_robot_h

const int enablePin = 999;

//SENSOR LEVEL

//Front
const int frontLeftSenzorPin = 999;
const int frontRightSenzorPin = 999;
//Back
const int backLeftSenzorPin = 999;
const int backRightSenzorPin = 999;

//Left motor
const int leftMotorDirectionPin = 5;
const int leftMotorStepPin = 6;
const int leftMotorEnablePin = 7;

//Right motor
const int rightMotorDirectionPin = 2;
const int rightMotorStepPin = 3;
const int rightMotorEnablePin = 4;

//UPPER LEVEL

//Horizontal arm
const int horizontalEncoderPin = 999;
const int horizontalArmDirectionPin = 999;
const int horizontalArmEnablePin = 999;

//Vertical arm
const int verticalDistancePin = 999;
const int verticalArmDirectionPin = 999;
const int verticalArmEnablePin = 999;

//Claw

const int clawMotorPin = 999; //servo
const int clawBasketPin = 999; //servo

#endif
