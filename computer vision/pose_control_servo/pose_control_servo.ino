#include <Servo.h>

String InBytes;
int BytesToInt;
Servo myservo;
int x;

void setup() {
 // put your setup code here, to run once:
 Serial.begin(115200);
 pinMode(LED_BUILTIN, OUTPUT);
 myservo.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    InBytes = Serial.readStringUntil('\n');
    BytesToInt = InBytes.toInt();
    
    if (BytesToInt >= 100){
      digitalWrite (LED_BUILTIN, HIGH);
      Serial.write("LED on");
      
    for (x = 0; x <= 180; x += 1){
    myservo.write(x);
    delay(15);
    
    }
   }
    else if (BytesToInt < 60){
      digitalWrite(LED_BUILTIN, LOW);
      Serial.write("LED off");
      for (x = 180; x >= 0; x -= 1){
      myservo.write(x);
      delay(15);
    }
  }
  else{
    Serial.write("invalid input");
  }
  }
}