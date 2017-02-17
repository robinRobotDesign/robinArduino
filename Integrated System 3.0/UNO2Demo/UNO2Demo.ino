#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//ScannerLED Patterns
#define DEFAULTPATTERN 0
#define ONPATTERN 1
#define OFFPATTERN 2
#define ERRORPATTERN 3
#define CORRECTPATTERN 4
#define STARTUPPATTERN 5
#define PROCESSPATTERN 0

const int myAddress = 2;        //I2C-address of this Uno, can be changed 
String lastMessage;

LiquidCrystal_I2C lcd0(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
LiquidCrystal_I2C lcd1(0x3E, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
LiquidCrystal_I2C lcd2(0x3D, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
LiquidCrystal_I2C lcd3(0x3C, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

LiquidCrystal_I2C lcds[4] = {lcd0, lcd1, lcd2, lcd3};

LiquidCrystal_I2C lcd = lcds[0];

const int stationArrows[4][3] = {{10, 9, 8}, {13, 12, 11}, {7, 6, 5}, {4, 3, 2}};

int patterns[4] = {0, 0, 0, 0};
int repeats[4] = {0, 0, 0, 0};

long arrowTimers[4] = {0, 0, 0, 0};
int patternStates[4] = {0, 0, 0, 0};


int totalLines[4] = {0, 0, 0, 0};
int currentLines[4] = {1, 1, 1, 1};
long lineTimers[4] = {0, 0, 0, 0};
String lcdStrings[4] = {"", "", "", ""};

String lcdText[4] = {"", "", "", ""};

//Variables used for displays
char c, d, e;
int lcdID, switchID, unoID, sensorID, part, total, rank;
String msg;

String msgDefault = "Default Received";



/* 
 * Ga naar CommUno2: de methodes onder decodeMessage:
 * Begin met LCDs, de simpelste berichten
 * Bestuur LCDs met displayAll(text) of displayText(lcdID, text), lcdID: [0-3], text: String
 * Bestuur ScannerLEDs met setAllPatterns(pattern, repeat) of setPattern(stationID, pattern, repeat), stationID: [0-3], pattern: een define hierboven, repeat: (aantal herhalingen, -1 is oneindig)
 * In setup() of loop() hoeft niets toegevoegd te worden
 * Parameters (als die er zijn) zijn af te lezen met parameters.charAt(i), de parameter a (char) kan daarna met convertToID(a) omgezet worden in int
 * Overzicht van messages op dropbox > TI > Beschrijving Arduino Communicatie Protocol.docx
 */

void setup() {/*
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); 
  // set up the LCD's number of columns and rows:*/
  Serial.begin(57600);

  //define ScannerLED pins
  for (int i = 2; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
  
  for (int i = 0; i < 4; i++) {
    lcds[i].begin(16, 2);
    lcds[i].noBlink();
    //lcds[i].clear;
    lineTimers[i] = millis();
  }

  setAllPatterns(DEFAULTPATTERN, -1);
  displayText(0,F("Station 1    Scan hieronder\nStation 1\nuw ticket"));

  Serial.println("TEST");
}

void loop() {
  //update ScannerLEDs and patterns
  updateDisplays(1000);
  displayText(1,F("Station 2    Scan hieronder"));  
  displayText(2,F("Station 3    Scan hieronder"));  
  displayText(3,F("Station 4\nScan hieronder"));
  displayText(0,F("Station 1    Scan hieronder"));
  updatePatterns();  
}
