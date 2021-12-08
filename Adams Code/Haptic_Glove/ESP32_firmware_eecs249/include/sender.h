/*
 * Remote Feelings: Adam Curtis, Aymeric Wang, Xinying Hu
 * 11/30/21
 * Prototype version. Author: Aymeric Wang
 */

#ifndef SENDER_H
#define SENDER_H

#include <Arduino.h>
#include <Wire.h>

/** * * * * * * * * * * * * * * * * *
 * @brief Sample code for setup() and loop()
 * 
 * @setup
 * void setup() {
 *  Serial.begin(115200);
 * }
 * 
 * @loop
 * void loop() {
 *  long int local_force;
 *  force_send(local_force);
 *  delay(500);
 * * * * * * * * * * * * * * * * * * * * */


void message_send(String message){
  String formatted_message;
  formatted_message ="<"+message+">";
  Serial2.print(formatted_message);
  Serial.println(formatted_message);
  //Serial.println("I sent message:\t"+String(formatted_message));
}

void send_control(int gripper_control) {
  String grip_cont_string = String(gripper_control);
  message_send(grip_cont_string);
}

int averageFingerPos(void){
  long int Pos_total = 0;
  for (int i = 0; i < 5; i++){
    Pos_total += servoPosition[i];
  }
  int Pos_AVG = Pos_total / 5;
  return Pos_AVG;
}

#endif