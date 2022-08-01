/**
 * Remote Feelings: Adam Curtis, Aymeric Wang, Xinying Hu
 * 11/30/21
 * Prototype version. Author: Aymeric Wang with edits and additions by Adam Curtis
 */

#ifndef CALIBRATION_H
#define CALIBRATION_H


#include <ESP32Servo.h>
#include <Arduino.h>
#include <iostream>
#include <list>
#include "pin_config.h"
#include "adc_to_newtons.h"

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
#define numOfFingers 5
int force[numOfFingers];
String Fingers[] = {"little", "ring", "middle", "index", "thumb"};
long int clenchForce[numOfFingers];
int forceRange[numOfFingers];
float forceScaler[numOfFingers];
int pinch[numOfFingers] = {0,1114,1506,434,1262}; // where the finger is now
int fingerPos[numOfFingers];

#if ADAM // Original glove
int fingerPosMin[numOfFingers] = {400,650,2200,1300,1600}; // hand all the way closed
int fingerPosMax[numOfFingers] = {1200,1500,1000,275,550}; // hand all the way open 
long int restForce[numOfFingers] = {1860,1720,1400,1840,2100}; // preload force on the force sensor
#endif

#if NEWUSER // Your glove
/* --------------------------- manual calibration --------------------------- */
// Easiest to start before you put the fingers onto the glove motors.
// Follow steps in main.cpp under manual calibration function. It was too hard put it here. 
long int restForce[numOfFingers] = {1860,1720,1400,1840,2100}; // preload force on the force sensor
int fingerPosMin[numOfFingers] = {0,644,2131,1338,1520}; // hand all the way closed
int fingerPosMax[numOfFingers] = {0,1571,1008,234,1169}; // hand all the way open 
#endif

// To use for calibration (Not working 7/31/22)
void calibration(){
    
    int samples = 50;
    Serial.println("Open your hand all the way please :)");
    delay(3000);
    
    for(int whichFinger = 0; whichFinger < numOfFingers; whichFinger++){
        for (int i=0; i < samples; i++){
        
            int force = analogRead(FFPins[whichFinger]);
            restForce[whichFinger] += force;
            delay(5);
        }
        fingerPosMin[whichFinger] = analogRead(EncPins[whichFinger]);
        Serial.println(fingerPosMin[whichFinger]);
        restForce[whichFinger] = restForce[whichFinger]/samples;
        Serial.println(restForce[whichFinger]);
    }
    Serial.println("Close your fist");
    delay(3000);

    for(int whichFinger = 0; whichFinger < numOfFingers; whichFinger++){
        fingerPosMax[whichFinger] = analogRead(EncPins[whichFinger]);
        Serial.println(fingerPosMax[whichFinger]);
        // for (int i=0; i < samples; i++){
            
        //     int force = analogRead(FFPins[whichFinger]);
        //     clenchForce[whichFinger] += force;
        //     //Serial.println(String("ADC of ")+String(Fingers[a])+String(":\t")+String(force));
        //     delay(5);
        // }
        // clenchForce[whichFinger] = clenchForce[whichFinger]/samples;
        // Serial.println(clenchForce[whichFinger]);
    }
    Serial.println("Pinch index and thumb");
    delay(3000);

    for(int whichFinger = 0; whichFinger < numOfFingers; whichFinger++){
        pinch[whichFinger] = analogRead(EncPins[whichFinger]);
        Serial.println(pinch[whichFinger]);
    }
}


void calcForceRange(){
    
    int minimumRange = 50; // Tuner variable
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