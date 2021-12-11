#ifndef MOTION_FUNCS_H
#define MOTION_FUNCS_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "calibration.h"
#include "pin_config.h"
#include "ForceFuncs.h"
#include "MovingAvg.h"

MovingAverage* avg_force[5];
float forceAverage[5];

int motorTest = 0;
int driveSpeed = 1;

int Pos_offset = 100;
int Neg_offset = 100;
// int driveSpeed[numOfFingers] = {1,1,1,1,1}; //upgrade to differnt drive speeds after finger calibration
uint delay_time = 1000;
bool forward[5];

void setupServos(void){
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  for (int i = 0; i < 5; i++){
    Servos[i].setPeriodHertz(50);
    Servos[i].attach(SPins[i], MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  }
}

void disableServos(void){
  for (int i = 0; i<5; i++){
    Servos[i].detach();
  }
}

void checkDelay(){
    if(delay_time < 1) {
    delay_time = 0;
    Serial.println("Zero delay");
  }
}

void driveServos(){
  //checkDelay();
  delayMicroseconds(delay_time);
  for(int i = 0; i < numOfFingers; i++){
    if(servoPosition[i] > 2000) servoPosition[i] = 2000;
    if(servoPosition[i] < 1000) servoPosition[i] = 1000;
  }
  for(int i = 0; i < 2; i++){
    Servos[i].write(3000 - servoPosition[i]);
  }
  for(int i = 2; i < 5; i++){
    Servos[i].write(servoPosition[i]);
  }
}

// integral control to follow fingers
// void followFingersIntegral(void){
//   readForce();
//   for(int i = 0; i < 5; i++){
//     if(force[i] > (restForce[i] + Pos_offset)) {
//       if(forward[i]){
//           driveSpeed[i]++;
//       } else {
//           driveSpeed[i] = 0;
//       }
//       forward[i] = true;
//       sPos[i] += driveSpeed[i];
//     }
//     if(force[i] < (restForce[i] - Neg_offset)) {
//       if(!forward[i]){
//           driveSpeed[i]++;
//       } else {
//           driveSpeed[i] = 0;
//       }
//       forward[i] = false;
//       sPos[i] -= driveSpeed[i];
//     }
//   }
//   driveServos();
// }

void followFingers(void){ //Working but performance not amazing
  readForce();
  for(int i = 0; i < 5; i++){
    if(force[i] > (restForce[i] + Pos_offset)) {
      if (i==0 || i==1){
        servoPosition[i] += driveSpeed;
      }
      else {
        servoPosition[i] -= driveSpeed;
      }
    }
    if(force[i] < (restForce[i] - Neg_offset)) {
      if (i==0 || i==1){
        servoPosition[i] -= driveSpeed;
      }
      else {
        servoPosition[i] += driveSpeed;
      }
    }
  }
  driveServos();
}

// Proportional control to follow fingers
// void followFingers(void){
//   int maxSpeed = 5;
//   readForce();
//   for(int i = 0; i < 5; i++){
//     if(force[i] > (restForce[i] + Pos_offset * forceScaler[i])) {
//       int temp = sPos[i] + (force[i]- restForce[i])/Pos_offset;
//       if (temp > (sPos[i] + maxSpeed)) temp = sPos[i] + maxSpeed;
//       sPos[i] = temp;
//     }
//     if(force[i] < (restForce[i] - Neg_offset * forceScaler[i])) {
//       int temp = sPos[i] - (restForce[i] - force[i])/Neg_offset;
//       if (temp < (sPos[i] - maxSpeed)) temp = sPos[i] - maxSpeed;
//       sPos[i] = temp;
//     }


void followFingersAverage(void){
  //int maxSpeed = 5;
  //readForce();
  for(int i = 0; i < 5; i++){
    // if(forceAverage[i] > (restForce[i] + Pos_offset)) {
    //   int temp = sPos[i] + (forceAverage[i]- restForce[i])/Pos_offset;
    //   if (temp > (sPos[i] + driveSpeed)) temp = sPos[i] + driveSpeed;
    //   sPos[i] = temp;
    // }
    // if(forceAverage[i] < (restForce[i] - Neg_offset)) {
    //   int temp = sPos[i] - (restForce[i] - forceAverage[i])/Neg_offset;
    //   if (temp < (sPos[i] - driveSpeed)) temp = sPos[i] - driveSpeed;
    //   sPos[i] = temp;
    // }
    if(forceAverage[i] > (restForce[i] + Pos_offset)) {
      servoPosition[i] += driveSpeed;
    }
    if(forceAverage[i] < (restForce[i] - Neg_offset)) {
      servoPosition[i] -= driveSpeed;
    }

  }
  driveServos();
}


// void driveHackedServo(void){
 
// const int INB = 2;

// int dutyCycle;
// int dutyCycle1 = 0;
// /* Setting PWM Properties */
// const int PWMFreq = 10000; /* 10 KHz */
// const int PWMChannel0 = 0;
// const int PWMChannel1 = 1;
// const int PWMResolution = 10;
// const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);
// void setup()
// {  
//   ledcSetup(PWMChannel0, PWMFreq, PWMResolution);
//   ledcSetup(PWMChannel1, PWMFreq, PWMResolution);
//   /* Attach the LED PWM Channel to the GPIO Pin */
//   ledcAttachPin(LEDPin, PWMChannel0);
//   ledcAttachPin(INB, PWMChannel1);
  
// }
// void loop()
// {
//   if(dutyCycle1 < 1){
//     dutyCycle1 = MAX_DUTY_CYCLE;
//   } else dutyCycle1 = 0;
  
//   ledcWrite(PWMChannel1, dutyCycle1);
//   /* Increasing the LED brightness with PWM */
//   for(dutyCycle = 0; dutyCycle <= MAX_DUTY_CYCLE; dutyCycle++)
//   {
//     ledcWrite(PWMChannel0, dutyCycle);
//     delay(3);
//     //delayMicroseconds(100);
//   }
//       /* Decreasing the LED brightness with PWM */
//   for(dutyCycle = MAX_DUTY_CYCLE; dutyCycle >= 0; dutyCycle--)
//   {
//     ledcWrite(PWMChannel0, dutyCycle);
//     delay(3);
//     //delayMicroseconds(100);
//   }
// }
// }

void driveHackedServoDeprecated(void){

digitalWrite(INA, LOW);
  while(motorTest < 254)
  { 
    int AnalogValue = analogRead(FFPins[1]);
    Serial.print("The direction of output shaft is ");
    Serial.println(AnalogValue);
    Serial.print("The 8bit motor PWM signal is ");
    Serial.println(motorTest);
    analogWrite(INB, motorTest);
    motorTest ++ ;
    delay(20);
  }
  while(motorTest > 30)
  {
    int AnalogValue = analogRead(FFPins[1]);
    Serial.print("The direction of output shaft is ");
    Serial.println(AnalogValue);
    Serial.print("The 8bit motor PWM signal is ");
    Serial.println(motorTest);
    analogWrite(INB, motorTest);
    motorTest -- ;
    delay(20);
  }
}

//--------Function to close robot arm until it feels an object then open in a loop
// int crunch(void){
//   for(int i = 1300; i<2000; i++){
//     sPos5 = i;
//     readForce();
//     printForce();
//     if(force1 > 3000) {
//       return 1;
//     }
//     driveServos();
//     delay(2);
//   }
//   return 0;
// }

#endif