
void dispenseCup() {
  for (pos = front; pos >= back; pos -= 1) { // goes from front degrees to back degrees
    cdServo.write(pos);              // tell servo to go to position in variable 'pos'
    //Serial.println(pos);
    delay(5);                       // waits 5ms for the servo to reach the position
  }
  delay(100);
  for (pos = back; pos <= front; pos += 1) { // goes from back degrees to front degrees
    // in steps of 1 degree
    cdServo.write(pos);              // tell servo to go to position in variable 'pos'
    //Serial.println(pos);
    delay(5);                       // waits 5ms for the servo to reach the position
  }
  delay(1500);
}

void positionNextCup(int cups) {
  if(cups<=4){
    for(int i = 0; i<(cups);i++){
      Serial.print("Position cup: ");
      Serial.println(i+1);
      setForward();
      laserPosition(i);
      setCupPlaced(i);
      checkCupsPlaced();
      Serial.print("Cup ");
      Serial.print(i+1);
      Serial.println(" is dispensed");
    }
  } else {
    Serial.println("Too many cups to dispense");
  }
  nh.spinOnce();
}

void laserPosition(int i) {
  Serial.println("state 1");
  Serial.println(digitalRead(CDLDRP));//lasertoren onder
  Serial.println(digitalRead(CDLDRC));//lasertoren boven

  alignTrain();
  
  if(i!=0){
    cupTwoUnitWhile(CDLDRP, CDLDRC, LOW, LOW, false, -1);
//  while(!(digitalRead(CDLDRP)==LOW&&digitalRead(CDLDRC)==LOW)){}
    Serial.println("state 2:");
    Serial.println(digitalRead(CDLDRP));
    Serial.println(digitalRead(CDLDRC));
    cupTwoUnitWhile(CDLDRP, CDLDRC, HIGH, HIGH, false, -1);
//  while(!(digitalRead(CDLDRP)==HIGH&&digitalRead(CDLDRC)==HIGH)){}
    Serial.println("state 3:");
    Serial.println(digitalRead(CDLDRP));
    Serial.println(digitalRead(CDLDRC));
    int counter = 0;
    long start = millis();
    
  cupTwoUnitWhile(CDLDRP, CDLDRC, LOW, LOW, false, -1);
    Serial.println("state 4:");
    Serial.println(digitalRead(CDLDRP));
    Serial.println(digitalRead(CDLDRC));
/*  while(!checkCupLocation()){
  }*/
  }
  cupTwoUnitWhile(CDLDRP, CDLDRC, HIGH, LOW, false, -1);
  Serial.print("state 5:");
  Serial.println(digitalRead(CDLDRP));
  Serial.println(digitalRead(CDLDRC));
  Serial.println("Train arrived at laser position");
  stopTrain();
  delay(1000);
  start=millis();
  while(checkCupLocation()&&millis()-start<1000){
  }
//check high and low for 2 seconds
  if(!checkCupLocation()){
    Serial.println("Train not positioned correctly");
//    locationCorrection();
//    check correct behavior
    //HandTest
    reverseDirection();
    //setTrainSpeed(1);
    alignTrain();
    laserPosition(0);
  } else {
      Serial.println("Train is positioned correctly");
    dispenseCup();
    bool a = cupTwoUnitWhile(CDLDRP, CDLDRC, LOW, LOW, false, 10000);
    if(a){
      Serial.println("Cup is dispensed");
    } else {
      
    if(useCommunication){
      sendMessageToUno("EB", uno2Address);
      sendMessageToUno("EB", uno1Address);
    }
    }
    /*
    while(!twoUnitWhile(CDLDRP, CDLDRC, LOW, LOW,false)){
      Serial.println("waiting for cup");
    } */
    //CREATE CONFIRMATION DISPENSED CUP
    //CREATE ERROR.  
    
    
  }
}


bool checkCupLocation() {
  return (digitalRead(CDLDRP)==HIGH&&digitalRead(CDLDRC)==LOW);
}

/*
 * unit1 = pin of unit 1
 * unit2 = pin of unit 2
 * value1 = value of unit1
 * value2 = value of unit2
 * possibleBreak = do you want to break out of the while loop if the counter resets to 0?
 */
void twoUnitWhile(int unit1, int unit2, int value1, int value2 , bool possibleBreak) {
  int counter = 0;
  while(counter<3){
    if((digitalRead(unit1)==value1&&digitalRead(unit2)==value2)){
      counter++;
      if(counter>0){/*
        Serial.print("counter: ");
        Serial.println(counter);*/
      }
    } else {
      counter=0;
      if(possibleBreak){
        break;
      }
    }
    delay(1);
  }
}

/**
 * Works like TwoUnitWhile(..) but with analog values.
 */
void cupTwoUnitWhile(int unit1, int unit2, int value1, int value2 , bool possibleBreak) {
  cupTwoUnitWhile(unit1, unit2, value1, value2, possibleBreak, -1);
}

bool cupTwoUnitWhile(int unit1, int unit2, int value1, int value2 , bool possibleBreak, int t) {
  long start = millis();
  int counter = 0;
  while(counter<3){
    if(t>=0){
      if((millis()-start)>t){
        return false;
      }
    }
    int sensorValue = analogRead(unit1);
    int sensorValue2 = analogRead(unit2);
    if((sensorValue>300)==value1&&(sensorValue2>400)==value2){
      counter++;
      if(counter>0){/*
        Serial.print("counter: ");
        Serial.println(counter);*/
      }
    } else {
      counter=0;
      if(possibleBreak){
        break;
      }
    }
    
    delay(1);
  }return true;
}

