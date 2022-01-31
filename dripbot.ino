#include <Servo.h>
#include <NoDelay.h>

#define ON 1
#define OFF 0

// Button
int button[] = {13};
int buttonState[] = {OFF};

// Pumps
int motorPin1[] = {2, 7};
int motorPin2[] = {4, 8};
int motorEn[] = {5, 6};
int motorState[] = {OFF, OFF};

// Servo
Servo servo[2];
int servoPosition[] = {0, 0};

// Delays
void motorOneToggle();
void motorTwoToggle();
noDelay motorOneTime(300, motorOneToggle, false);
noDelay motorTwoTime(1000, motorTwoToggle, false);

void servoOneSweep();
void servoTwoSweep();
noDelay servoOneTime(550, servoOneSweep, false);
noDelay servoTwoTime(650, servoTwoSweep, false);

void buttonToggle();
noDelay buttonOneTime(100, buttonToggle, false);

void setup() {
  // Buttons
  pinMode(button[0], INPUT_PULLUP); 
  
  // Pumps
  pinMode(motorPin1[0], OUTPUT);
  pinMode(motorPin2[0], OUTPUT);
  pinMode(motorEn[0], OUTPUT);
  pinMode(motorPin1[1], OUTPUT);
  pinMode(motorPin2[1], OUTPUT);
  pinMode(motorEn[1], OUTPUT);

  // Servos
  servo[0].attach(11);
  servo[1].attach(3);

  randomSeed(analogRead(A0));
}

void loop() {

  // Activate with button
  if (digitalRead(button[0]) == LOW) {
    buttonOneTime.start();
  }

  // Update timing
  buttonOneTime.update();  
  motorOneTime.update();
  motorTwoTime.update();
  servoOneTime.update();
  servoTwoTime.update();
}

// ----- BUTTON HELPERS
void buttonToggle() {
  buttonState[0] ^= 1; // Toggle on/off

  if (buttonState[0]) {
    motorOneTime.start();
    motorTwoTime.start();
    servoOneTime.start();
    servoTwoTime.start();
    buttonOneTime.setdelay(50000);
  }
  else {
    turnMotor(0, OFF);
    turnMotor(1, OFF); 
    servoSweep(0, 0);
    servoSweep(1, 0);
    
    motorOneTime.stop();
    motorTwoTime.stop();
    servoOneTime.stop();
    servoTwoTime.stop();
    buttonOneTime.stop();
    buttonOneTime.setdelay(100);
  }
}


// ----- MOTOR HELPERS


void motorOneToggle() {
  motorState[0] ^= 1;
  setMotorSpeed(0, random(200, 255));
  turnMotor(0, motorState[0]);
}

void motorTwoToggle() {
  motorState[1] ^= 1;
  setMotorSpeed(1, random(200, 255));
  turnMotor(1, motorState[1]);
}

void turnMotor(int motor, int on) {
  int pin1 = on ? HIGH : LOW;
  int pin2 = LOW;
  
  digitalWrite(motorPin1[motor], pin1);
  digitalWrite(motorPin2[motor], pin2);
}

void setMotorSpeed(int motor, unsigned speed) {
  //Controlling speed (0 = off and 255 = max speed)
  analogWrite(motorEn[motor], speed);
}

// ----- SERVO HELPERS

void servoOneSweep() {
  servoPosition[0] = !servoPosition[0] ? 180 : 0; 
  servoSweep(0, servoPosition[0]);
}

void servoTwoSweep() {
  servoPosition[1] = !servoPosition[1] ? 180 : 0; 
  servoSweep(1, servoPosition[1]);
}

void servoSweep(int servoInstance, int position) {
  servo[servoInstance].write(position);
}
