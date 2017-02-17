#include <Servo.h>


#define CDHALL 12


//CupDispenser
Servo cdServo;


void setup() {
  Serial.begin(115200);
  cdServo.attach(10);
  pinMode(CDHALL, INPUT);
//  pinMode(13, OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
  Serial.print("Start Loop");
  Serial.println(digitalRead(CDHALL));
//  setSpeed(1);
//  positionFirstCup();
/*  if(digitalRead(CDHALL)==LOW) {
    dispenseCup();
  }*/
  positionFirstCup();
  delay(10);
}

void positionFirstCup() {
  while(digitalRead(CDHALL)!=LOW){
  }
  Serial.println("Train arrives");
  Serial.println(digitalRead(CDHALL));
//  stop();
//  long start = millis();
//  long finish = millis();
  //delay(5000);//Check if HIGH for 2 seconds
//  while(start-finish<2000&&digitalRead(CDHALL)!=LOW){
//    finish = millis();
//  }
  delay(2000);
  Serial.println(digitalRead(CDHALL));
  if(digitalRead(CDHALL)!=LOW){
    //locationCorrection();
    Serial.println("Train moved too much");
    positionFirstCup();
    return;
  }else {
    Serial.println("Cup dispensed");
    dispenseCup();
    return;
  }
}

void dispenseCup() {
      //dispensing
      cdServo.write(129);
      delay(1000);
      cdServo.write(0);
      delay(3000);
}
