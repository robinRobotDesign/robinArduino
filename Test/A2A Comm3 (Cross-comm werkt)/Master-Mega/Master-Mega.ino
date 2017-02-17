/*Arduino Master Writer code for robin Robot Design
 *Written on 13-01-17 by Arjan Buyserd
 *
 *To be implemented in overall program by our TI-wizards, good luck!
 */

#include <Wire.h>

const int uno1Address = 1; //Address of first Uno, can be changed but also change in slave code
const int uno2Address = 2; //Address of second Uno, can be changed but also change in slave code

int messageCounter = 0; //Debug code: Is sent to UNO to check whether messages are skipped

void setup() {
  Serial.begin(9600);
  Wire.begin();                                 //Currently, the master has no address, can be implemented however
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

  writeC();
  
  Serial.print("Message cycle: ");
  Serial.print(messageCounter);
  Serial.println();
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

//Write a message containing a C en request an answer
void writeC(){
  String exampleMsg = "{CCabcabc}";          //Just an example message, input is normally composed from other things
  sendMsgToAddress(exampleMsg, uno1Address);    //Use method to send the message to a slave
  
  
  Wire.requestFrom(uno1Address, 6);    // request 6 bytes from slave device #1

  Serial.print("Request answered: ");
  
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }

}

