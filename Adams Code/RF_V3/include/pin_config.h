 /*
 * Remote Feelings: Adam Curtis, Aymeric Wang, Xinying Hu
 * 11/30/21
 * Prototype version. Author: Adam Curtis, Aymeric Wang
 */

#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

#define vibrator 33
#define vibrator2 25

//#define RXp2 16
//#define TXp2 17

#define LOGOLED 23

#define NEWUSER 1
#define ADAM 0
#if ((ADAM + NEWUSER) == 1)

#if NEWUSER == 1
#define PinkyEnc 4
#define RingEnc 26
#define MiddleEnc 27
#define IndexEnc 14
#define ThumbEnc 13

#define LOGOLED 23

#define PWM1_Ch    0
#define PWM1_Res   8
#define PWM1_Freq  20000
#define INA 2
#define INB 15

#define FF1 36
#define FF2 39
#define FF3 34
#define FF4 35
#define FF5 32

#define pinkyMotor1 17
#define pinkyMotor2 16
#define ringMotor1 22
#define ringMotor2 21
#define middleMotor1 19
#define middleMotor2 18
#define indexMotor1 25
#define indexMotor2 23
#define thumbMotor1 33
#define thumbMotor2 12
#endif

#if ADAM == 1
#define PinkyEnc 25
#define RingEnc 26
#define MiddleEnc 27
#define IndexEnc 14
#define ThumbEnc 4

#define LOGOLED 23

#define PWM1_Ch    0
#define PWM1_Res   8
#define PWM1_Freq  20000
#define INA 2
#define INB 15

#define FF1 36
#define FF2 39
#define FF3 34
#define FF4 35
#define FF5 32

#define pinkyMotor1 17
#define pinkyMotor2 16
#define ringMotor1 22
#define ringMotor2 21
#define middleMotor1 19
#define middleMotor2 18
#define indexMotor1 5
#define indexMotor2 23
#define thumbMotor1 15
#define thumbMotor2 12
#endif
 
const int motorPins[] = {pinkyMotor1, pinkyMotor2, ringMotor1, ringMotor2, // continue on next line
middleMotor1, middleMotor2, indexMotor1, indexMotor2, thumbMotor1, thumbMotor2};

const int FFPins[] = {FF1,FF2,FF3,FF4,FF5};
const int SPins[] = {pinkyMotor1,pinkyMotor2,ringMotor1,ringMotor2,middleMotor1};
const int EncPins[] = {PinkyEnc, RingEnc, MiddleEnc, IndexEnc, ThumbEnc};

#endif

#endif


// 