#include <Servo.h>

Servo myservo;  // create servo object to control a servo

void setup() {
  myservo.attach(3);
  
  Serial.begin(9600);
}

void loop() {
  
  for (int i =25 ; i < 110; i++) {
    //dispensing
    myservo.write(i);
    delay(25);
    //delay(random(5000));
  }
for (int i = 110; i > 25; i--) {
    //dispensing
    myservo.write(i);
    delay(25);
    //delay(random(5000));
  }


 
  
}


