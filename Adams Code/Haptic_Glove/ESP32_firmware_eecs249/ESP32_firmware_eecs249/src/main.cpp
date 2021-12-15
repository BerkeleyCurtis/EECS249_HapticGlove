/*
 * Remote Feelings: Adam Curtis, Aymeric Wang, Xinying Hu
 * 11/30/21
 * Prototype version. Author: Adam Curtis, Aymeric Wang
 */
//adam made a change
//#include <ESP32Servo.h>
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
int robotMin = 1800;

#define MAXFORCE 3000

/* * * * * * * * * * * * * * * * * * * *
 * THIS MUST BE REMOVED AFTER TEST
 * OF UART BETWEEN PINCHER AND GLOVE!!!
 * * * * * * * * * * * * * * * * * * * */

// #define PLATFORM 1
// #if PLATFORM!=1 & PLATFORM!=2
// #error [ERROR] PLATFORM must be 1 or 2
// #endif

/* * * * * * * *
 * 1 = Pincher
 * 2 = Glove
 * * * * * * * */

bool calibrated = false;

int averageFingerPos(void){
  long int Pos_total = 0;
  readFingerPositions();
  for (int i = 3; i < 5; i++){
     Pos_total += fingerPos[i];
  }
  int Pos_AVG = Pos_total / 2;
  //Serial.println(Pos_AVG);
  return Pos_AVG;
}

void robotControl(){
  //Gets force from robot and scales it
  robotForce = scaleFactor(); 
  //Serial.println(robotForce);
  //Track finger position
  V2FollowandForce();
  //applyRobotForce();  
  //scale value before sending        
  int corrected_value = robotMin - 0.8 * (2*averageFingerPos() - (pinch[3] + pinch[4])); 
  //controls robot gripper position
  send_control(corrected_value); 
  delayMicroseconds(delay_time); //tunable parameter to synchronize uart communication
}

void controller() { // state machine takes keyboard inputs 
  switch(state) {
    
    case 'a' : { // STOP!!
      stopMotors();
      Serial.println("Stopped");
      state = 'b';
      break;
    }
    case 'b' : { //Waiting 
      Serial.println("Waiting");
      printFingerPositions();
      printForce();
      delay(1000);
      lastState = state;
      break;
    }
    case 'c' : { // Calibrte Glove
      Serial.println("Calibrating Glove");
      //setupServos();
      calibration();
      //setupServos();
      delay(200);
      state = 'b';
      break;
    }
    // case 'd' : { // Calculate Range
    //   Serial.println("Calculating Force range");
    //   calcForceRange();
    //   calcOffsetScaler();
    //   state = 'b';
    //   break;
    // }
    case 'e' : { // Free following 
      
      V2FollowandForce();
      //Serial.println("Following");
      lastState = state;
      break;
    }
    case 'f' : { // Robot Control
      robotControl();
      lastState = state;
      break;
    }
    case 'p' : { // Increase Force
      Serial.print("increasing force to ");
      robotForce += 1;
      Kr += 0.01;
      //Neg_offset += 1;
      Serial.println(robotForce);
      state = lastState;;
      break;
    }
    case ';' : { // Decrease Force
      robotForce -= 1;
      Kr -= 0.01;
      //Neg_offset -= 1;
      if(Kp < 1)
        Kp = 0;
      if(robotForce < 1){
        robotForce = 0;
      }
      Serial.print("Decreasing force to ");
      Serial.println(robotForce);
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
    // case 'w' : { // Increasing speed
    //   Serial.print("Increasing speed to ");
    //   driveSpeed[0] += 10;
    //   Serial.println(driveSpeed[0]);
    //   state = lastState;
    //   break;
    // }
    // case 's' : { // Decreasing speed
    //   Serial.print("decreasing speed to ");
    //   driveSpeed[0] -= 10;
    //   if(driveSpeed[0] < 11){
    //     driveSpeed[0] = 0;
    //     Serial.println("Zero drive speed");
    //     if(direction){
    //       direction = 0;
    //     }else direction = 1;
    //   }
    //   Serial.println(driveSpeed[0]);
    //   state = lastState;
    //   break;
    // }
    default : { // Default Case
      Serial.println("Invalid state");
      state = 'a';
    }
  }
}

void setup() {
	Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1,RXp2,TXp2);
  setupMotors();
  //initializeMovingAvg();
  delay(1000);
}

void loop() {

  robotControl(); // 
  
  /************** Use for access to state machine controller *********
  if (Serial.available() > 0) {                                      
    state = Serial.read();                                  
  }
  controller();
  *******************************************************************/

  
}