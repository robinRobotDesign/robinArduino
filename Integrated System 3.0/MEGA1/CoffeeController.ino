
//The setup function is called once at startup of the sketch
unsigned long start;
int waittime;

bool makeCoffee(int Drink) {
  bool value;
  switch (Drink) {
    case HOTCHOCO:
      //    coffee_status.data = "Selecting Hot Choco";
      //    pub_coffee.publish(&coffee_status);
      digitalWrite(HOTCHOCO, LOW);
      delay(1000);
      digitalWrite(HOTCHOCO, HIGH);
      
      waitForCoffee(20000);
      //    coffee_status.data = "Pouring Hot Choco";
      //    pub_coffee.publish(&coffee_status);
      /*    start = millis();
          waittime = 60000;
        //    value = waitFor(waittime);
          start = millis();
          while(millis()-start<3000){
          }*/
      //    if (value) {
      //      return waitForFinish();
      //    }
      //    coffee_status.data = "The machine took to long to answer";
      //   pub_coffee.publish(&coffee_status);
      return false;
      break;
    case LATTEMACCHIATO:
      //    coffee_status.data = "Selecting Latte Macchiato";
      //    pub_coffee.publish(&coffee_status);
      digitalWrite(LATTEMACCHIATO, LOW);
      delay(1000);
      digitalWrite(LATTEMACCHIATO, HIGH);

      waitForCoffee(30000);
      //    coffee_status.data = "The machine took to long to answer";
      //    pub_coffee.publish(&coffee_status);
      return false;
      break;
    case WIENERMELANGE:
      //    coffee_status.data = "Selecting Wiener Melange";
      //    pub_coffee.publish(&coffee_status);
      digitalWrite(WIENERMELANGE, LOW);
      delay(1000);
      digitalWrite(WIENERMELANGE, HIGH);
      //value = waitFor(60000);
      
      waitForCoffee(30000);
      //    coffee_status.data = "The machine took to long to answer";
      //    pub_coffee.publish(&coffee_status);
      return false;
      break;
    case TEA:
      //    coffee_status.data = "Selecting Tea";
      //    pub_coffee.publish(&coffee_status);
      digitalWrite(TEA, LOW);
      delay(1000);
      digitalWrite(TEA, HIGH);
      /*   value = waitFor(60000);
         if (value) {
           return waitForFinish();
         }*/
      waitForCoffee(20000);
      //    coffee_status.data = "The machine took to long to answer";
      //    pub_coffee.publish(&coffee_status);
      return false;
      break;
    case CAFFELATTE:
      //    coffee_status.data = "Selecting Caffe Latte";
      //    pub_coffee.publish(&coffee_status);
      digitalWrite(CAFFELATTE, LOW);
      delay(1000);
      digitalWrite(CAFFELATTE, HIGH);
      /*   value = waitFor(60000);
         if (value) {
           return waitForFinish();
         }*/
      waitForCoffee(30000);

      //   coffee_status.data = "The machine took to long to answer";
      //    pub_coffee.publish(&coffee_status);
      return false;
      break;
    case CAPPUCCINO:
      //    coffee_status.data = "Selecting Cappuccino";
      //    pub_coffee.publish(&coffee_status);
      digitalWrite(CAPPUCCINO, LOW);
      delay(1000);
      digitalWrite(CAPPUCCINO, HIGH);
      waitForCoffee(30000);
      //    coffee_status.data = "The machine took to long to answer";
      //    pub_coffee.publish(&coffee_status);
      return false;
      break;
    case COFFEE:
      //    coffee_status.data = "Selecting Coffee";
      //    pub_coffee.publish(&coffee_status);
      digitalWrite(COFFEE, LOW);
      delay(1000);
      digitalWrite(COFFEE, HIGH);
      //    coffee_status.data = "Initializing";
      //    pub_coffee.publish(&coffee_status);
      //    value = waitFor(60000);
      /* if (value) {

        //      coffee_status.data = "Your coffee has been served";
        //      pub_coffee.publish(&coffee_status);
         return waitForFinish();
        }*/
      waitForCoffee(30000);
      //    coffee_status.data = "The machine took to long to answer";
      //    pub_coffee.publish(&coffee_status);
      return false;
      break;
    case ESPRESSO:
      //    coffee_status.data = "Selecting Espresso";
      //    pub_coffee.publish(&coffee_status);
      digitalWrite(ESPRESSO, LOW);
      delay(1000);
      digitalWrite(ESPRESSO, HIGH);
      waitForCoffee(30000);
      //    coffee_status.data = "The machine took to long to answer";
      //    pub_coffee.publish(&coffee_status);
      return false;
      break;
    default:
      //    error.data = "Not a possible drink";
      //    pub_error.publish(&error);
      break;
  }
  return false;
}
/*
  bool waitFor(uint64_t maxwait) {
  //  uint64_t finish = millis();
  //  nrs_status.data = finish-start;
  //  pub_nrs.publish(&nrs_status);
  //  maxwait = 60000;
  while((finish-start)<maxwait){
    pwm_value = pulseIn(MONITOR, HIGH);
    if(pwm_value>10000){
  //      coffee_status.data = "ISTRUE";
  //      pub_coffee.publish(&coffee_status);
      return true;
    }
    finish = millis();
  }
  return false;
  }

  bool waitForFinish () {
  boolean isready = false;
  long coffeestart = millis();
  long finish = millis();
  while(!isready){
    pwm_value = pulseIn(MONITOR, HIGH);
    if(pwm_value>10000) {
      coffeestart = millis();
    }
    finish = millis();
    if(finish-coffeestart>5000){
      isready = true;
    }
    if(finish-start>120000){
      return false;
    }
  }
  return true;
  }
*/
bool restingState() {
  int totalcount = 0;
  int countzeros = 0;
  int countones = 0;
  bool state = true;
  while (state) {
    totalcount++;
    int value = digitalRead(MONITOR);
    if (value > 0) {
      countones++;
    } else {
      countzeros++;
    }
    if (totalcount == 100) {
      if (countones > 80) {
        return true;
      } else if (countzeros > 80) {
        return true;
      } else {
        return false;
      }
    }
  }
}

void waitForCoffee(int t) {
  start = millis();
  while (restingState()) {
    if (millis() - start > 15000) {
      
    if(useCommunication){
      sendMessageToUno("EC", uno2Address);
      sendMessageToUno("EC", uno1Address);
    }
    }
  }
  Serial.println("Left cycle");
  start = millis();
  while ((millis()-start) < t) {
    nh.spinOnce();
  }
  Serial.println(start);
  Serial.println(millis());
}
