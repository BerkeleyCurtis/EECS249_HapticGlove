/*
 * Remote Feelings: Adam Curtis, Aymeric Wang, Xinying Hu
 * 11/30/21
 * Prototype version. Author: Aymeric Wang. Edits and additions by Adam Curtis
 */

#include <Arduino.h>
#include <Wire.h>
#include <string.h>

int robotForce = 0;

// Only receives one force value. This should be updated to have unique values for each finger. 
void force_message_reciever(){
    //Serial.println("Listening to force messages...");
    
    if(Serial.available()){
        char force_message[3];
        int i = 0;
        for(; i < 3; i++){
            force_message[i] = Serial.read();
        }
        // force_message[i] = '\n'; //not needed. Shrink force_message array size?
        robotForce = atoi(force_message);
        // Clear the UART buffer
        while(Serial.available()){
            Serial.read();
        }
    }
    // Serial.println(robotForce);
}

// deprecated. Not sure how well this works. 
int force_message_reciever_full_fingers(){
    //Serial.println("Listening to force messages...");
    char force_message[10];
    char character;
    char msg_delimiter_init[] = "<";
    char msg_delimiter_end[] = ">";
    int i = 0;
    static int robotForce = 0;
    static int robotForceTemp = 0;

    while (Serial2.available()>0 && i<10){
        character = Serial2.read();
        if (character == msg_delimiter_init[0]){
            i = 0;
        }
        else if (character == msg_delimiter_end[0]){
            i = 0;
            break;
        }
        else{
            force_message[i] = character;
            i++;
        }
        while(!Serial2.available()); //wait until there is more serial data available. 
        //Without this, sometimes the processor clears the UART buffer before all contents have arrived. 
    }
    robotForceTemp = atoi(force_message);
    if (robotForceTemp == 0){
        robotForce--;
        if(robotForce<0)
            robotForce = 0;
    }
    else
        robotForce = robotForceTemp;
    //Serial.println(robotForce);
    return(robotForce);
}