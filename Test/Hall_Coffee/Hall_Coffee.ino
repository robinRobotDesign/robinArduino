#define NOFIELD 505L    // Analog output with no applied field, calibrate this

// Uncomment one of the lines below according to device in use A1301 or A1302
// This is used to convert the analog voltage reading to milliGauss
#define TOMILLIGAUSS 1953L  // For A1301: 2.5mV = 1Gauss, and 1024 analog steps = 5V, so 1 step = 1953mG
// #define TOMILLIGAUSS 3756L  // For A1302: 1.3mV = 1Gauss, and 1024 analog steps = 5V, so 1 step = 3756mG


long timer;



void setup() 
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  timer = millis();
  digitalWrite(13, HIGH);
}

void DoMeasurement()
{
  
// measure magnetic field
  int raw = analogRead(0);   // Range : 0..1024

//  Uncomment this to get a raw reading for calibration of no-field point
//  Serial.print("Raw reading: ");
//  Serial.println(raw);

  long compensated = raw - NOFIELD;                 // adjust relative to no applied field 
  long gauss = compensated * TOMILLIGAUSS / 1000;   // adjust scale to Gauss

  //Serial.print(gauss);
  //Serial.print(" Gauss ");

  if (gauss > 0) {
    timer = millis();
    digitalWrite(13, HIGH);     Serial.print(gauss); Serial.println(" (South pole)");
  }
  else if(gauss < 0) {
    Serial.print(gauss); Serial.println(" (North pole)");
    if (millis() - timer > 5000) {
      digitalWrite(13, LOW);
      delay(3000);
      timer = millis();
    }
  }
  else               Serial.println();
}

void loop() 
{
    delay(5);
    DoMeasurement();
}
