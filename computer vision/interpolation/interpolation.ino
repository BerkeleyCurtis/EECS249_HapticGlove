///////// library and parameter setting ///////////////////////
# include <Servo.h>

String InBytes;
int BytesToInt = 40;
int CurrentPos = 40;
int oldValue = 40;
int current_value = 40;
int x= 0;
int toggle = 0;
int slowly = 40;

Servo myservo;
///////////////////////////////////////////////////////////////


//////// interpolation function ///////////////////////////////
void Interpolate1 (int NextPos, float N){
  NextPos = constrain(NextPos, 40, 120);
  CurrentPos = myservo.read();
  for (float i = 1.0; i <= N; i++){
    myservo.write(Lerp(CurrentPos, NextPos, EaseOut(i/N)));
    //delay(5);
  }
  }

int Lerp(int CurrentPos, int NextPos, float pc){
  int LerpReturn = CurrentPos + (NextPos - CurrentPos) * pc;
  return LerpReturn;
}

float EaseIn (float pc){
  return pc * pc;
}

float EaseOut (float pc){
  return (1 - (pc - 1) * (pc - 1));
}

float EaseInEaseOut (float pc){
  return Lerp(EaseIn(pc), EaseOut(pc), pc);
}



void Interpolate2 (int NextPos, float pc = 0.7){
  NextPos = constrain(NextPos, 40, 120);
  CurrentPos = myservo.read();
  int ServoInput = CurrentPos + (NextPos - CurrentPos) * pc;
  myservo.write(ServoInput);
}


void Interpolate3 (int NextPos){
  CurrentPos = myservo.read();
  while (CurrentPos != NextPos){
    if (CurrentPos < NextPos){
      CurrentPos++;
      myservo.write(CurrentPos);
    }
    if (CurrentPos > NextPos){
      CurrentPos--;
      myservo.write(CurrentPos);
    }
  }
}


///////////////////////////////////////////////////////////////


//////// program enter point /////////////////////////////////
void setup() {
  myservo.attach(9);
  Serial.begin(115200);
  Serial.setTimeout(1);
  myservo.write(40);
  delay(20);
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0){

    InBytes = Serial.readStringUntil('\n');
    BytesToInt = InBytes.toInt();
    //Interpolate1 (BytesToInt, float(BytesToInt));
    
    
    
//    Interpolate2(BytesToInt);
//    Interpolate3(BytesToInt);

    // status indicator
//    x++; 
//    if (x > 5){
//      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//      x = 0;
//    }

  }
  //BytesToInt = constrain(BytesToInt, 40, 120);
  if(BytesToInt > current_value){
      current_value ++; 
    }
  if(BytesToInt < current_value){
      current_value --; 
    }
  myservo.write(current_value);
  delay(slowly);
}
