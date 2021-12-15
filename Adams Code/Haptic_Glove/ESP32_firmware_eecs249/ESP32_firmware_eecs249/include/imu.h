/*
 * Remote Feelings: Adam Curtis, Aymeric Wang, Xinying Hu
 * 11/30/21
 * Prototype version. Author: Aymeric Wang
 */

# ifndef IMU_H
# define IMU_H

# ifndef IMU_INIT
# define IMU_INIT
# include "HardwareSerial.h"
# include <Adafruit_MPU6050.h>
# include <Adafruit_Sensor.h>
# endif

/** * * * * * * * * * * * * * * * * * * * *
 * @brief Sample code for setup() and loop()
 * 
 * @setup 
 * void setup() {
 *  Serial.begin(115200);
 *  while (!Serial);
 *  Serial.println("MPU6050 OLED demo");
 * 
 *  if (!mpu.begin()) {
 *      Serial.println("Sensor init failed");
 *      while (1)
 *      yield();
 *  }
 *  Serial.println("Found a MPU-6050 sensor");
 *  delay(500);
 *  }
 * 
 * @loop
 * void loop() {
 *  Accel();
 *  delay(100);
 *  }
* * * * * * * * * * * * * * * * * * * */

Adafruit_MPU6050 mpu;

void Accel() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    Serial.print("Accel ");
    Serial.print("X: ");
    Serial.print(a.acceleration.x, 5);
    Serial.print(" m/s^2, ");
    Serial.print("\tAccel ");
    Serial.print("Y: ");
    Serial.print(a.acceleration.y, 5);
    Serial.print(" m/s^2, ");
    Serial.print("\tAccel ");
    Serial.print("Z: ");
    Serial.print(a.acceleration.z, 5);
    Serial.print(" m/s^2\n");
}

void Gyro() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    Serial.print("Gyro ");
    Serial.print("X: ");
    Serial.print(g.gyro.x, 5);
    Serial.print(" rps, ");
    Serial.print("\tGyro ");
    Serial.print("Y: ");
    Serial.print(g.gyro.y, 5);
    Serial.print(" rps, ");
    Serial.print("\tGyro ");
    Serial.print("Z: ");
    Serial.print(g.gyro.z, 5);
    Serial.print(" rps\n");
}

# endif