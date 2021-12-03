#include <Servo.h>
String InBytes;
int BytesToInt;
int CurrentServo = 0;
int x = 0;
int toggle = 0;
Servo myservo;


void setup() {
  myservo.attach(9);
  Serial.begin(115200);
  Serial.setTimeout(1);
  pinMode(LED_BUILTIN, OUTPUT);
  //Serial.print("booting");
 

}

void loop() {
//   InBytes = Serial.readStringUntil('\n');
//    BytesToInt = InBytes.toInt();
//    Serial.write("Success!\n");
  
  
  if (Serial.available() > 0){
    x++;
    InBytes = Serial.readStringUntil('\n');
    BytesToInt = InBytes.toInt();
    myservo.write(BytesToInt);
    
//    if bytestoint < current servo
//      current servo ++

//    while (BytesToInt != CurrentServo){
//      if (BytesToInt < CurrentServo){
//        CurrentServo ++;
//        myservo.write(CurrentServo);
//      }
//      if (BytesToInt > CurrentServo){
//        CurrentServo --;
//        myservo.write(CurrentServo);
//      }
//    }
//    Serial.write("Success!\n");
    
    if (x > 5){
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      x = 0;
    }

  }
}
