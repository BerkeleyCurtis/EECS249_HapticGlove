#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

/* * * * * * * * * * * * * *
* Code testing IMU_6050
* 
* @env: PlatformIO
* @config: 
* @imports: Adafruit_MPU6050, Adafruit_Sensor
* 
* The content of this file is to be pasted 
* in the main.cpp of a PlatformIO project
* We used the Arduino IDE, not Espressif.
* * * * * * * * * * * * * */

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200); // Initialize serial with 115200 baudwidth
  Serial.println("MPU6050 OLED demo");

  if (!mpu.begin()) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  Serial.println("Found a MPU-6050 sensor");
  delay(500); // Pause

void loop() {
  sensors_event_t a, g, temp;   // initialize method for IMU sensor
  mpu.getEvent(&a, &g, &temp);  // affect measurements to variables 

  Serial.print("Accelerometer ");
  Serial.print("X: ");
  Serial.print(a.acceleration.x, 1);
  Serial.print(" m/s^2, ");
  Serial.print("Y: ");
  Serial.print(a.acceleration.y, 1);
  Serial.print(" m/s^2, ");
  Serial.print("Z: ");
  Serial.print(a.acceleration.z, 1);
  Serial.println(" m/s^2");

  Serial.print("Gyroscope ");
  Serial.print("X: ");
  Serial.print(g.gyro.x, 1);
  Serial.print(" rps, ");
  Serial.print("Y: ");
  Serial.print(g.gyro.y, 1);
  Serial.print(" rps, ");
  Serial.print("Z: ");
  Serial.print(g.gyro.z, 1);
  Serial.println(" rps");

  delay(100);
}