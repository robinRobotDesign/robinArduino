float analogRaw[16];   // Range : 0..1024
int digitalRaw[54];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for (int i = 0; i < 54; i++) {
    pinMode(i, INPUT);
  }

  for (int i = 0; i < 16; i++) {
    digitalRaw[i] = digitalRead(i);
  }
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  pinMode(A13, INPUT);
  pinMode(A14, INPUT);
  pinMode(A15, INPUT);

  for (int i = 0; i < 16; i++) {
    analogRaw[i] = analogRead(i);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
/*
  for (int i = 0; i < 54; i++) {
    int dRead = digitalRead(i);
    if (digitalRaw[i] != dRead) {
      digitalRaw[i] = dRead;
      Serial.print("Pin ");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(dRead);
      Serial.print(" - ");
      Serial.print(dRead*5);
      Serial.println("V");
    }
  }*/

  //for (int i = 0; i < 16; i++) {
    float aRead = analogRead(A3);
    if (abs(analogRaw[3] - aRead) > 200.0) {
      analogRaw[3] = aRead;
      Serial.print("Pin A");
      Serial.print(3);
      Serial.print(": ");
      Serial.print(aRead);
      Serial.print(" - ");
      Serial.print((aRead*5.0)/1024);
      Serial.println("V");
    }
 // }
  
  delay(10);
}
