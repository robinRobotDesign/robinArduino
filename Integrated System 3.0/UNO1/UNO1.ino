#include <Servo.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            3

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS       293//293//0..373
//#define NUMPIXELS       449

//Configuration for single LED strip (Testing)
/*
#define PICKUP          0
#define DISPENSER       5
#define POSTMIXER       10
#define COFFEE          15
#define TUNNEL_START    20
#define TUNNEL_A        30
#define TABLE_1         40
#define TUNNEL_B        60
#define TABLE_2         70
#define TUNNEL_C        90
#define TABLE_3         99
*/


//Configuration for full track
// /*
//#define PICKUP          0
//#define DISPENSER       0
//#define POSTMIXER       0
//#define COFFEE          0
#define TUNNEL_START    0
#define TUNNEL_A        80
#define TABLE_1         113
#define TUNNEL_B        171
#define TABLE_2         202
#define TUNNEL_C        262
#define TABLE_3         292
// */

//LED Strip states
#define ORDERLESS 0
#define TEST 1
#define SINGLEDATAFLOW 2
#define DOUBLEDATAFLOW 3
#define MULTIDATAFLOW 4
#define RAINBOW 5
#define GLOW 6
#define XMAS 7
#define RED 8
#define GREEN 9
#define BLUE 10
#define OFF 11

//StationDoors states
#define STOP 0
#define OPEN 1
#define CLOSE 2
#define OPENDEFECT 3
#define CLOSEDEFECT 4
#define CLOSEALL 5
#define CLOSEALLDEFECT 6
#define CHECKLDRS 7


const int myAddress = 1;        //I2C-address of this Uno, can be changed 
String lastMessage;


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int pathArray[NUMPIXELS];
int pathLength;

long ledTimer;
int currentStep;
int ledEffect;



// Both pins for the LDRs scanning the lasers (yet to be defined), and the killswitch
const int topLDR[4] = {A0, A1, A2, A3};
const int bottomLDR[4] = {2, 4, 7, 8};


// Initialize Servo's
const int servoPins[4] = {5, 6, 9, 10};


Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;

Servo servos[4] = {servo0, servo1, servo2, servo3};

int startPos = 150; //Fully closed
int endPos = 40; //Fully opened

int state[4] = {STOP, STOP, STOP, STOP};

int servoPos[4] = {startPos, startPos, startPos, startPos};

boolean isOpen[4] = {false, false, false, false};
boolean isClosed[4] = {false, false, false, false};
boolean isEmptied[4] = {false, false, false, false};
boolean isUnobstructed[4] = {false, false, false, false};


long servoTimers[4] = {0, 0, 0, 0};
long emptyTimers[4] = {0, 0, 0, 0};
long obstructionTimers[4] = {0, 0, 0, 0};

int emptyCounters[4] = {0, 0, 0, 0};
int obstructionCounters[4] = {0, 0, 0, 0};
int emptyThreshold = 10;
int obstructionThreshold = 10;


int stationID = 0;

void setup() {
  Serial.begin(57600);

  //Initiate pins
  for(int i = 0; i<4; i++) {
    if (topLDR[i] != -1) {
      pinMode(topLDR[i], INPUT);
    }
    
    if (bottomLDR[i] != -1) {
      pinMode(bottomLDR[i], INPUT);
    }
    

    if (servoPins[i] != -1) {
      servos[i].attach(servoPins[i]);
    }

    //Set servos in the halfway open position
    if (servoPins[i] != -1) {
      //servos[i].write((startPos+endPos)/2);
      servos[i].write(servoPos[i]);
    }
  }

  Wire.begin(myAddress);        //I2C-address of this Uno is 1, can be changed 
  Wire.onReceive(receiveEvent); //Register event for incoming message
  Wire.onRequest(requestEvent); // register event
  lastMessage = "";

  pixels.begin(); // This initializes the NeoPixel library.

  ledTimer = millis();
  currentStep = 0;
  
  defaultPath();
  //setEffect(ORDERLESS);
  Serial.println("TEST");
  setEffect(RAINBOW);
}

int counter = 0;

void loop() {

  
  updateAllLDRs();
  updateAllDoors();

  
  showEffect();

  
  if (counter == 1000) {
    Serial.println("Test:");
    Serial.print("Station: ");
    Serial.println(stationID);
    Serial.print("servoPos: ");
    Serial.println(servoPos[stationID]);
    Serial.print("state: ");
    Serial.println(state[stationID]);
    Serial.print("isOpen: ");
    Serial.println(isOpen[stationID]);
    Serial.print("isClosed: ");
    Serial.println(isClosed[stationID]);
    Serial.print("isEmptied: ");
    Serial.println(isEmptied[stationID]);
    Serial.print("isUnobstructed: ");
    Serial.println(isUnobstructed[stationID]);
    Serial.print("obstructionCounters: ");
    Serial.println(obstructionCounters[stationID]);
    //Serial.print(""
    counter = 0;
  }
  counter++;
  delay(1);
  
  
}


