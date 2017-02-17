int val = 0;
const int led = 13;
int incoming = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);     
}

void loop()
{
  incoming = Serial.available();
  while (incoming == 0)                 //Keep checking until there is something available 
  {
    incoming = Serial.available();
  }

  val = Serial.parseInt();             //Reads integers as integer rather than ASCI. Anything else returns 0

  switch (val){
    case 0:
      Serial.println("Rowbot starts the game");
      break;

     case 1:
      Serial.println("Player starts the game");
      break;

     case 2:
      Serial.println("Rowbot wins!");
      break;

     case 3:
      Serial.println("Player wins!");
      break;

     case 4:
      Serial.println("It's a draw.");
      break;
  }
}

