/*
 * Remote Feelings: Adam Curtis, Aymeric Wang, Xinying Hu
 * 11/30/21
 * Prototype version. Author: Adam Curtis, Aymeric Wang
 */
//adam made a change
#include <ESP32Servo.h>
#include <Arduino.h>
#include "calibration.h"
#include "pin_config.h"
#include "sender.h"
#include "reciever.h"
#include "MotionFuncs.h"
//#include "adc.h"

int state = 'b'; //waiting
int lastState = 'b'; //waiting
int listSize = 100;

#define MAXFORCE 3000

/* * * * * * * * * * * * * * * * * * * *
 * THIS MUST BE REMOVED AFTER TEST
 * OF UART BETWEEN PINCHER AND GLOVE!!!
 * * * * * * * * * * * * * * * * * * * */

#define PLATFORM 1
#if PLATFORM!=1 & PLATFORM!=2
#error [ERROR] PLATFORM must be 1 or 2
#endif

/* * * * * * * *
 * 1 = Pincher
 * 2 = Glove
 * * * * * * * */

bool calibrated = false;

void robotControl(){
  Pos_offset = 5 + scaleFactor(); //Get force from robot and scales it
  //followFingers();
  followFingersAverage();
  send_control(averageFingerPos());
}

void controller() {
  switch(state) {
    
    case 'a' : { // STOP!!
      disableServos();
      state = 'b';
      Serial.println("Stopped");
      break;
    }
    case 'b' : { //Waiting 
      setupServos();
      Serial.println("Waiting");
      delay(1000);
      lastState = state;
      break;
    }
    case 'c' : { // Calibrate Glove
      Serial.println("Calibrating Glove");
      setupServos();
      calibration();
      setupServos();
      delay(200);
      state = 'b';
      break;
    }
    case 'd' : { // Calculate Range
      Serial.println("Calculating Force range");
      calcForceRange();
      calcOffsetScaler();
      state = 'b';
      break;
    }
    case 'e' : { // Free following 
      followFingers();
      //Serial.println("Following");
      lastState = state;
      break;
    }
    case 'f' : { // Robot Control
     // Serial.println("controlling Robot");
      //int timer = micros();
      robotControl();
      //Serial.println(micros() - timer);
      lastState = state;
      break;
    }
    case 'p' : { // Increase Force
      Serial.print("increasing force to ");
      Pos_offset += 1;
      Neg_offset += 1;
      Serial.println(Pos_offset);
      state = 'f';
      break;
    }
    case ';' : { // Decrease Force
      Serial.print("Decreasing force to ");
      Pos_offset -= 1;
      Neg_offset -= 1;
      Serial.println(Pos_offset);
      state = lastState;
      break;
    }
    case 'i' : { // Increase Time Delay
      delay_time += 10;
      Serial.print("increasing time delay to ");
      Serial.print(delay_time);
      Serial.println(" microseconds");
      state = lastState;;
      break;
    }  
    case 'o' : { // Decrease Time Delay
      delay_time -= 10;
      if(delay_time < 11){
        delay_time = 10;
        Serial.println("Time delay is ~ Zero");
      }
      else {
        Serial.print("decreasing time delay to ");
        Serial.print(delay_time);
        Serial.println(" microseconds");
      }
      state = lastState;;
      break;
    } 
    case 'w' : { // Increasing speed
      Serial.print("Increasing speed to ");
      driveSpeed += 1;
      Serial.println(driveSpeed);
      state = lastState;
      break;
    }
    case 's' : { // Decreasing speed
      Serial.print("decreasing speed to ");
      driveSpeed -= 1;
      if(driveSpeed < 1){
        driveSpeed = 0;
        Serial.println("Zero drive speed");
      }
      Serial.println(driveSpeed);
      state = lastState;
      break;
    }
    default : { // Default Case
      Serial.println("Invalid state");
      state = 'a';
    }
  }
}

void setup() {
	Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1,RXp2,TXp2);
  
  for(int i = 0; i < 5; i++){
    avg_force[i] = construct_moving_average(listSize);
  }
  for(int j = 0; j < listSize; j++){
    for(int i = 0; i < 5; i++){
      forceAverage[i] = update_moving_average_value(avg_force[i], analogRead(FFPins[i]));
    }
  }

  //---
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(LOGOLED, OUTPUT);

  setupServos();
  delay(200);
  calibrateForceZero();
  delay(1000);
}

void loop() {


  if (Serial.available() > 0) {
    state = Serial.read();
  }
  controller();

  for(int i = 0; i < 5; i++){
    forceAverage[i] = update_moving_average_value(avg_force[i], analogRead(FFPins[i]));
  }
  // Serial.println(forceAverage[0]);

  // Serial.print("Averaged ");
  // Serial.println(forceAverage[2]);
  // Serial.println("");
  // Serial.print("NOT ");
  // Serial.println(analogRead(FFPins[2]));
  // delay(100);

  // followFingers();
  // driveServos();
  
}