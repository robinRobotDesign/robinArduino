/*Arduino Slave Receiver code for robin Robot Design
 *Written on 13-01-17 by Arjan Buyserd
 *
 *To be implemented in overall program by our TI-wizards, good luck!
 */

#include <Wire.h>

const int myAddress = 1;        //I2C-address of this Uno is 1, can be changed 
int counter = 0;                //Degug: Counts amount of iterations in ReceiveA() for loop, to ensure it completes the loop

void setup()
{
  Serial.begin(9600);
  Wire.begin(myAddress);        //I2C-address of this Uno is 1, can be changed 
  Wire.onReceive(receiveEvent); //Register event for incoming message

}

void loop()
{
  //Nothing to be done here
  delay(100);
}

//Event function for incoming messages. Unwritten Arduino rule for delays when receiving: delay(x) -> delay(1)
void receiveEvent(int nBytes)
{
  String inString = "";
  while (Wire.available())          //While something is available from the other Arduino
  {
    char c = Wire.read();           //Receive byte
    inString.concat(c);             //Add byte to incoming string
    }

    if(checkInputFormat(inString)){ //Only start decoding if input is in correct format
      decodeMessage(inString);      //Start decoding
    } else {
      Serial.print("\"");
      Serial.print(inString);
      Serial.println("\" is not in the correct format");
    }
}

//Check for correct input format of the incoming String
boolean checkInputFormat(String message){

  boolean cond0 = (message.length() >= 4);  //Smallest message (no params) has format: {XX}
  boolean cond1 = (message.indexOf("{")==0);
  boolean cond2 = (message.indexOf("{")==message.lastIndexOf("{"));
  boolean cond3 = (message.indexOf("}")==(message.length()-1));
  boolean cond4 = (message.indexOf("}")==message.lastIndexOf("}"));
  boolean cond5 = (isUpperCase(message.charAt(1)));
  boolean cond6 = (isUpperCase(message.charAt(2)));
  
  return(cond0 && cond1 && cond2 && cond3 && cond4 && cond5 && cond6);
}


/*********** FROM HERE ON THINGS ARE STILL VERY WORK IN PROGRESS! ***********/


//Decode the information from the incoming String
void decodeMessage(String message){
  message = message.substring(1,message.length()-1);       //Cut of '{' and '}'

  if (message.length()>0){
    
    Serial.print("Arduino Received message of type: ");
    Serial.println(message.substring(0,2));
    
    char nextChar = message.charAt(0);  //Select the first char
    message = message.substring(1);     //Remove first char from message

    
    if(isUpperCase(nextChar)){          //Only use if nextChar is indeed Upper Case
      switch (nextChar) {               //Determine message-type
         case 'A':
             //Serial.println("Arduino Received A");
             receivedA(message);
             break;
         case 'B':
             //Serial.println("Arduino Received B");
             break;
         case 'C':
             //Serial.println("Arduino Received C");
             break;
         default:
             Serial.println("Default Decoding");
      }
    }
  }
}


//Method to call when Arduino received an A
void receivedA(String remainderMessage){

  char nextChar = remainderMessage.charAt(0);       //Select the first char
  remainderMessage = remainderMessage.substring(1); //Remove first char from message
  Serial.print("receivedA() received parameters: ");
  Serial.println(remainderMessage);
    
    if(isUpperCase(nextChar)){
      switch (nextChar) {                           //Determine message-type
         case 'A':
             Serial.println("Normal order");
             //delay(10000); //test delay  (result: doesn't work when receiving message)
             long i;
             i = 0;
             if (counter == 0) {
               // test to see how Arduino's handle message flooding
               for (i = 0; i < 10000; i++) {//alternative test delay (result: works, 10000*delay(1))
                 delay(10); //is converted to delay(1) by Arduino
               }
             }
             counter++;
             Serial.println(i);
             Serial.println("End of Normal order");
             break;
         default:
             Serial.println("Default Received A");
      }
    }
    
}


