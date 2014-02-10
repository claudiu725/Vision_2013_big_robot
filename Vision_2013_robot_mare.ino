#include <Servo.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#include <TimerThree.h>

/*
  2 motoare brat x 3 pini
  2 motoare deplasare x 3 pini
  2 encodere x 1 pin
  brat - 2 pini (enable, level)
  cupa - 2 pini
  4 pini de senzori directie
  2 pini enable directie laterala
  2 pini enable encodere
  1 pin senzor de distanta
  2 pini senzor de culoare
*/

/*
  12.3 cm diametrul rotii
  1.8 degrees / step
  200 steps -> 360 degrees
  Encoders + Senzors - 2, 3, 21, 20, 19, 18 
  colours : 1-red, 2-blue, 3-green
*/
const int ColourPin = 8; // 0 - left/blue , 1 - right/red
const int StrategyPin = 49; // 0 - gifts , 1 - candles

const int FrontSenzorPin = 46;
const int BackSenzorPin = 45;
const int LeftSenzorPin = 43;
const int RightSenzorPin = 42;

//const int S0 = 6;//pinB
//const int S1 = 7;//pinA
//const int S2 = 8;//pinE
//const int S3 = 9;//pinF
//const int taosOutPin = 53;//pinC

int selectedSide = 0;
int selectedStrategy = 0;
int teamColour = 2;
int strategyStep = 0;

const float fastSpeedDelay = 3;
const float slowSpeedDelay = 5;
const float accelerationSteps = 80;
const float delayRate = 0.025;

const float armFastSpeedDelay = 5;
const float armSlowSpeedDelay = 7;
const float armAccelerationSteps = 80;
const float armDelayRate = 0.025;

// 0 - left , 1 - right , 2 - extend arm , 3 - raise/lower
const int numberOfMotors = 4; 
int motorEnablePins[numberOfMotors] = {51,52,41,40};

byte motorSteps[4][4] = {
                          {B10101010, B10100110, B10100101, B10101001},
                          {B01101010, B01100110, B01100101, B01101001},
                          {B01011010, B01010110, B01010101, B01011001},
                          {B10011010, B10010110, B10010101, B10011001}
                          };

//int platformMotorPins[] = {37,36},
//    platformMotorSteps = 200;

//Stepper platformMotor(platformMotorSteps, platformMotorPins[0],platformMotorPins[1]);
//LiquidCrystal lcd(13, 12, 11, 10, 5, 4);
      
int motorCounters[2] = {0,0}; //  index: 0 - left, 1 - right
const int wheelDiameter = 12.3; //cm   
const int wheelRevolutionSteps = 200;
const float stepCmRatio = (float)wheelRevolutionSteps/wheelDiameter; // steps for a cm
const float degreeStepRatio = 5.8;
String direction = "";
volatile boolean frontDetected = false;
volatile boolean leftDetected = false;
volatile boolean rightDetected = false;
volatile boolean backDetected = false;
volatile int stepsLeft = 0;
volatile int stepsRight = 0;
boolean completeSteps = false;
const int delayActions = 1500;
volatile int timer = 0;

      
void MotionMotors();
void MoveForward(float);
void MoveBackwards(float);
void TurnLeft(int);
void TurnRight(int);
//void PlatformRotateLeft(int);
//void PlatformRotateRight(int);
void EncoderLeft();
void EncoderRight();
void SenzorFront();
void SenzorLeft();
void SenzorRight();
void SenzorBack();
void ArmExtend();
void ArmDrop();
void BlowCandle();
void PushGift();
void BlowAir();
void ColourSenzorExtend();
void TimeUp();
void BalloonTime();
void StopMotors();
void StartMotors();
void StopEverything();
        /*
void setup()
{
  Timer3.initialize();
  Timer3.attachInterrupt(StopEverything,1000000);
  //pinMode(50, OUTPUT);
  //digitalWrite(50, HIGH);
  pinMode(47, OUTPUT);
  pinMode(47, HIGH);
  pinMode(StopLedsPin, OUTPUT);
  pinMode(BirthdaySoundPin, OUTPUT);
  pinMode(LeftSignalingLedsPin, OUTPUT);
  pinMode(RightSignalingLedsPin, OUTPUT);
  pinMode(ArmDropPin, OUTPUT);
//  TCS3200setup();
  
//  attachInterrupt(0, EncoderLeft, FALLING);
//  attachInterrupt(1, EncoderRight, FALLING);
  attachInterrupt(1, SenzorBack, CHANGE);
  attachInterrupt(0, SenzorFront, CHANGE);
  attachInterrupt(4, SenzorLeft, CHANGE);
  attachInterrupt(5, SenzorRight, CHANGE);
  
  StartMotors();
    
  DDRA = B11111111; // set PORTA pins to output -> 22-29
  if(analogRead(ColourPin)<=800)
  {
    teamColour = 2;
    selectedSide = 0;
  }else
  {
    teamColour = 1;
    selectedSide = 1;
  }
  if(digitalRead(StrategyPin) == LOW)
  {
    selectedStrategy = 0;
  }else
  {
    selectedStrategy = 1;
  }
  strategyStep = 0;
  //selectedSide = 0;
  //selectedStrategy = 0;
  //teamColour = 2;
}*/

Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
 
int pos = 0;    // variable to store the servo position
int in1Pin = 12;
int in2Pin = 11;
int in3Pin = 10;
int in4Pin = 9;
 
//Stepper motor(512, in1Pin, in2Pin, in3Pin, in4Pin); 
 
int dir = 5;
int en = 6;
int stp = 7;
void setup()
{
//  StartMotors();
//  myservo.attach(8);  // attaches the servo on pin 9 to the servo object
//  for(int counter = 0;counter < numberOfMotors;counter++)
//  {
//    pinMode(motorEnablePins[counter], OUTPUT);
//    digitalWrite(motorEnablePins[counter], HIGH);
//  }
//  DDRA = B11111111; // set PORTA pins to output -> 22-29
  pinMode(dir, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(stp, OUTPUT);
  digitalWrite(dir, HIGH);
  digitalWrite(en, LOW);
  digitalWrite(stp, LOW);
//  digitalWrite(motorEnablePins[counter], HIGH);
//  digitalWrite(motorEnablePins[counter], HIGH);
//   myservo.write(10);
  // this line is for Leonardo's, it delays the serial interface
  // until the terminal window is opened
//  while (!Serial);
//  Serial.begin(9600);
//  motor.setSpeed(200);
Serial.begin(9600);
}

int delay2=90;
int delay2Slow=1000;
int stepcount=20000;
int trapezPercent=5;
float percent;

void loop()
{
//  StartMotors();
//  ArmMoveForward(100);
//  StopMotors();
delay(5000);
  digitalWrite(dir, HIGH);
  doStuff();
  myservo.write(150);
  delay(2000);
  digitalWrite(dir, LOW);
  doStuff();
  myservo.write(10);
//  StartMotors();
//  ArmMoveBackwards(100);
//  StopMotors();
  delay(2000);
}

void doStuff()
{
  digitalWrite(en, HIGH);
  for (int i=0; i<stepcount;i++)
  {
    percent = ((1.0*i)/stepcount) * 100;
    if (i%2==0)
    {
      digitalWrite(stp, LOW);
      
      delayMicroseconds(delay2);
    }
    else
    {
      digitalWrite(stp, HIGH);
      if (percent < trapezPercent)
      {
          delayMicroseconds((delay2 * percent + delay2Slow * (trapezPercent - percent)) / trapezPercent);
          //Serial.println((delay2 * percent + delay2Slow * (trapezPercent - percent)) / trapezPercent);
      }
      else
        delayMicroseconds(delay2);
    }
    
  }
  digitalWrite(en, LOW);
}

void StartMotors()
{
  for(int counter = 0;counter < numberOfMotors;counter++)
  {
    pinMode(motorEnablePins[counter], OUTPUT);
    digitalWrite(motorEnablePins[counter], HIGH);
  }
//  for(int counter = 0;counter<2;counter++)
//  {
//    pinMode(platformMotorPins[counter], OUTPUT);
//  }
//  platformMotor.setSpeed(20);
}

void StopMotors()
{
  for(int counter = 0;counter < numberOfMotors;counter++)
  {
    digitalWrite(motorEnablePins[counter], LOW);
    //pinMode(motorEnablePins[counter], INPUT);
  }
}

void TimeUp()
{
  Timer3.detachInterrupt();
}

void BlowCandle()
{
//  if(detectColor(taosOutPin) == teamColour)
//  {
//    ArmDrop();
//  }
}

//int detectColor(int taosOutPin){
//
//float white = colorRead(taosOutPin,0,1);
//float red = white/colorRead(taosOutPin,1,1);
//float blue = white/colorRead(taosOutPin,2,1);
//float green = white/colorRead(taosOutPin,3,1);
//
//if(red > blue && red > green)
//{
//  return 1; // red
//}
//
//if(blue > red && blue > green)
//{
//  return 2; // blue
//}
//
//if(green > blue && green > red)
//{
//  return 3; // green
//}
//
//}

/*
This section will return the pulseIn reading of the selected color.  
It will turn on the sensor at the start taosMode(1), and it will power off the sensor at the end taosMode(0)
color codes: 0=white, 1=red, 2=blue, 3=green
if LEDstate is 0, LED will be off. 1 and the LED will be on.
taosOutPin is the ouput pin of the TCS3200.
*/

//float colorRead(int taosOutPin, int color, boolean LEDstate){ 
//  
////turn on sensor and use highest frequency/sensitivity setting
//taosMode(1);
//
////setting for a delay to let the sensor sit for a moment before taking a reading.
//int sensorDelay = 100;
//
////set the S2 and S3 pins to select the color to be sensed 
//if(color == 0){//white
//digitalWrite(S3, LOW); //S3
//digitalWrite(S2, HIGH); //S2
//}
//
//else if(color == 1){//red
//digitalWrite(S3, LOW); //S3
//digitalWrite(S2, LOW); //S2
//}
//
//else if(color == 2){//blue
//digitalWrite(S3, HIGH); //S3
//digitalWrite(S2, LOW); //S2 
//}
//
//else if(color == 3){//green
//digitalWrite(S3, HIGH); //S3
//digitalWrite(S2, HIGH); //S2 
//}
//
//// create a var where the pulse reading from sensor will go
//float readPulse;
//
//// wait a bit for LEDs to actually turn on, as directed by sensorDelay var
//delay(sensorDelay);
//
//// now take a measurement from the sensor, timing a low pulse on the sensor's "out" pin
//readPulse = pulseIn(taosOutPin, LOW, 80000);
//
////if the pulseIn times out, it returns 0 and that throws off numbers. just cap it at 80k if it happens
//if(readPulse < .1){
//  readPulse = 80000;
//}
//
////turn off color sensor and LEDs to save power 
//taosMode(0);
//
//// return the pulse value back to whatever called for it... 
//return readPulse;
//}
//
//// Operation modes area, controlled by hi/lo settings on S0 and S1 pins.
////setting mode to zero will put taos into low power mode. taosMode(0);
//
//void taosMode(int mode){
//    
//    if(mode == 0){
//    //power OFF mode-  LED off and both channels "low"
//    //digitalWrite(LED, LOW);
//    digitalWrite(S0, LOW); //S0
//    digitalWrite(S1, LOW); //S1
//    //  Serial.println("mOFFm");
//    
//    }else if(mode == 1){
//    //this will put in 1:1, highest sensitivity
//    digitalWrite(S0, HIGH); //S0
//    digitalWrite(S1, HIGH); //S1
//    // Serial.println("m1:1m");
//    
//    }else if(mode == 2){
//    //this will put in 1:5
//    digitalWrite(S0, HIGH); //S0
//    digitalWrite(S1, LOW); //S1
//    //Serial.println("m1:5m");
//    
//    }else if(mode == 3){
//    //this will put in 1:50
//    digitalWrite(S0, LOW); //S0
//    digitalWrite(S1, HIGH); //S1 
//    //Serial.println("m1:50m");
//    }
//    return;
//}
//
//void TCS3200setup(){
//
//    //initialize pins
//    //pinMode(LED,OUTPUT); //LED pinD
//    
//    //color mode selection
//    pinMode(S2,OUTPUT); //S2 pinE
//    pinMode(S3,OUTPUT); //s3 pinF
//    
//    //color response pin (only actual input from taos)
//    pinMode(taosOutPin, INPUT); //taosOutPin pinC
//    
//    //communication freq (sensitivity) selection
//    pinMode(S0,OUTPUT); //S0 pinB
//    pinMode(S1,OUTPUT); //S1 pinA 
//    
//    return;
//}
//
//void ArmExtend()
//{
//    digitalWrite(ArmExtendPin, HIGH);
//}
//
//void ArmDrop()
//{
//  digitalWrite(ArmDropPin, HIGH);
//  delay(delayActions);
//  digitalWrite(ArmDropPin, LOW);
//}

void SenzorFront()
{
  frontDetected = !frontDetected;
}

void SenzorBack()
{
  backDetected = !backDetected;
}

void SenzorLeft()
{
  leftDetected = !leftDetected;
}

void SenzorRight()
{
  rightDetected = !rightDetected;
}

void EncoderLeft()
{
  stepsLeft++;
}

void EncoderRight()
{
  stepsRight++;
}

void MotionMotors()
{
  PORTA = motorSteps[motorCounters[0]][motorCounters[1]];
}

void MoveForward(float distance)
{
  int steps = distance * stepCmRatio;
  stepsLeft = 0;
  stepsRight = 0;
  float stepDelay = slowSpeedDelay;
  int deccelerationSteps = steps - accelerationSteps;
  for(int stepCounter = 1;stepCounter<=steps;stepCounter++)
  {
  while(digitalRead(FrontSenzorPin) == HIGH){
    delay(250);
  }
    MotionMotors();
    
    motorCounters[0]++;
    if(motorCounters[0] == 4){
      motorCounters[0] = 0;
    }
    
    motorCounters[1]++;
    if(motorCounters[1] == 4){
      motorCounters[1] = 0;
    }
    
    if(stepCounter<accelerationSteps)
    {
      stepDelay -= delayRate;
    }
    if(stepCounter>deccelerationSteps)
    {
      stepDelay += delayRate;
    }
    delay(stepDelay);
  }
//  while(stepsLeft!=steps)
//  {
//    MotionMotors();
//    motorCounters[0]++;
//    if(motorCounters[0] == 4){
//      motorCounters[0] = 0;
//    }
//    delay(slowSpeedDelay);
//  }
//  while(stepsRight!=steps)
//  {
//    MotionMotors();
//    motorCounters[1]++;
//    if(motorCounters[1] == 4){
//      motorCounters[1] = 0;
//    }
//    delay(slowSpeedDelay);
//  }
}

void MoveBackwards(float distance)
{
  int steps = distance * stepCmRatio;
  stepsLeft = 0;
  stepsRight = 0;
  float stepDelay = slowSpeedDelay;
  int deccelerationSteps = steps - accelerationSteps;
  
  for(int stepCounter = 1;stepCounter<=steps;stepCounter++)
  {
    while(digitalRead(BackSenzorPin) == HIGH)
    {
      delay(250);
    }
    MotionMotors();
    
    motorCounters[0]--;
    if(motorCounters[0] == -1)
    {
      motorCounters[0] = 3;
    }
    
    motorCounters[1]--;
    if(motorCounters[1] == -1)
    {
      motorCounters[1] = 3;
    }
    if(stepCounter<accelerationSteps)
    {
      stepDelay -= delayRate;
    }
    if(stepCounter>deccelerationSteps)
    {
      stepDelay += delayRate;
    }
    delay(stepDelay);
  }
//  while(stepsLeft!=steps)
//  {
//    MotionMotors();
//    motorCounters[0]--;
//    if(motorCounters[0] == -1)
//    {
//      motorCounters[0] = 3;
//    }
//  }
//  while(stepsRight!=steps)
//  {
//    MotionMotors();
//    motorCounters[1]--;
//    if(motorCounters[1] == -1)
//    {
//      motorCounters[1] = 3;
//    }
//  }
}

void ArmMoveForward(float distance)
{
  int steps = distance * stepCmRatio;
  stepsLeft = 0;
  stepsRight = 0;
  float stepDelay = armSlowSpeedDelay;
  int deccelerationSteps = steps - armAccelerationSteps;
  for(int stepCounter = 1;stepCounter<=steps;stepCounter++)
  {
  while(digitalRead(FrontSenzorPin) == HIGH){
    delay(250);
  }
    MotionMotors();
    
    motorCounters[0]++;
    if(motorCounters[0] == 4){
      motorCounters[0] = 0;
    }
    
    motorCounters[1]++;
    if(motorCounters[1] == 4){
      motorCounters[1] = 0;
    }
    
    if(stepCounter<armAccelerationSteps)
    {
      stepDelay -= armDelayRate;
    }
    if(stepCounter>deccelerationSteps)
    {
      stepDelay += armDelayRate;
    }
    delay(stepDelay);
  }
//  while(stepsLeft!=steps)
//  {
//    MotionMotors();
//    motorCounters[0]++;
//    if(motorCounters[0] == 4){
//      motorCounters[0] = 0;
//    }
//    delay(slowSpeedDelay);
//  }
//  while(stepsRight!=steps)
//  {
//    MotionMotors();
//    motorCounters[1]++;
//    if(motorCounters[1] == 4){
//      motorCounters[1] = 0;
//    }
//    delay(slowSpeedDelay);
//  }
}

void ArmMoveBackwards(float distance)
{
  int steps = distance * stepCmRatio;
  stepsLeft = 0;
  stepsRight = 0;
  float stepDelay = armSlowSpeedDelay;
  int deccelerationSteps = steps - armAccelerationSteps;
  
  for(int stepCounter = 1;stepCounter<=steps;stepCounter++)
  {
    while(digitalRead(BackSenzorPin) == HIGH)
    {
      delay(250);
    }
    MotionMotors();
    
    motorCounters[0]--;
    if(motorCounters[0] == -1)
    {
      motorCounters[0] = 3;
    }
    
    motorCounters[1]--;
    if(motorCounters[1] == -1)
    {
      motorCounters[1] = 3;
    }
    if(stepCounter<armAccelerationSteps)
    {
      stepDelay -= armDelayRate;
    }
    if(stepCounter>deccelerationSteps)
    {
      stepDelay += armDelayRate;
    }
    delay(stepDelay);
  }
//  while(stepsLeft!=steps)
//  {
//    MotionMotors();
//    motorCounters[0]--;
//    if(motorCounters[0] == -1)
//    {
//      motorCounters[0] = 3;
//    }
//  }
//  while(stepsRight!=steps)
//  {
//    MotionMotors();
//    motorCounters[1]--;
//    if(motorCounters[1] == -1)
//    {
//      motorCounters[1] = 3;
//    }
//  }
}


void TurnLeft(int degreeCount)
{
  int steps = degreeCount * degreeStepRatio;
  float stepDelay = slowSpeedDelay;
    int deccelerationSteps = steps - accelerationSteps;
  for(int stepCounter = 1;stepCounter<=steps;stepCounter++)
  {
    while(digitalRead(LeftSenzorPin) == HIGH)
    {
      delay(250);
    }
    MotionMotors();
    
    motorCounters[0]--;
    if(motorCounters[0] == -1)
    {
      motorCounters[0] = 3;
    }
    motorCounters[1]++;
    if(motorCounters[1] == 4){
      motorCounters[1] = 0;
    }
    if(stepCounter<accelerationSteps)
    {
      stepDelay -= delayRate;
    }
    if(stepCounter>deccelerationSteps)
    {
      stepDelay += delayRate;
    }
    delay(stepDelay);
  }
}

void TurnRight(int degreeCount)
{
  int steps = degreeCount * degreeStepRatio;
  float stepDelay = slowSpeedDelay;
    int deccelerationSteps = steps - accelerationSteps;
    
  for(int stepCounter = 1;stepCounter<=steps;stepCounter++)
  {
    while(digitalRead(RightSenzorPin) == HIGH)
    {
      delay(250);
    }
    MotionMotors();
    
    motorCounters[0]++;
    if(motorCounters[0] == 4){
      motorCounters[0] = 0;
    }
    motorCounters[1]--;
    if(motorCounters[1] == -1)
    {
      motorCounters[1] = 3;
    }
    if(stepCounter<accelerationSteps)
    {
      stepDelay -= delayRate;
    }
    if(stepCounter>deccelerationSteps)
    {
      stepDelay += delayRate;
    }
    delay(stepDelay);
  }
}

//void PlatformTurnLeft(int degreeCount)
//{
//  int steps = degreeCount * degreeStepRatio;
//  digitalWrite(RightSignalingLedsPin, HIGH);
//  digitalWrite(LeftSignalingLedsPin, HIGH);
//  platformMotor.step(-steps);
//  digitalWrite(RightSignalingLedsPin, LOW);
//  digitalWrite(LeftSignalingLedsPin, LOW);
//}
//
//void PlatformTurnRight(int degreeCount)
//{
//  int steps = degreeCount * degreeStepRatio;
//  digitalWrite(RightSignalingLedsPin, HIGH);
//  digitalWrite(LeftSignalingLedsPin, HIGH);
//  platformMotor.step(steps);
//  digitalWrite(RightSignalingLedsPin, LOW);
//  digitalWrite(LeftSignalingLedsPin, LOW);
//}

void StopEverything()
{
}
