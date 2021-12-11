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
bool calibrated = false;

#define MAXFORCE 3000

#define PLATFORM 1
#if PLATFORM!=1 & PLATFORM!=2
#error [ERROR] PLATFORM must be 1 or 2
#endif

/* * * * * * * *
 * 1 = Pincher
 * 2 = Glove
 * * * * * * * */


void robotControl(){
  /**a try-catch statement allows to deal with concurrent
   * access to UART. when that happens an error should be
   * sent but we disregard it. It's a bit dirty but it works!
   * */
  try {
    Pos_offset = 10 + scaleFactor(); //Get force from robot and scales it
    //followFingers();
    followFingersAverage();
    int average_position = averageFingerPos();
    Serial.println(String("Average position:\t")+String(average_position));
    send_control(average_position);
  }
  catch(...){}
}

void controller();

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

  if (PLATFORM == 2){
    controller();
    for(int i = 0; i < 5; i++){
      forceAverage[i] = update_moving_average_value(avg_force[i], analogRead(FFPins[i]));
    }
  }
  else{
    int position_recieved = force_message_reciever();
    if (position_recieved!=0) {
      int adc_command = MIN_PULSE_WIDTH;
      //double sum = ((position_recieved-1000)/1000);
      float alpha = (MAX_PULSE_WIDTH-MIN_PULSE_WIDTH)/1000;
      float sum = (position_recieved-1000);
      adc_command = adc_command + int(sum*alpha);
      Serial.println(String("Position:\t")+String(position_recieved));
      Serial.println(String("ADC command:\t")+String(adc_command));
      
      if (adc_command<(1250)){
        Servos[3].write(1250);
      }
      else if (adc_command>(2000)){
        Servos[3].write(2000);
      }
      else {
        Servos[3].write(adc_command);
      }
      
      forceAverage[0] = update_moving_average_value(avg_force[0], analogRead(FFPins[0]));
      long int local_force = forceAverage[0];
      send_control(local_force);
    }
  }
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
      calibrated = true;
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
      if (calibrated) {
        robotControl();
        lastState = state;
      }
      else {
        Serial.println("Calibration required to launch program");
        delay(1000);
        state = 'b';
      }
      //Serial.println(micros() - timer);
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