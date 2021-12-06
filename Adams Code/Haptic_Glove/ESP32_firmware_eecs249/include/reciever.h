/*
 * Remote Feelings: Adam Curtis, Aymeric Wang, Xinying Hu
 * 11/30/21
 * Prototype version. Author: Aymeric Wang
 */

#include <Arduino.h>
#include <Wire.h>
#include <string.h>

<<<<<<< HEAD

int force_message_reciever(){
    //Serial.println("Listening to force messages...");
=======
#define MAX_TRANSM_DELAY 2000

/** * * * * * * * * * * * * * * * * * *
 * @brief Sample code for setup() and loop()
 * 
 * @setup 
 * void setup() {
 *  Serial.begin(115200);
 *  Serial2.begin(115200, SERIAL_8N1,RXp2,TXp2);
 * }
 * 
 * @loop
 * void loop() {
 *  int may_the_force_bwu;
 *  may_the_force_bwu = force_message_reciever();
 *  delay(500);
 * * * * * * * * * * * * * * * * * * * * */


int force_message_reciever(){
    Serial.println("Listening to force messages...");
>>>>>>> a3909edc40921542d464333fe33da864d1b510da
    char force_message[10];
    char character;
    char msg_delimiter_init[] = "<";
    char msg_delimiter_end[] = ">";
    int i = 0;
<<<<<<< HEAD
    int robotForce = 0;
    //int time;
    //int timer_init = millis();
    // while (!Serial2.available()){
    //     Serial.println("Waiting for signal...");
    //     time = millis();
    //     if ((time-timer_init)>MAX_TRANSM_DELAY){
    //         Serial.println("No signal recieved... Check connectivity");
    //         return 0;
    //     }
    //     delay(250);
    // }

//---- Consider using while(Serial2.read() != msg_delimiter_end) ??? --------

=======
    int sPos5 = 0;
    int time;
    int timer_init = millis();
    while (!Serial2.available()){
        Serial.println("Waiting for signal...");
        time = millis();
        if ((time-timer_init)>MAX_TRANSM_DELAY){
            Serial.println("No signal recieved... Check connectivity");
            return 0;
        }
        delay(250);
    }
>>>>>>> a3909edc40921542d464333fe33da864d1b510da
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
    }
<<<<<<< HEAD
    robotForce = atoi(force_message);
    //Serial.println(String("Force recieved:\t")+String(robotForce));
    return(robotForce);
}

int scaleFactor(void){
    int scale = 20;
    return (force_message_reciever() / scale);
=======
    sPos5 = atoi(force_message);
    Serial.println(String("Force recieved:\t")+String(sPos5));
    return(sPos5);
>>>>>>> a3909edc40921542d464333fe33da864d1b510da
}



//-------Deprecated Use force_message_reciever();------------------------------
// void receive_command(){
//   char force[10];
//   int i = 0;

//   while (!Serial2.available());
//   while (Serial2.available()>0){
//     char string = Serial2.read();
//     //Serial.print(string);
//     force[i] = string;
//     i++;
//   }

//   sPos5 = atoi(force);
//   Serial.println(sPos5);
//   //delay(200);
//   driveServos();
// }