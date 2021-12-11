/*
 * Remote Feelings: Adam Curtis, Aymeric Wang, Xinying Hu
 * 11/30/21
 * Prototype version. Author: Aymeric Wang
 */

#ifndef ADC_TO_NEWTS_H
#define ADC_TO_NEWTS_H

#include <Arduino.h>
#include <math.h>

#define FORCE_SENSOR_PIN 36
#define RESISTANCE 5000.00
#define VOLTAGE_INPUT_ESP32 3.0
#define ESP32_ADC_RANGE 4095

#define COEFF_FORCE_SENSOR_LAW 93.199
#define EXP_FORCE_SENSOR_LAW 1.277

/** * * * * * * * * * * * * * * * * * *
 * @brief Sample code for setup() and loop()
 * 
 * @setup 
 * void setup() {
 *   Serial.begin(115200);
 * }
 * 
 * @loop
 * void loop(){
 *  uint16_t analogReading = analogRead(FORCE_SENSOR_PIN);
 *  float resistance = bridgeDivider(RESISTANCE,analogReading);
 *  Serial.print("ADC:\t");
 *  Serial.print(analogReading);
 *  Serial.print("\t\tResistance:\t");
 *  Serial.println(resistance);
 *  delay(500);
 * }
 * * * * * * * * * * * * * * * * * * * * */


float voltageDivider(float resistance, uint16_t ADC_measurement){
  float voltage_measurement = (float) ADC_measurement/ESP32_ADC_RANGE*VOLTAGE_INPUT_ESP32;
  return voltage_measurement*resistance/(VOLTAGE_INPUT_ESP32-voltage_measurement);
}

float ohmsToNewts(float resistance, uint16_t ADC_measurement){
  float force_resistance = voltageDivider(resistance, ADC_measurement);
  return(COEFF_FORCE_SENSOR_LAW*pow(force_resistance,EXP_FORCE_SENSOR_LAW-3));
}

#endif