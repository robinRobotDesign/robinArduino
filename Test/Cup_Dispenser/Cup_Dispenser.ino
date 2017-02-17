#include <Servo.h>

Servo myservo;  // create servo object to control a servo

void setup() {
  myservo.attach(10);
  
  //Serial.begin(9600);
}

void loop() {
  //myservo.write(90);

  for (int i = 0; i < 3; i++) {
    //dispensing
    myservo.write(150);
    delay(1000);
    //delay(random(5000));
    
    //preparing
    myservo.write(80);
    delay(5000);
    //delay(random(5000));
  } 

  
}


