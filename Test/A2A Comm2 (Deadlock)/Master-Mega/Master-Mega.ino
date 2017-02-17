/*Arduino Master Writer code for robin Robot Design
 *Written on 13-01-17 by Arjan Buyserd
 *
 *To be implemented in overall program by our TI-wizards, good luck!
 */

#include <Wire.h>

const int megaAddress = 0; //Address of the Mega, can be changed but also change in slave code
const int uno1Address = 1; //Address of first Uno, can be changed but also change in slave code
const int uno2Address = 2; //Address of second Uno, can be changed but also change in slave code

int messageCounter = 0; //Debug code: Is sent to UNO to check whether messages are skipped

void setup() {
  Serial.begin(9600);
  Wire.begin(megaAddress);      //Currently, the master has no address, can be implemented however
  Wire.onReceive(receiveEvent); //Register event for incoming message
}

void loop() {
  //String exampleMsg = "{AAabcBBabc}";          //Just an example message, input is normally composed from other things
  String exampleMsg = "{AA";
  exampleMsg.concat(messageCounter);
  exampleMsg.concat("}");
  
  sendMsgToAddress(exampleMsg, uno1Address);    //Use method to send the message to a slave
  //delay(2000);                                  //Wait before sending msg again
  
  exampleMsg = "{BBabcabc}";          //Just an example message, input is normally composed from other things
  sendMsgToAddress(exampleMsg, uno1Address);    //Use method to send the message to a slave
  //delay(2000);                                  //Wait before sending msg again

  exampleMsg = "{CCabcabc}";          //Just an example message, input is normally composed from other things
  sendMsgToAddress(exampleMsg, uno1Address);    //Use method to send the message to a slave
  delay(2000);                                  //Wait before sending msg again

  messageCounter++;
}

//Method used to send a message to another Arduino at a certain address
void sendMsgToAddress(String message, int currentAddress){

  char msg[message.length()+1];                 //Create new char array
  message.toCharArray(msg, message.length()+1); //Convert String to char-array to send message
  
  Wire.beginTransmission(currentAddress);       //Transmit to device with certain address
  Wire.write(msg);                              //Send message
  Wire.endTransmission();                       //Stop sending, message send
}



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

void decodeMessage(String message){
  message = message.substring(1,message.length()-1);       //Cut of '{' and '}'

  if (message.length()>0){
    
    Serial.print("Arduino Received message of type: ");
    Serial.println(message.substring(0,2));
    Serial.println(message);
    char nextChar = message.charAt(0);  //Select the first char
    message = message.substring(1);     //Remove first char from message

    
    if(isUpperCase(nextChar)){          //Only use if nextChar is indeed Upper Case
      switch (nextChar) {               //Determine message-type
         case 'A':
             //Serial.println("Arduino Received A");
             //receivedA(message);
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

