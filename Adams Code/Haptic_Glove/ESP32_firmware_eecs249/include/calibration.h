/*
 * Remote Feelings: Adam Curtis, Aymeric Wang, Xinying Hu
 * 11/30/21
 * Prototype version. Author: Aymeric Wang
 */

#ifndef CALIBRATION_H
#define CALIBRATION_H


#include <ESP32Servo.h>
#include <Arduino.h>
#include <iostream>
#include <list>
#include "pin_config.h"
#include "adc_to_newtons.h"

#define RESISTIVE_FORCE_THRESHOLD 4
#define DANGER_FORCE_THRESHOLD 8
#define numOfFingers 5

/* * * * * * * * * * * * * * * * * * * * * *
 * We define a series of global variables
 * identified from 1 to 5. They each
 * correspond to a servo motor on the
 * glove. In this order:
 *      1. is the little finger
 *      2. is the ring finger
 *      ... and so on ...
 *      5. is the thumb
 * * * * * * * * * * * * * * * * * * * * * */

# define medium (int(MAX_PULSE_WIDTH+MIN_PULSE_WIDTH)/2)
# define samples 50

void setupServos();
void driveServos();

int servoPosition[] = {1000, 1000, 1000, 1000, 1000};
int force[numOfFingers];
String Fingers[] = {"little", "ring", "middle", "index", "thumb"};
Servo Servos[numOfFingers];
long int restForce[numOfFingers];
long int clenchForce[numOfFingers];
int forceRange[numOfFingers];
float forceScaler[numOfFingers];
//----------Pins---------------
int FFPins[] = {FF1,FF2,FF3,FF4,FF5};
int SPins[] = {S1,S2,S3,S4,S5};
//-----------Incrementer-----------
//int calibrationTracker = 0;

//---------Tuners ------------
int minimumRange = 50;
float scalerTuner = 200;


void calibration(){
 
    for(int calibrationTracker = 0; calibrationTracker < numOfFingers; calibrationTracker++){
        Serial.println("Relax your hand...");
        delay(1500);

        for (int i=0; i < samples; i++){
        
            delay(2);
            int force = analogRead(FFPins[calibrationTracker]);
            restForce[calibrationTracker] += force;
            //Serial.println(String("ADC of ")+String(Fingers[calibrationTracker])+String(":\t")+String(force));
            delay(50);
        }
        restForce[calibrationTracker] = restForce[calibrationTracker]/samples;
        Serial.println(restForce[calibrationTracker]);
    
        Servos[calibrationTracker].write(medium);
        Serial.println("GENTLY clench...");
        delay(1000);

        for (int i=0; i < samples; i++){
            
            int force = analogRead(FFPins[calibrationTracker]);
            clenchForce[calibrationTracker] += force;
            //Serial.println(String("ADC of ")+String(Fingers[a])+String(":\t")+String(force));
            delay(50);
        }
        clenchForce[calibrationTracker] = clenchForce[calibrationTracker]/samples;
        Serial.println(clenchForce[calibrationTracker]);
        
        Servos[calibrationTracker].detach();
        setupServos();
        //calibrationTracker++;
    }
}



// void calibration(){
//     Serial.println("Loose your hand...");
//     delay(1500);

//     for(int i = 0; i < numOfFingers; i++){
//         sPos[i] = medium;
//     }
//     driveServos();

//     for (int i=0; i < samples; i++){
//         for(unsigned int a = 0; a<numOfFingers; a++){
//             delay(2);
//             int force = analogRead(FFPins[a]);
//             restForce[a] += force;
//             Serial.println(String("ADC of ")+String(Fingers[a])+String(":\t")+String(force));
//         }
//         delay(50);
//     }
//     for(unsigned int a = 0; a < numOfFingers; a++){
//         restForce[a] = restForce[a]/samples;
//         Serial.println(restForce[a]);
//     }
    
//     Serial.println("GENTLY clench...");
//     delay(1000);

//     for (int i=0; i < samples; i++){
//         for(unsigned int a = 0; a < numOfFingers; a++){
//             delay(2);
//             int force = analogRead(FFPins[a]);
//             clenchForce[a] += force;
//             Serial.println(String("ADC of ")+String(Fingers[a])+String(":\t")+String(force));
//         }
//         delay(100);
//     }
//     for(unsigned int a = 0; a < numOfFingers; a++){
//         clenchForce[a] = clenchForce[a]/samples;
//         Serial.println(clenchForce[a]);
//     }

// }

void calcForceRange(){
    for (int i = 0; i < numOfFingers; i++){
        forceRange[i] = clenchForce[i] - restForce[i];
        if(forceRange[i] < minimumRange){
            Serial.print("Force range is too small on ");
            Serial.println(Fingers[i]);
            if(forceRange[i] < 0) Serial.println("It's less than zero...");
            forceRange[i] = minimumRange;
        }
    }
}

void calcOffsetScaler(){
    for(int i = 0; i < numOfFingers; i++){
        forceScaler[i] = float(forceRange[i]) / scalerTuner;
        Serial.print("Force Scaler for ");
        Serial.print(Fingers[i]);
        Serial.print(" is ");
        Serial.println(forceScaler[i]);
    }
}


bool confirmation(bool calibrated){
    label1:
    delay(200);
    Serial.println("Please attach the glove to your hand. Are you done? (y/n)");
    while (Serial.available()==0){}
    delay(200);
    if (Serial.read()!='y'){
        Serial.println("Take the time you need! Are your done? (y/n)");
        goto label1;
    }
    else {
        if (!calibrated) {
            Serial.println("Initiating calibration...");
            calibrated = true;
            return(calibrated);
        }
        else {
            bool hesitation = true;
            while (hesitation) {
                delay(200);
                Serial.println("Calibration stored in memory. Would you like to recalibrate? (y/n)");
                while (Serial.available()==0){}
                delay(200);
                if (Serial.read()!='y'){
                    Serial.println("Skipping calibration...");
                    return(calibrated);
                }
                else{
                    Serial.println("Initiating calibration...");
                    return(calibrated);
                }
            }
        }
    }
    calibrated = false;
    Serial.println("An error occured in the confirmation function...");
    return(calibrated);
}


#endif