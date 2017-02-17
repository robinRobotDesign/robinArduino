#include <Wire.h>
#include <ros.h>
#define USE_USBCON
#include <Arduino.h>
#include <std_msgs/String.h>
#include <Servo.h>
#include <QueueList.h>
#define HALLSTATION1a 22
#define HALLSTATION1b 23
#define PostMixSensor1 24
#define PostMixSensor2 25
#define PostMixSensor3 26
#define PostMixSensor6 27
#define PostMixSensor4 28
#define PostMixSensor7 29
#define PostMixSensor5 30
#define PostMixSensor9 31
#define PostMixSensor8 32
#define PostMixSensor10 33
#define Coffee1 34
#define Coffee2 35
#define Coffee3 36
#define Coffee4 37
#define B2 39
#define B3 40
#define B4 41

#define MOTOR1 A9
#define MOTOR2 A8
#define MOTOR3 4


#define CDLDRP A3
#define CDLDRC A2
#define HALLSTATION2a 42
#define HALLSTATION2b 43
#define HALLSTATION3a 44
#define HALLSTATION3b 45
#define HALLSTATION4a 46
#define HALLSTATION4b 47
#define MONITOR 2

#define HOTCHOCO 5
#define LATTEMACCHIATO 6
#define WIENERMELANGE 7
#define TEA 8
#define CAFFELATTE 9
#define CAPPUCCINO 10
#define COFFEE 11
#define ESPRESSO 12
#define Soda1 50
#define Soda2 51
#define Soda3 52
#define Soda4 53
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
bool useCommunication = false;
int coffeechoices[] = {HOTCHOCO, LATTEMACCHIATO, WIENERMELANGE,
                       TEA, CAFFELATTE, CAPPUCCINO,
                       COFFEE, ESPRESSO
                      };
int CoffeePos[] = {Coffee1, Coffee2, Coffee3, Coffee4};
String stationLetter[] = {"_", "a", "b", "c", "d"};
bool isCurved[] = {false, false};

// Both pins for the LDR's scanning the lasers (yet to be defined), and the killswitch
const int topLDR[4] = { -1, -1, -1, -1};
const int bottomLDR[4] = { -1, -1, -1, -1};
// Kill everything when scanning 0;
const int killSwitch[4] = { -1, -1, -1, -1};

int HALL = 11;
int order = 0;

//Communication with uno's
const int uno1Address = 1; //Address of first Uno, can be changed but also change in slave code
const int uno2Address = 2; //Address of second Uno, can be changed but also change in slave code


// Initialize Servo's
const int servoPins[4] = { -1, -1, -1, -1};


int destinationSensor;
int destinationValue;

int DEST = 1;
int currentCup = -1;
int cup[] = {0, 0, 0, 0};
String currentDrinks = "";
int sodaLocation = -1;
int sodachoices[] = {Soda1, Soda2, Soda3, Soda4};
int cup1[] = {PostMixSensor4, PostMixSensor3, PostMixSensor2, PostMixSensor1};
int cup2[] = {PostMixSensor5, PostMixSensor4, PostMixSensor3, PostMixSensor2};
int cup3[] = {PostMixSensor8, PostMixSensor5, PostMixSensor7, PostMixSensor6};
int cup4[] = {PostMixSensor10, PostMixSensor8, PostMixSensor9, PostMixSensor7};

int currentSpeed;
int amountCups;
boolean forward;
//CupDispenser
Servo cdServo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int pos = 0;    // variable to store the servo position
int front = 138; //Moves to the front of the cup dispenser - 144; Difference = 101
int back = 37; //Moves to the back of the cup dispenser - 43;


//speeds used to switch train speeds (used by setTrainSpeed)
#define curvedRails 2
#define pureCurvedRails 8
#define straightRails 3
#define arrivalSpeed 5
#define arrivalSpeedStation1 6
#define arrivalSpeedStation2 7
#define arrivalSpeedStation3 9

//All hall sensors used in long distance driving
int HallSensors[] = {HALLSTATION1a, HALLSTATION1b, Coffee4, B2, HALLSTATION2a, HALLSTATION2b, B3, HALLSTATION3a, HALLSTATION3b, B4, HALLSTATION4a, HALLSTATION4b};
int currentSensor = -1;
int oldSensor = -1;

//SwitchPins
int curve[] = {A1, A10};
int straight[] = {A0, A11};

//Values for every Hall Sensor
int HallValues[] = {0, 1, 2, 10, 11, 12, 20, 21, 22, 30, 31, 32};

//List of orders
QueueList<int> orderlist;

QueueList<int> possibleDefect;

//Time-out timers for switches
long timeOutSwitches[2] = {0, 0};

//A boolean that describes whether the system is capable of working correctly
boolean SystemWorking;

//Global speed correction to compensate for hardware changes, like wheels, motor power, etc.
int globalSpeedCorrection;

//ArduinoCommunication
String message;

//ROS
ros::NodeHandle nh;
char channel[] = "set_msg";
int Drink;
//char message[] = "";
std_msgs::String order_status;
ros::Publisher pub_status("orderStatus", &order_status);
void messageCb( const std_msgs::String& toggle_msg) {
  String a = toggle_msg.data;
  char message[a.length() + 1];
  a.toCharArray(message, a.length() + 1);
  order_status.data = "Order has been received";
  String order = "BA";
  if (a.length() < 5) {
    order = "BF";
    a.toLowerCase();
    order += a;
    if(useCommunication){
      sendMessageToUno(a, uno2Address);
    }
  }
  if (a.indexOf("Order has already been scanned") >= 0) {
    if(useCommunication) {
      sendMessageToUno("BB", uno2Address);
    }
  } else if (a.indexOf("Order can not be scanned") >= 0) {
    if(useCommunication) {
      sendMessageToUno("BC", uno2Address);
    }
  } else if (message[a.length() - 1] == 'A') {
    //    order_status.data = "Station A";
    pub_status.publish(&order_status);
    DEST = 1;
    order += stationLetter[DEST];
    makeOrder(a.substring(0, a.length() - 1));
    order += a.substring(0, a.length() - 1);
    if(useCommunication) {
      sendMessageToUno(order, uno2Address);
    }
  } else if (message[a.length() - 1] == 'B') {
    //    order_status.data="Station B";
    DEST = 2;
    pub_status.publish(&order_status);
    order += stationLetter[DEST];
    makeOrder(a.substring(0, a.length() - 1));
    order += a.substring(0, a.length() - 1);

  } else if (message[a.length() - 1] == 'C') {
    //    order_status.data="Station C";
    DEST = 3;
    pub_status.publish(&order_status);
    order += stationLetter[DEST];
    makeOrder(a.substring(0, a.length() - 1));
    order += a.substring(0, a.length() - 1);

  } else if (message[a.length() - 1] == 'D') {
    order_status.data = "Station D";
    DEST = 4;
    pub_status.publish(&order_status);
    order += stationLetter[DEST];
    makeOrder(a.substring(0, a.length() - 1));
    order += a.substring(0, a.length() - 1);
  }
  nh.spinOnce();
  delay(100);
}

ros::Subscriber<std_msgs::String> sub(channel, &messageCb );


void setup() {
  /* Set to HIGH at start:
     Power postmixer, Power coffee machine, <small delay>, 4xChoices Soda, 8xChoices Coffee
  */
  Serial.begin(57600);
  Serial.println("TEST");
  Wire.begin();
  cdServo.attach(3);
  cdServo.write(back);
  for (int i = 5; i < 13; i++) {
    digitalWrite(i, HIGH);
    pinMode(i, OUTPUT);
  }
 
  digitalWrite(Soda1, HIGH);
  digitalWrite(Soda2, HIGH);
  digitalWrite(Soda3, HIGH);
  digitalWrite(Soda4, HIGH);
  pinMode(Soda1, OUTPUT);
  pinMode(Soda2, OUTPUT);
  pinMode(Soda3, OUTPUT);
  pinMode(Soda4, OUTPUT);
  Serial.println("State 0");
  if (useCommunication) {
  sendMessageToUno("FA", uno2Address);
  sendMessageToUno("FA", uno1Address);
  Serial.println("State 1");
  sendMessageToUno("FB", uno2Address);
  sendMessageToUno("FB", uno1Address);
    Serial.println("State 2");
    boolean uno1 = true;
    String reply = sendRequestToUno("ZA", uno1Address);
    Serial.println(reply);
    long startTimer = millis();
    while (reply != "ZB") {
      reply = sendRequestToUno("ZA", uno1Address);
      if (millis() - startTimer > 5000) {
        uno1 = false;
        break;
      }
    }

    boolean uno2 = true;
    reply = sendRequestToUno("ZA", uno2Address);
    startTimer = millis();
    while (reply != "ZB") {
      reply = sendRequestToUno("ZA", uno2Address);
      if (millis() - startTimer > 5000) {
        uno2 = false;
        break;
      }
    }
    if (uno1 && uno2) {

      sendMessageToUno("FC", uno2Address);
      sendMessageToUno("FC", uno1Address);

    } else {
      if(!uno1){
        sendMessageToUno("HAa", uno2Address);
        sendMessageToUno("HAb", uno1Address);
      }
      if(!uno2){
        sendMessageToUno("HAb", uno2Address);
        sendMessageToUno("HAa", uno1Address);
        
      }
    }
    String string = sendRequestToUno("FD", uno1Address);
    while(string!="FEb"){
      string = sendRequestToUno("FD", uno1Address);
    }
  }
  Serial.println("State 3");
  delay(100);
  nh.initNode();
  delay(100);
  nh.subscribe(sub);
  delay(100);
  nh.advertise(pub_status);
  pinMode(MOTOR1, OUTPUT);
  digitalWrite(MOTOR1, HIGH);
  pinMode(MOTOR2, OUTPUT);
  digitalWrite(MOTOR2, LOW);
  pinMode(MOTOR3, OUTPUT);
  digitalWrite(MOTOR3, 0);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A10, OUTPUT);
  pinMode(A11, OUTPUT);
  
  digitalWrite(straight[0], HIGH);
  delay(300);
  digitalWrite(straight[0], LOW);
  delay(1000);
  digitalWrite(straight[1], HIGH);
  delay(300);
  digitalWrite(straight[1], LOW);
  nh.spinOnce();

  //Initiate pinModes of all Hall sensors
  for (int i = 22; i < 47; i++) {
    if (i != 38) {
      pinMode(i, INPUT);
    }
  }

  SystemWorking = true;
  forward = true;
  amountCups = 0;
  currentSpeed = 0;
  globalSpeedCorrection = 25;
  checkForPossibleDefects();
  DEST = 1;
  prepareForDestination(DEST);

  nh.spinOnce();

  driveToStation();
  if(useCommunication){
    String string = sendRequestToUno("FN", uno1Address);
    sendMessageToUno("FN", uno2Address);
    while(string!="FOa"||string!="GCb"){
      sendMessageToUno("GB", uno2Address);
      string = sendRequestToUno("GB", uno1Address);
    }
    string = sendRequestToUno("GD", uno1Address);
    sendMessageToUno("GD", uno2Address);
    while(string!="GEb"){
      string = sendRequestToUno("GD", uno1Address);
      delay(100);
    }
   /* while(string != "FOb"){
      string = sendRequestToUno("FN", uno1Address);
    }*/
    
  sendMessageToUno("FP", uno2Address);
  sendMessageToUno("FP", uno1Address);
  }
//  DEST=4;
//  prepareForDestination(DEST);
//  makeOrder("000000000100");
}




void loop() {
  if (currentSensor != HALLSTATION1b) {
    String msg = "CB";
    msg+=stationLetter[DEST];
    sendMessageToUno("CB", uno2Address);
    driveTo(HALLSTATION1b);
  }
  //ORDERLIST
  // Wordt methode
  nh.spinOnce();/*
  cup[0]=1;
  cup[1]=1;
  cup[2]=1;
  cup[3]=1;*/
  while (orderlist.count() > 0 && checkFilledCups() < 4) {
    // Serial.println("In the distribution loop");
    order_status.data = "In the distribution loop";
    pub_status.publish(&order_status);
    Serial.println(orderlist.count());
    if (!checkEnoughCupsReady()) {
      Serial.println("There are not enough cups distributed");
      if (orderlist.count() > 4) {
        String msg = "CC";
        msg+=stationLetter[DEST];
        msg+="d";
        positionNextCup(4);
        if(useCommunication){         
        sendMessageToUno(msg, uno2Address); 
        }
      } else {
        message = "CC";
        message += stationLetter[DEST];
        message += stationLetter[orderlist.count()];
        sendMessageToUno(message, uno2Address);
        Serial.print("Dispense ");
        Serial.print(orderlist.count());
        Serial.println(" Cups");
        positionNextCup(orderlist.count());
        Serial.print("Amount of cups placed: ");
        Serial.println(checkCupsPlaced());

      }
    }
    int drink = orderlist.pop();
    if (contains(0, drink)) {
      setCupPosition();
      driveTo(CoffeePos[currentCup]);
      message = "CE";
      message += stationLetter[DEST];
      message += convertToParam(drink - 5);
      makeCoffee(drink);
      currentDrinks+=convertToParam(drink-5);
      setCupFilled(currentCup);
      if(useCommunication){
      sendMessageToUno(message, uno2Address);
      }
      Serial.println("Uw koffie wordt gemaakt");
    } else if (contains(1, drink)) {
      setCupPosition();
      setSodaLoc(drink);
      driveTo(getSodaSensor(currentCup));
      message = "CD";
      message += stationLetter[DEST];
      message += convertToParam(drink - 50);
      if(useCommunication){
        sendMessageToUno(message, uno2Address);
      }
      makeSoda(drink);
      currentDrinks+=convertToParam(drink-50);
      setCupFilled(currentCup);
      Serial.println("Uw soda wordt gemaakt");
    }
    nh.spinOnce();

  }

  ///DESTINATION
  //If the train contains cups, drive to the destination.
  if (!checkEmptyCups()) {
    String msg = "CF";
    msg+=stationLetter[DEST];
    sendMessageToUno(msg, uno2Address);
    Serial.println("Send Train to destination");
    prepareForDestination(DEST);

    Serial.print("My current Sensor is: ");
    Serial.println(currentSensor);

    driveToStation();
    delay(3000);
  }

  //if Orderlist is empty you are ready for a new order.
  if (orderlist.count() == 0) {
    if(useCommunication){
      sendMessageToUno("AA", uno2Address);
      sendMessageToUno("AA", uno1Address);
    }
    
    order_status.data = "Ready for new order";
    pub_status.publish(&order_status);
  }
}

/**
   Checks if there are any defects in the hall sensors
*/
void checkForPossibleDefects() {
  if(useCommunication){
  sendMessageToUno("FH", uno1Address);
  sendMessageToUno("FH", uno2Address);
  }
  for (int i = 22; i < 44; i++) {
    if (i != 38) {
      Serial.println(i);
      Serial.println(" is ");
      Serial.println(digitalRead(i));
      if (digitalRead(i) != HIGH) {
        possibleDefect.push(i);
      }
    }
  }
  Serial.print("Possible defects ");
  Serial.println(possibleDefect.count());
  if (possibleDefect.count() > 1) {
    SystemWorking = false;
    setForward();
    setTrainSpeed(1);
    delay(100);
    stopTrain();
    QueueList<int> brokenSensor;
    while (!possibleDefect.isEmpty()) {
      int i = possibleDefect.pop();
      if (digitalRead(i) != HIGH) {
        brokenSensor.push(i);
        Serial.println(i);
        Serial.println(digitalRead(i));
      }
    }
    possibleDefect = brokenSensor;
    //Rij een stukje vooruit
  } else if (possibleDefect.count() == 1) {
    setForward();
    setTrainSpeed(1);
    delay(1000);
    stopTrain();
    int brokenSensor = 0;
    while (!possibleDefect.isEmpty()) {
      int i = possibleDefect.pop();
      if (digitalRead(i) != HIGH) {
        brokenSensor = i;
      }
    }
    if (brokenSensor > 0) {
      possibleDefect.push(brokenSensor);

    }
  }

  Serial.print("there are ");
  Serial.println(possibleDefect.count());
  Serial.print("defects");
  if(possibleDefect.count()==0){
    sendMessageToUno("FI", uno1Address);
    sendMessageToUno("FI", uno2Address);
  } else if(possibleDefect.count()>0){
    SystemWorking = false;
    message = "HC";
    while(possibleDefect.count()>0){
      message += convertToParam(possibleDefect.pop());
    }
    if(useCommunication){
    sendMessageToUno(message, uno1Address);
    sendMessageToUno(message, uno2Address);
    }
  }
}

/**
   Sets the rails for the destination DEST (1-4).
*/
void prepareForDestination(int DEST) {
  if (DEST == 4) {
    setRail(0, 0);
    delay(1000);
    setRail(1, 0);
    Serial.println("Switch to station 4");
  } else if (DEST == 3) {
    setRail(0, 0);
    delay(1000);
    setRail(1, 1);
    Serial.println("Switch to station 3");
  } else if (DEST == 2) {
    setRail(0, 1);
    Serial.println("Switch to station 2");
  } else {
    Serial.println("Destination is station 1");
  }
  setDestinationSensor(DEST);
}

/**
   Used to drive long distances
*/
void driveToStation() {

  if (currentSensor == -1) {
    if (DEST > 1) {
      setForward();
      setTrainSpeed(1);
      Serial.print("Train is driving to station ");
      Serial.println(DEST);
    } else {
      Serial.println("Train is driving to station 1");
      setBackward();
      setTrainSpeed(1);
    }
  } else {
    if (currentSensor > destinationSensor) {
      setBackward();
      setTrainSpeed(1);
    } else if (currentSensor < destinationSensor) {
      if (currentSensor == HALLSTATION2a || currentSensor == HALLSTATION2b
          || currentSensor == HALLSTATION3a || currentSensor == HALLSTATION3b) {
        setBackward();
        setTrainSpeed(1);
      } else {
        setForward();
        setTrainSpeed(1);
      }
    }
  }
  //setTrainSpeed(1);

  checkLimitSwitches();


  //Loops until the destination is reached

  //   setTrainSpeed(2);
  while (currentSensor != destinationSensor) {
    /*
      Serial.print("while loop 1: ");
      Serial.print(currentSpeed);
      Serial.print(" | ");
      Serial.print(currentSensor);
      Serial.print(" | ");
      Serial.println(destinationSensor);
    */
    //checks every HallSensor/
    for (int i = 0; i < NELEMS(HallSensors); i++) {
      //      Serial.println("WhileLoop1");
      checkHallSensor(HallSensors[i], -1, LOW);
    }
    checkLimitSwitches();
    //updates values on current sensor
    if (oldSensor != currentSensor) {

      setCurrentState(currentSensor);
      oldSensor = currentSensor;
    }
    nh.spinOnce();
  }

  //Serial.print("Out of the loop");
  sensorCorrection(destinationSensor);
  //if (useCommunication) {               //BLABLABLA
    String message = "CK";
    message+=stationLetter[DEST];
    message += currentDrinks;
    String a = sendRequestToUno(message, uno1Address);
    
    delay(100);
    while (a != "CLb") {
      a = sendRequestToUno(message, uno1Address);
      delay(100);
    }
    currentDrinks="";
    message = "CM";
    message += stationLetter[DEST];
    sendRequestToUno(message, uno1Address);
    delay(100);
    while (a != "CNb") {
      a = sendRequestToUno(message, uno1Address);
      delay(100);
    }
  //}
  
  cleanCups();

}

/**
   Checks if a limit switch has been activated
*/
void checkLimitSwitches() {

  if (checkLimitSensor(A4, LOW) || checkLimitSensor(A5, LOW) || checkLimitSensor(A6, LOW)) {
    stopTrain();
    delay(1000);
    setBackward();
    
    setTrainSpeed(arrivalSpeed);
    int t = 5000;
    long start = millis();
    currentSensor = -1;
    while(currentSensor==-1){
      checkHallSensor(HALLSTATION2b, -1, LOW);
      checkHallSensor(HALLSTATION3b, -1, LOW);
      checkHallSensor(HALLSTATION4b, -1, LOW);
      if(millis()-start>5000){
    if(useCommunication){
        sendMessageToUno("DC", uno2Address);
        sendMessageToUno("DC", uno1Address);
    }
      }
    }
    Serial.println("Limit switch has been touched");
  } else if (checkLimitSensor(A7, LOW)) {
    stopTrain();
    delay(1000);
    setForward();
    setTrainSpeed(1);
    delay(100);
    stopTrain();
    Serial.println("Limit switch has been touched");
  }
}

/**
   Drive to a specific sensor between sensor 22 and 38
*/
void driveTo(int destination) {
  Serial.println("in driveTo");
  Serial.println(currentSensor);
  Serial.println(destination);
  if (currentSensor != destination) {
    int currentDirection = 0;
    if (currentSensor < destination) {
      currentDirection = 1;
      setForward();
    } else {
      currentDirection = -1;
      setBackward();
    }
    int nextDestinations[] = { -1, -1, -1};
    setTrainSpeed(1);
    while ((currentSensor < destination && currentDirection > 0) || (currentSensor > destination && currentDirection < 0)) {
      checkLimitSwitches();
      if (currentSensor > 38) {
        nextDestinations[0] = 38 + currentDirection * 1;
        nextDestinations[1] = 38 + currentDirection * 2;
        nextDestinations[2] = 38 + currentDirection * 3;
      } else {
        nextDestinations[0] = currentSensor + currentDirection * 1;
        nextDestinations[1] = currentSensor + currentDirection * 2;
        nextDestinations[2] = currentSensor + currentDirection * 3;
      }
      for (int i = 0; i < 2; i++) {
        if (38 > nextDestinations[i] && nextDestinations[i] > 21) {
          checkHallSensor(nextDestinations[i], -1, LOW);
        }
      }
    }
    stopTrain();
    sensorCorrection(destination);

  }
}

/**
   Used to filter out false positives from the hall sensors
   hall = hall sensor pin
   timer = the maximum wait time before the while loop is left
   value = HIGH or LOW according to what the sensor should do
*/
void whileLoop(int hall, int timer, bool value) {
  int counter = 0;
  long start = millis();
  while (counter < 3) {
    if (digitalRead(hall) == value) {
      counter++;
      if (counter > 0) {
        //      Serial.print("counter: ");
        //      Serial.println(counter);
      }
    } else {
      counter = 0;
    }
    delay(1);
    if (timer > 0 && timer < millis() - start) {
      break;
    }
  }
}


/**
   Used to position the cup correctly on the hall sensor
   HALL = pin of the hall effect sensor
*/
void positionCup(int HALL) {
  whileLoop(HALL, -1, LOW);
  Serial.println("Train arrives");

  Serial.println("train stops");
  stopTrain();
  long start = millis();
  whileLoop(HALL, 2000, HIGH);
  if (digitalRead(HALL) != LOW) {
    Serial.println("Train moved too much");
    reverseDirection();
    setTrainSpeed(1);
    positionCup(HALL);
  } else {
    Serial.println("Done with positioning");
  }
}


// POSTMIXERPOSITIONEREN

/**
   Sets sodalocation which is used to determine the location at which the train should stop to fill a specific cup
*/
void setSodaLoc(int i) {
  switch (i) {
    case Soda1: sodaLocation = 0; break;
    case Soda2: sodaLocation = 1; break;
    case Soda3: sodaLocation = 2; break;
    case Soda4: sodaLocation = 3; break;
  }
}

/**
   returns the location of the sensor the train should drive to fill cup
*/
int getSodaSensor(int i) {
  switch (i) {
    case 0:
      return cup1[sodaLocation];
      break;
    case 1:
      return cup2[sodaLocation];
      break;
    case 2:
      return cup3[sodaLocation];
      break;
    case 3:
      return cup4[sodaLocation];
      break;
  }
}

//CUPINFORMATION

/**
   Returns a preset value of the sensor to define its position according to the rest of the sensors
*/
int getSensorValue(int sensor) {
  for (int i = 0; i < NELEMS(HallSensors); i++) {
    if (sensor == HallSensors[i]) {
      return HallValues[i];
    }
  }
}

/**
   Used to correct the train position
   i = Hall Sensor pin
*/
void sensorCorrection(int i) {
  sensorCorrection(i, false);
}

/**
   Used to correct the train position
   i = Hall Sensor pin
*/
void sensorCorrection(int i, boolean repeated) {
  Serial.print("Correcting train position,");
  Serial.print(i);
  Serial.print(" = ");
  Serial.println(digitalRead(i));
  if (repeated) {
    whileLoop(i, -1, LOW);
  }
  stopTrain();
  long start = millis();
  whileLoop(i, 2000, HIGH);
  if (digitalRead(i) != LOW) {
    Serial.println("Train moved too much");
    reverseDirection();
    setTrainSpeed(1);
    sensorCorrection(i, true);
  }
}

//Provides the state for the hall sensor currently scanned.
void setCurrentState(int i) {
  currentSensor = i;
  switch (i) {
    case HALLSTATION1a:
      if (getSensorValue(i) < destinationValue) {
        setForward();
        setTrainSpeed(straightRails);
        currentSensor = HALLSTATION1a;
      } else if (i == destinationSensor) {
        stopTrain();
        currentSensor = HALLSTATION1a;
      } else {
        stopTrain();
        currentSensor = HALLSTATION1a;
      }
      break;
    case HALLSTATION1b:
      if (getSensorValue(i) < destinationValue) {
        setForward();
        setTrainSpeed(straightRails);
        currentSensor = HALLSTATION1b;
      } else {
        setBackward();
        currentSensor = HALLSTATION1b;
        setTrainSpeed(arrivalSpeedStation1);
      }
      break;
    case Coffee4:
      if (getSensorValue(i) < destinationValue) {
        setForward();
        setTrainSpeed(pureCurvedRails);
        currentSensor = Coffee4;
      } else {
        setBackward();
        setTrainSpeed(1);
        currentSensor = Coffee4;
      }
      break;
    case B2:
      if (getSensorValue(i) < destinationValue) {
        setForward();
        if (destinationSensor == HALLSTATION2b) {
          //        setRail(0,1);
          setTrainSpeed(curvedRails);
          currentSensor = B2;
        } else {
          //        setRail(0,0);
          setTrainSpeed(straightRails);
          currentSensor = B2;
        }
      } else {
        setBackward();
        setTrainSpeed(curvedRails);
      }
      break;
    case HALLSTATION2a:
      if (getSensorValue(i) < destinationValue) {
        if (destinationSensor == HALLSTATION2b) {
          Serial.println("HALLSTATION2a: Drive to HALLSTATION2b");
          setForward();
          setTrainSpeed(arrivalSpeedStation2);
          currentSensor = HALLSTATION2a;
        } else {
          Serial.println("HALLSTATION2a: Drive back to B2");
          //        setRail(0,1);
          //possibly set rails to curved
          setBackward();
          setTrainSpeed(curvedRails);
          currentSensor = HALLSTATION2a;
        }
      } else {
        Serial.println("HALLSTATION2a: Drive back to B2");
        //possibly set rails to curved
        setBackward();
        setTrainSpeed(curvedRails);
        currentSensor = HALLSTATION2a;
      }
      break;
    case HALLSTATION2b:
      if (getSensorValue(i) < destinationValue) {
        setBackward();
        setTrainSpeed(straightRails);
        currentSensor = HALLSTATION2b;
      } else if (i == destinationSensor) {
        stopTrain();
        currentSensor = HALLSTATION2b;
      } else {
        setBackward();
        setTrainSpeed(straightRails);
        currentSensor = HALLSTATION2b;
      }
      break;
    case B3:
      if (getSensorValue(i) < destinationValue) {
        setForward();
        if (destinationSensor == HALLSTATION3b) {
          //        setRail(1,1);
          setTrainSpeed(curvedRails);
        } else {
          Serial.println("Set rails to curve");
          //        setRail(1,0);
          setTrainSpeed(straightRails);
        }
      } else {
        Serial.println("set rails to straight");
        //      setRail(0,0);
        setBackward();
        setTrainSpeed(straightRails);
      }
      break;
    case HALLSTATION3a:
      if (getSensorValue(i) < destinationValue) {
        if (destinationSensor == HALLSTATION3b) {
          setForward();
          setTrainSpeed(arrivalSpeedStation2);
        } else {
          //possibly set rails to curved
          setBackward();
          setTrainSpeed(curvedRails);
        }
      } else {
        //possibly set rails to curved
        setBackward();
        setTrainSpeed(curvedRails);
      }
      break;
    case HALLSTATION3b:
      if (getSensorValue(i) < destinationValue) {
        setBackward();
        setTrainSpeed(straightRails);
      } else if (i == destinationSensor) {
        stopTrain();
      } else {
        setBackward();
        setTrainSpeed(straightRails);
      }
      break;
    case B4:
      if (getSensorValue(i) < destinationValue) {
        setForward();
        setTrainSpeed(curvedRails);
      } else {
        //      setRail(1,0);
        setBackward();
        setTrainSpeed(straightRails);
      }
      break;
    case HALLSTATION4a:
      if (getSensorValue(i) < destinationValue) {
        setForward();
        setTrainSpeed(arrivalSpeedStation2);
      } else {
        setBackward();
        setTrainSpeed(curvedRails);
      }
      break;
    case HALLSTATION4b:
      if (i == destinationSensor) {
        stopTrain();
      } else {
        setBackward();
        setTrainSpeed(straightRails);
      }
      break;

  }
}

/* This method keeps looping until the desired value of the hallsensor sustains within the time of the timer.
    int hall = The hallsensor to be checked
    int timer = The amount of time that has to be checked (if timer = -1, no time limit is used)
    bool value = Whether it needs to be checked whether the train detects the hallsensor (true/HIGH), or leaves the hallsensor (false/LOW)
*/
void checkHallSensor(int hall, int timer, bool value) {
  // Counts how often the sensor returns a 0
  int counter = 0;
  int n = 7; // Times to check +1
  long startTime = millis();

  // If the sensor is detected more than 2 times, the loops stops
  while (counter < n) {
    if (digitalRead(hall) == value) {
      counter++;
      if (counter == n) {
        //       Serial.println("counter: " + counter);
      }
    } else {
      counter = 0;
      break;
    }

    // Delay 1 ms to check the hallsensor every millisecond
    //delay(1);

    // Leave the loop as soon as the timer is reached.
    if (timer != -1 && timer < (millis() - startTime)) {
      break;
    }

    // If the counter
    if (counter == n) {
      Serial.println("CurrentSensor has been updated");
      currentSensor = hall;
      Serial.print(currentSensor);
      Serial.print(" found in ms: ");
      Serial.println(millis() - startTime);
    }
  }
}

/**
   used to correct false positives of the hall sensors
*/
bool checkLimitSensor(int hall, bool value) {
  // Counts how often the sensor returns a 0
  int counter = 0;
  int n = 7; // Times to check +1

  // If the sensor is detected more than 2 times, the loops stops
  while (counter < n) {
    if (digitalRead(hall) == value) {
      counter++;
      if (counter == n) {
        //       Serial.println("counter: " + counter);
      }
    } else {
      counter = 0;
      return false;
    }
    // If the counter
    if (counter == n) {
      return true;
    }
  }
}

/* Sets the rails
   i = switchID
   j == 1 means curv
   j == 0 means forward
*/
void setRail(int i, int j) {
  int wait = 5000;

  if (timeOutSwitches[i] < (millis() - wait)) {
    if (j) {
      if (isCurved[i] == false) {
        Serial.println("Rail: " + i);
        digitalWrite(curve[i], HIGH);
        delay(300);
        digitalWrite(curve[i], LOW);
        isCurved[i] = true;
      }
    } else {
      if (isCurved[i] == true) {
        digitalWrite(straight[i], HIGH);
        delay(300);
        digitalWrite(straight[i], LOW);
        isCurved[i] = false;
      }
      
    }
    timeOutSwitches[i] = millis();
  }
}

/**
   Sets the switches according to the destination of the train.
*/
void setSwitch(int dest) {
  if (dest == 4) {
    setRail(0, 0);
    setRail(1, 0);
    Serial.println("Switch to station 4");
  } else if (dest == 3) {
    setRail(0, 0);
    setRail(1, 1);
    Serial.println("Switch to station 3");
  } else if (dest == 2) {
    setRail(0, 1);
    Serial.println("Switch to station 2");
  }
}

/**
   Checks if the train is at the correct destination.
*/
void setDestinationSensor(int i) {
  switch (i) {
    case 1: destinationSensor = HALLSTATION1a;
      destinationValue = getSensorValue(HALLSTATION1a);
      break;
    case 2: destinationSensor = HALLSTATION2b;
      destinationValue = getSensorValue(HALLSTATION2b);
      break;
    case 3: destinationSensor = HALLSTATION3b;
      destinationValue = getSensorValue(HALLSTATION3b);
      break;
    case 4: destinationSensor = HALLSTATION4b;
      destinationValue = getSensorValue(HALLSTATION4b);
      break;
  }
  Serial.print("destination = ");
  Serial.println(i);
}

/**
   checks if a drink is in coffeechoices or in sodachoices
   drinks = integer of current drink
   value = 0 if coffeeChoices 1 if sodaChoices
*/
bool contains(int drinks, int value) {
  if (drinks == 0) {
    for (int i = 0; i < NELEMS(coffeechoices) ; i++) {
      if (coffeechoices[i] == value) {
        return true;
      }
    }

  }
  if (drinks == 1) {
    for (int i = 0; i < NELEMS(sodachoices) ; i++) {
      if (sodachoices[i] == value) {
        return true;
      }
    }
  }
  return false;
}

