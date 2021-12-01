/*
 * Remote Feelings: Adam Curtis, Aymeric Wang, Xinying Hu
 * 11/29/21
 * Prototype version. Author: Adam Curtis
 */

#include <ESP32Servo.h>
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

#define FF1 36
#define FF2 39
#define FF3 34
#define FF4 35
#define FF5 32

#define S1 26
#define S2 27
#define S3 14
#define S4 12
#define S5 13

int force1;
int force2;
int force3;
int force4;
int force5;

int j = 1500;

void setup() {
	Serial.begin(115200);
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

  servo1.setPeriodHertz(50); 
  servo2.setPeriodHertz(50);
  servo3.setPeriodHertz(50);
  servo4.setPeriodHertz(50);
  servo5.setPeriodHertz(50);
  
  servo1.attach(S1, 1000, 2000); 
  servo2.attach(S2, 1000, 2000);
  servo3.attach(S3, 1000, 2000);
  servo4.attach(S4, 1000, 2000);
  servo5.attach(S5, 1000, 2000);
}

void loop() {
  
  force1 = analogRead(FF1);
  force2 = analogRead(FF2);
  force3 = analogRead(FF3);
  force4 = analogRead(FF4);
  force5 = analogRead(FF5);
  
//	if(force1 > 220){ 
//    j += ((force1 - 200)/20);
//	} else if (force1 < 180) {
//    j -= ((200 - force1)/20);
//	}

//  if(j>2000) j = 2000;
//  if(j<1000) j = 1000;
  Serial.println("F1");
	Serial.println(force1);
  Serial.println("F2");
  Serial.println(force2);
  Serial.println("F3");
  Serial.println(force3);
  Serial.println("F4");
  Serial.println(force4);
  Serial.println("F5");
  Serial.println(force5);
  //Serial.println(j);
	delay(200);

  servo1.write(1500);
  servo2.write(1500);
  servo3.write(1500);
  servo4.write(1500);
  servo5.write(1500);

}
