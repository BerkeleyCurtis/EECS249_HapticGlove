#include <Servo.h>
String InBytes;
int BytesToInt;
int x = 0;
int toggle = 0;
Servo myservo;


void setup() {
  myservo.attach(9);
  Serial.begin(115200);
  Serial.setTimeout(1);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  if (Serial.available() > 0){
    x++;
    InBytes = Serial.readStringUntil('\n');
    BytesToInt = InBytes.toInt();
    myservo.write(BytesToInt);
    
    
    if (x > 5){
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      x = 0;
    }

  }
}
