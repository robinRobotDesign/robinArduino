#include <Servo.h>

#define MOTOR1 7
#define MOTOR2 8
#define MOTOR3 9
#define CDHALL 2

int currentSpeed = 240;

//CupDispenser
//Servo cdServo;


void setup() {
  Serial.begin(115200);
//  cdServo.attach(3);
  pinMode(CDHALL, INPUT);
  pinMode(MOTOR1, OUTPUT);
  digitalWrite(MOTOR1, HIGH);
  pinMode(MOTOR2, OUTPUT);
  digitalWrite(MOTOR2, LOW);
  pinMode(MOTOR3, OUTPUT);
//  pinMode(13, OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
  analogWrite(MOTOR3, 250);
  positionFirstCup();
 /* while(1){
  }*/
}
void whileLoop(int hall, int timer, bool value){
  int counter = 0;
  long start = millis();
  while(counter<3){
    if(digitalRead(hall)==value){
      counter++;
      if(counter>0){
        Serial.print("counter: ");
        Serial.println(counter);
      }
    } else {
      counter=0;
    }
    delay(10);
    if(timer>0&&timer<millis()-start){
      break;
    }
  }
}

void positionFirstCup() {
  whileLoop(CDHALL, -1, LOW);
  
  int a = digitalRead(CDHALL);
  Serial.println(digitalRead(CDHALL));
  if(!a){
  Serial.println("Train arrives");/*
  Serial.println(a);
  Serial.println(digitalRead(CDHALL));*/
  }
  /*while(start-millis()<100&&digitalRead(CDHALL)!=LOW){
    
  }*/
  Serial.println("train stops");
  stopTrain();
  long start = millis();
  whileLoop(CDHALL, 2000, HIGH);
  if(digitalRead(CDHALL)!=LOW){
    Serial.println("Train moved too much");
    reverseDirection();
    setTrainSpeed(1);
    positionFirstCup();
  }else {
    Serial.println("Cup dispensed");
    dispenseCup();
  }
}


void reverseDirection() {
  if(digitalRead(MOTOR1)==HIGH&&digitalRead(MOTOR2)==LOW){
    digitalWrite(MOTOR1, LOW);
    digitalWrite(MOTOR2, HIGH);
  } else if(digitalRead(MOTOR1)==LOW&&digitalRead(MOTOR2)==HIGH) {
    digitalWrite(MOTOR1, HIGH);
    digitalWrite(MOTOR2, LOW);
  } else {
    //error.data = "Error in motor controller";
    //pub_//error.publish(&error);
  }
  Serial.print(digitalRead(MOTOR1) - digitalRead(MOTOR2));
  Serial.println(" direction");
}

void stopTrain() {
  setTrainSpeed(0);
}

void setTrainSpeed(int level) {
  Serial.println(currentSpeed);
  switch(level) {
  case 0:
    currentSpeed = 0;
    Serial.print("Set to 0");
    analogWrite(MOTOR3, currentSpeed);
    break;
  case 1:
    currentSpeed = 210;
    Serial.println(currentSpeed);
    analogWrite(MOTOR3, currentSpeed);
    Serial.println(currentSpeed);
    break;
  case 2:
    currentSpeed = 225;
    analogWrite(MOTOR3, currentSpeed);
    break;
  case 3:
    currentSpeed = 240;
    analogWrite(MOTOR3, currentSpeed);
    break;
  case 4:
    currentSpeed = 255;
    analogWrite(MOTOR3, currentSpeed);
    break;
  default:
    //error.data = "This is not a sufficient speed level";
    //pub_error.publish(&error);
    
    break;

  }
}


void dispenseCup() {
      //dispensing
//      cdServo.write(129);
      delay(1000);
 //     cdServo.write(0);
      delay(3000);
}
