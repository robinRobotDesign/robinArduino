int val = 0;
int incoming = 0;

void setup()
{
  Serial.begin(9600);     
  Serial.println("Serial Monitor Connected");    //Is not an integer so it won't interfere
}

void loop()
{
  incoming = Serial.available();  
  while (incoming == 0)     //keep checking until something is available
  {
    incoming = Serial.available();
  }
  Serial.print("Received value... Transmitting:  ");
  val = Serial.parseInt();  //Reads integers as integer rather than ASCI. Anything else returns 0
                            //Note that this is a user input via the Serial Monitor
                            
   Serial.println(val);       //This is being sent to the Rx board

}
