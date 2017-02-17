byte PWM_PIN = 2;
 
int pwm_value;
 
void setup() {
  pinMode(PWM_PIN, INPUT);
  Serial.begin(9600);
}
 
void loop() {
  float pwm_value = digtalRead(PWM_PIN, HIGH);
  Serial.print(pwm_value);
  Serial.print(" - ");
  Serial.print(pwm_value/1000);
  Serial.println("ms");
  
  //delay(10);
}

