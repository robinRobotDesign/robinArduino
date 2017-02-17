/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int forward =138;//144; Difference = 101

int backward =37;// 43 ;

void setup() {
  Serial.begin(9600);
  myservo.attach(3);  // attaches the servo on pin 9 to the servo object
}

void loop() {/*
  long starttime = millis();

  while(millis()-starttime < 10000){
    Serial.println(digitalRead(2));
    delay(500);
  }*/
  
  for (pos = forward; pos >= backward; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    //Serial.println(pos);
    delay(5);                       // waits 15ms for the servo to reach the position
  }
  delay(100);
  for (pos = backward; pos <= forward; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    //Serial.println(pos);
    delay(5);                       // waits 15ms for the servo to reach the position
  }
  delay(1500);
}

