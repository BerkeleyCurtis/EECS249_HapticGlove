#include <Arduino.h>
#include "calibration.h"
#include "pin_config.h"



void calibrateForceZero(void){
  for(int i = 0; i < numOfFingers; i++){
    restForce[i] = analogRead(FFPins[i]);
  }
}

void printForce(void){
  for(int i = 0; i < numOfFingers; i++){
    Serial.print(Fingers[i]);
    Serial.print(" force is\t\t");
    Serial.println(analogRead(FFPins[i]));
  }
}

void readForce(void){
  for(int i = 0; i < 5; i++){
    force[i] = analogRead(FFPins[i]);
  }
}

// void timeNewForce(){
//     long int timer = micros();
//     int temp = analogRead(FFPins[0]);
//     while(temp == analogRead(FFPins[0]));
//     timer = micros() - timer;
//     Serial.println(timer); 
// }

// void timing_readForce(void){ // 
//   for(int i = 0; i < 5; i++){
//     long int timer = micros();
//     int temp = analogRead(FFPins[i]);
//     timer = micros() - timer;
//     Serial.println(timer); 
//     if (temp == force[i]){
//         Serial.print("force wasn't updated for ");
//         Serial.println(Fingers[i]);
//     }
//     else force[i] = temp;
//   }
// }
