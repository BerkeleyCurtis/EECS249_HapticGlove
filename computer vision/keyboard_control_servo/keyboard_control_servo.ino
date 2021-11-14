String InBytes;
int BytesToInt;

void setup() {
 // put your setup code here, to run once:
 Serial.begin(115200);
 pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    InBytes = Serial.readStringUntil('\n');
    BytesToInt = InBytes.toInt();
    if (BytesToInt == 1){
      digitalWrite (LED_BUILTIN, HIGH);
      Serial.write("LED on");
    }
    else if (BytesToInt == 2){
      digitalWrite(LED_BUILTIN, LOW);
      Serial.write("LED off");
    }
    else{
      Serial.write("invalid input");
    }
  }

}
