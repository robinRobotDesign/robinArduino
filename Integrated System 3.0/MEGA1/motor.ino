
void setBackward() {
  digitalWrite(MOTOR1, LOW);
  digitalWrite(MOTOR2, HIGH);
  forward = false;
}
void setForward() {
  digitalWrite(MOTOR1, HIGH);
  digitalWrite(MOTOR2, LOW);
  forward = true;
}
void reverseDirection() {
  if(forward){
    setBackward();
  } else if(!forward) {
    setForward();
  } else {
    //error.data = "Error in motor controller";
    //pub_//error.publish(&error);
  }
  Serial.print(digitalRead(MOTOR1) - digitalRead(MOTOR2));
  Serial.println(" direction");
}

void stopTrain() {
  setTrainSpeed(0);
  currentSpeed=0;
  Serial.println(currentSpeed);
}

//Only for aligning train with cup dispenser, derivative of setTrainSpeed(), but with minimal initial boost.
void alignTrain() {
  int desiredSpeed = 160;

  //Global speed correction to compensate for hardware changes
  desiredSpeed += globalSpeedCorrection;

  //Compensate by increasing speed by 3 when going backwards
  if (!forward) {
      desiredSpeed += 3;
  }

  int startSpeed = 160;
  int topSpeed = 220;
  int deltaSpeed = 5;

  int delayVal = 4;
    
  boolean topSpeedReached = false;
  boolean belowCupDispenser = false;
    
  currentSpeed = startSpeed;
  analogWrite(MOTOR3, currentSpeed);
      
  while(!((topSpeedReached && currentSpeed <= desiredSpeed) || belowCupDispenser)) {
    if (!topSpeedReached && currentSpeed >= topSpeed) {
      topSpeedReached = true;
    } else if (topSpeedReached) {
      currentSpeed -= deltaSpeed;
      analogWrite(MOTOR3, currentSpeed);
    } else {
       currentSpeed += deltaSpeed;
       analogWrite(MOTOR3, currentSpeed);
    }
    //Serial.println(currentSpeed);
    
    long timer = millis();
    int counter = 0;

    while(millis()-timer<delayVal && counter<1){
      if((digitalRead(CDLDRP)==HIGH && digitalRead(CDLDRC)==LOW)){ //State 5: right position to dispense cups
        counter++;
        if(counter >= 1){
          belowCupDispenser = true;
          break;
        }
      } else {
        counter=0;
      }
    }
  }
  currentSpeed = desiredSpeed;
  analogWrite(MOTOR3, currentSpeed);
}



void setTrainSpeed(int level) {
  int desiredSpeed = 0;
  
  Serial.println(currentSpeed);
  switch(level) {
  case 0:
    desiredSpeed = 0;
    currentSpeed = desiredSpeed;
    Serial.print("Set to 0");
    analogWrite(MOTOR3, currentSpeed);
    break;
  case 1:
  //Normal speed
    desiredSpeed = 200;
    break;
  case 2:
  //CurveSpeed
    desiredSpeed = 200;
    break;
  case 8:
  //PureCurvedSpeed
    desiredSpeed = 205;
  case 3:
  //StraightSpeed
    desiredSpeed = 215;
    break;
  case 4:
  //Max
    desiredSpeed = 255;
    break;
  case 5:
    //ArrivalSpeed
    desiredSpeed = 180;
    break;
  case 6:
  //ArrivalSpeedStation1
    if(currentSpeed<170){
      desiredSpeed = 180;
    } else {
      desiredSpeed = 160;
    }
    break;
    case 7:
    //ArrivalSpeedStation2
    desiredSpeed = 150;
    break;
    case 9:
   //ArrivalSpeedStation3
    desiredSpeed = 160;
    break;
  default:
    //error.data = "This is not a sufficient speed level";
    //pub_error.publish(&error);
    break;
  }

  //Global speed correction to compensate for hardware changes
  if (desiredSpeed + globalSpeedCorrection > 255) {
    desiredSpeed = 255;
  } else {
    desiredSpeed += globalSpeedCorrection;
  }

  
  Serial.print("desiredSpeed = ");
  Serial.println(desiredSpeed);
  Serial.print("currentSpeed = ");
  Serial.println(currentSpeed);

  if(level > 0) {
    //Compensate by increasing speed by 3 when going backwards
    if (!forward) {
      if (desiredSpeed+3 > 255) {
        desiredSpeed = 255;
      } else {
        desiredSpeed += 3;
      }
    }

    //Compensate by increasing speed by 1 for each full carried cup
    if (desiredSpeed+amountCups > 255) {
      desiredSpeed = 255;
    } else {
      desiredSpeed += amountCups;
    }
    
    if(currentSpeed>170 || level == 5 || level == 6){
      currentSpeed = desiredSpeed;
      analogWrite(MOTOR3, currentSpeed);
    } else {
      //If speed is too low to start from halt, give train an initial controlled speed boost
      //While regulating speed boost, check destination sensor and stop regulating when destination has been reached.
      int startSpeed = 180;
      int topSpeed = 240;
      int deltaSpeed = 5;

      int delayVal = 5;
    
      boolean topSpeedReached = false;
      boolean onHallSensor = false;
    
      currentSpeed = startSpeed;
      analogWrite(MOTOR3, currentSpeed);

      Serial.print("startSpeed = ");
      Serial.println(currentSpeed);
      
      while(!((topSpeedReached && currentSpeed <= desiredSpeed) || onHallSensor)) {
        if (!topSpeedReached && currentSpeed >= topSpeed) {
          topSpeedReached = true;
        } else if (topSpeedReached) {
          currentSpeed -= deltaSpeed;
          analogWrite(MOTOR3, currentSpeed);
        } else {
          currentSpeed += deltaSpeed;
          analogWrite(MOTOR3, currentSpeed);
        }
        Serial.println(currentSpeed);
        
        long timer = millis();
        int counter = 0;

        //In between the time (=delayVal) that the speed isn't changed, check destination sensor
        while(millis()-timer<delayVal && counter<3){
          if(digitalRead(HALL)==LOW){
            counter++;
            if(counter >= 3){
              onHallSensor = true;
              break;
            }
          } else {
            counter=0;
          }
        }
      }
      
      currentSpeed = desiredSpeed;
      analogWrite(MOTOR3, currentSpeed);
      Serial.println("test");
    }
  }

  Serial.print("desiredSpeed = ");
  Serial.println(desiredSpeed);
  Serial.print("currentSpeed = ");
  Serial.println(currentSpeed);
  /*
  for(int i = 200;i<240;i=i+deltaSpeed){
    currentSpeed += deltaSpeed;
    analogWrite(MOTOR3, currentSpeed);
    delay(100);
  }
  
  currentSpeed = 20;
  analogWrite(MOTOR3, currentSpeed);
  delay(200);
  */
  
  //currentSpeed =170;
  //Serial.println(currentSpeed);
  //analogWrite(MOTOR3, currentSpeed);
  //Serial.println(currentSpeed);
}


