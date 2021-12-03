///////// library and parameter setting ///////////////////////
# include <Servo.h>

String InBytes;
int BytesToInt;
int CurrentPos;

int x= 0;
int toggle = 0;

Servo myservo;
///////////////////////////////////////////////////////////////


//////// interpolation function ///////////////////////////////
void Interpolate1 (int NextPos, float N){
  CurrentPos = myservo.read();
  for (float i = 1.0; i <= N; i++){
    myservo.write(Lerp(CurrentPos, NextPos, i/N));
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



void Interpolate2 (int NextPos, float pc = 0.5){
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
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0){
    
    InBytes = Serial.readStringUntil('\n');
    BytesToInt = InBytes.toInt();
    Interpolate1(BytesToInt, 10.0);

//    Interpolate2(BytesToInt);
//    Interpolate3(BytesToInt);

    // status indicator
    x++; 
    if (x > 5){
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      x = 0;
    }

  }
}
