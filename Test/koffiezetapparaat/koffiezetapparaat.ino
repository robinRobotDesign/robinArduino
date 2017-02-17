byte PWM_PIN=3;
int pwm_value;
void setup() {
  for(int i = 5;i<13;i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  pinMode(PWM_PIN, INPUT);
  Serial.begin(57600);
  // put your setup code here, to run once:
  
  digitalWrite(8, LOW);
  delay(300);
  digitalWrite(8, HIGH);
}

void loop() {
//  delay(1000);
  pwm_value=pulseIn(PWM_PIN, HIGH);
  Serial.println(pwm_value);
  // put your main code here, to run repeatedly:
  
}
