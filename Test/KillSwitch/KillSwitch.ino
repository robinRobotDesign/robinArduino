int currentSensor = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 // for(int i = 0;i<40;i++){
    pinMode(A6, INPUT);
 // }
  /*
  pinMode(5, INPUT);
  pinMode(6, INPUT);*/
}

void loop() {
  // put your main code here, to run repeatedly:
 // int kill = digitalRead(5);
/*  int kill1= digitalRead(5);
  int kill2= digitalRead(24);
  int kill3= digitalRead(25);
  int kill4= digitalRead(26);
  int kill5= digitalRead(27);
  int kill6= digitalRead(28);
  int kill7= digitalRead(29);
  int kill8= digitalRead(30);
  int kill9= digitalRead(31);*/
  //Serial.println(kill1);
  //Serial.println(kill2);
 /* for(int i = 0;i<48;i++){*/
  /*  Serial.print("Sensor");
    Serial.print(22);
    Serial.print(" is :");*/
    Serial.println(digitalRead(A6));
 /* }*/
  delay(1);
}


void checkHallSensor(int hall, int timer, bool value){
  // Counts how often the sensor returns a 0
  int counter = 0;
  int n = 3; // Times to check +1
  long startTime = millis();
  
  // If the sensor is detected more than 2 times, the loops stops
  while(counter<n){
    if(digitalRead(hall)==value){
      counter++;
      if(counter==3) {
 //       Serial.println("counter: " + counter);
      }
    } else {
      counter=0;
      break;
    }

    // Delay 1 ms to check the hallsensor every millisecond
    delay(1);

    // Leave the loop as soon as the timer is reached.
    if(timer!=-1 && timer<(millis()-startTime)){
      break;
    }

    // If the counter
    if(counter==n){
      Serial.println("CurrentSensor has been updated");
      currentSensor = hall;
      Serial.println(currentSensor);
    }
  }
}
