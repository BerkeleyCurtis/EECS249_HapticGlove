/*
 * Remote Feelings: Adam Curtis, Aymeric Wang, Xinying Hu
 * 11/30/21
 * Prototype version. Author: Adam Curtis
 */

#ifndef MOTION_FUNCS_H
#define MOTION_FUNCS_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "calibration.h"
#include "pin_config.h"
#include "ForceFuncs.h"
#include "reciever.h"

float forceAverage[numOfFingers];
int driveSpeed[numOfFingers];
// int driveSpeed[numOfFingers] = {1,1,1,1,1}; //upgrade to differnt drive speeds after finger calibration

float Kp = 1.7;
uint delay_time = 1000;

//-------------------------Motor Variables-------------------------
int dutyCycle = 0;
int dutyCycle1 = 0;
/* Setting PWM Properties */
const int PWMFreq = 10000; /* 10 KHz */
const int PWMChannel[] = {0,1,2,3,4,5,6,7,8,9};
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);


void setupMotors(void){

  for(int i = 0; i < (numOfFingers * 2); i++){
    ledcSetup(PWMChannel[i], PWMFreq, PWMResolution);
    ledcAttachPin(motorPins[i], PWMChannel[i]);
  }
}

void printFingerPositions(){
  for(int i = 0; i < numOfFingers; i++){
    Serial.print(Fingers[i]);
    Serial.println(analogRead(EncPins[i]));
  }
}

void readFingerPositions(){
  for(int i = 0; i < numOfFingers; i++){
    fingerPos[i] = analogRead(EncPins[i]);
  } 
}

void driveMotors(){
  int motorSpeed[numOfFingers];
  for(int i = 0; i < numOfFingers; i++){
    bool direction = 0;
    if(driveSpeed[i] < 0){ // set direction
      direction = 0;
      motorSpeed[i] = -1 * driveSpeed[i];
    } else{
      direction = 1;
      motorSpeed[i] = driveSpeed[i];
    }
    if(motorSpeed[i] > MAX_DUTY_CYCLE)
      motorSpeed[i] = MAX_DUTY_CYCLE;
    //Serial.println(driveSpeed[i]);
    if(direction){ // drive forward
      ledcWrite(PWMChannel[(2*i)], motorSpeed[i]);
      ledcWrite(PWMChannel[(2*i + 1)], 0);
    }else{ // drive in reverse
      ledcWrite(PWMChannel[(2*i)], 0);
      ledcWrite(PWMChannel[(2*i + 1)], motorSpeed[i]);
    }
  }
}

void testMotors(int delay_time){
  int max = 1000;
  for(int i = -max; i < max; i++){
    for(int j = 0; j < numOfFingers; j++){
      driveSpeed[j] = i;  
    }
    driveMotors();
    delay(delay_time);
  }
  for(int i = max; i > -max; i--){
    for(int j = 0; j < numOfFingers; j++){
      driveSpeed[j] = i;  
    }
    driveMotors();
    delay(delay_time);
  }
}

void stopMotors(){
  for(int i = 0; i < numOfFingers; i++)
    driveSpeed[i] = 0;
  driveMotors(); // zero speed
}

float smooth_force(int force){
  static int avg_force[10];
  long long longForce = 0;
  static unsigned int i = 0;
  avg_force[i] = force;
  for(int j = 0; j < 10; j++){
    longForce += avg_force[j];
  }
  float output = (float)longForce / 10.0;
  i++;
  if(i > 9)
    i = 0;
  return output;
}

void V2FollowandForce(){
  static float Kr = 8;
  float smoothed_force = smooth_force(robotForce);
  readForce();
  for(int i = 0; i < (numOfFingers); i++){
    if(i > 2){
      driveSpeed[i] =  (-Kp * (force[i] - restForce[i]) + Kr * smoothed_force);
    }
    else{
      driveSpeed[i] =  (-Kp * (force[i] - restForce[i]));
    }
  }
  driveMotors();
}

void followFingersV2(){ // proportional force control only. Could add full PID
  readForce();
  for(int i = 0; i < (numOfFingers); i++){
    driveSpeed[i] =  -Kp * (force[i] - restForce[i]); // Ki*driveSpeed[i] ?
  }
  driveMotors();
}

void checkDelay(){
    if(delay_time < 1) {
    delay_time = 0;
    Serial.println("Zero delay");
  }
}

#endif