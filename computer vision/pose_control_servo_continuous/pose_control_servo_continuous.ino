#include <Servo.h>
String InBytes;
int BytesToInt;
int x = 0;
Servo myservo;


void setup() {
  myservo.attach(9);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  if (Serial.available() > 0){
    InBytes = Serial.readStringUntil('\n');
    BytesToInt = InBytes.toInt();

    digitalWrite(LED_BUILTIN, HIGH);
    myservo.write(BytesToInt);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.write("Success!");
//    if (BytesToInt == 1){
//      while (x != 180){
//        myservo.write(x);
//        x ++;
//      }
//      }else if (BytesToInt == 2){
//        while (x != 0){
//          myservo.write(x);
//          x --;
       }
      
    }

//  }


//}
