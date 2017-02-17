void setup() {
  
  // put your setup code here, to run once:
  pinMode(A10, OUTPUT);
  pinMode(A11, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(A11, HIGH);
  delay(300);
  digitalWrite(A11, LOW);
  delay(60000);
}


