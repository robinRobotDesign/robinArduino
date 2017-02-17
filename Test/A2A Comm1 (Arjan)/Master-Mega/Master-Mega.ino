/*Arduino Master Writer code for robin Robot Design
 *Written on 13-01-17 by Arjan Buyserd
 *
 *To be implemented in overall program by our TI-wizards, good luck!
 */

#include <Wire.h>

const int unoAddress1 = 1; //Address of first Uno, can be changed but also change in slave code
const int unoAddress2 = 2; //Address of second Uno, can be changed but also change in slave code

int messageCounter = 0; //Debug code: Is sent to UNO to check whether messages are skipped

void setup() {
  Wire.begin();                                 //Currently, the master has no address, can be implemented however
}

void loop() {
  //String exampleMsg = "{AAabcBBabc}";          //Just an example message, input is normally composed from other things
  String exampleMsg = "{AA";
  exampleMsg.concat(messageCounter);
  exampleMsg.concat("}");
  
  sendMsgToAddress(exampleMsg, unoAddress1);    //Use method to send the message to a slave
  //delay(2000);                                  //Wait before sending msg again
  
  exampleMsg = "{BBabcabc}";          //Just an example message, input is normally composed from other things
  sendMsgToAddress(exampleMsg, unoAddress1);    //Use method to send the message to a slave
  //delay(2000);                                  //Wait before sending msg again

  exampleMsg = "{CCabcabc}";          //Just an example message, input is normally composed from other things
  sendMsgToAddress(exampleMsg, unoAddress1);    //Use method to send the message to a slave
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

