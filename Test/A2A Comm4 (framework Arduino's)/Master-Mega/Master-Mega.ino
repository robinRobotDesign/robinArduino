/*Arduino Master Writer code for robin Robot Design
 *Written on 13-01-17 by Arjan Buyserd
 *
 *To be implemented in overall program by our TI-wizards, good luck!
 */

#include <Wire.h>

const int uno1Address = 1; //Address of first Uno, can be changed but also change in slave code
const int uno2Address = 2; //Address of second Uno, can be changed but also change in slave code

void setup() {
  Serial.begin(57600);
  Wire.begin();                                 //Currently, the master has no address, can be implemented however
}

void loop() {
/*  String uno1Answer = sendRequestToUno("GB", uno1Address);
  Serial.println(uno1Answer);
*/
  delay(2000);
  String str  = sendRequestToUno("ZCd", uno1Address);
  Serial.println(str);

  delay(2000);
  
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

int convertToID(char c) {
  return (int) (c - 97);
}

char convertToParam(int i) {
  return (char) (i + 97);
}

//Method used to send a message to another Arduino at a certain address
void sendMsgToAddress(String message, int currentAddress){

  char msg[message.length()+1];                 //Create new char array
  message.toCharArray(msg, message.length()+1); //Convert String to char-array to send message
  
  Wire.beginTransmission(currentAddress);       //Transmit to device with certain address
  Wire.write(msg);                              //Send message
  Wire.endTransmission();                       //Stop sending, message send
}


void sendMessageToUno(String message, int unoAddress) {
  String formattedMessage = "{";
  formattedMessage.concat(message);
  formattedMessage.concat("}");

  sendMsgToAddress(formattedMessage, unoAddress);
}



String sendRequestToUno(String message, int unoAddress) {
  //Send request message to let UNO know what it is expected to send
  sendMessageToUno(message, unoAddress);

  //Small delay to ensure right order of messaging
  delay(300);

  //Amount of bytes the MEGA expects to get back in UNO answer
  int bytes = 0;

  //If MEGA sends BD[a-d]: Check laser sensors (Main),
  //expect formattedAnswer {BE[a-b]}: LDRs are (not) working
  if (message.substring(0,2).equals("BD")) {
    bytes = 5;
    
  //If MEGA sends message CG[a-d]: Open doors, empty train (Main), 
  //expect formattedAnswer {CH[a-b]}: Train is (not) empty
  } else if (message.substring(0,2).equals("CG")) {
    bytes = 5;
    
  //If MEGA sends message CI: Close doors (Main), 
  //expect formattedAnswer {CJ[a-b]}: Doors are (not) closed
  } else if (message.substring(0,2).equals("CI")) {
    bytes = 5;
    
  //If MEGA sends CK[a-d]: Open doors (LDRs DEFECT) (Main),
  //expect formattedAnswer {CL[a-b]}: Train is (not) empty (LDRs DEFECT)
  } else if (message.substring(0,2).equals("CK")) {
    bytes = 5;
    
  //If MEGA sends message CM[a-d]: Close doors (LDRs DEFECT) (Main), 
  //expect formattedAnswer {CN[a-b]}: Doors are (not) closed (LDRs DEFECT)
  } else if (message.substring(0,2).equals("CM")) {
    bytes = 5;
    
  //If MEGA sends FD: Closing all doors (Start), 
  //expect formattedAnswer {FE[a-b]}: Doors are (not) closed
  } else if (message.substring(0,2).equals("FD")) {
    bytes = 5;
  
  //If MEGA sends FL[a-d]: Check laser sensors station (Start),
  //expect formattedAnswer {FM[a-b]}: LDRs are (not) working
  } else if (message.substring(0,2).equals("FL")) {
    bytes = 5;
  
  //If MEGA sends FN: Check train empty in station1 (Start), 
  //expect formattedAnswer {FO[a-b]}: Train is (not) empty
   } else if (message.substring(0,2).equals("FN")) {
    bytes = 5;
    
  //If MEGA sends GB: Train not empty, empty train in station1 (Start), 
  //expect formattedAnswer {GC[a-b]}: Train is (not) empty
  } else if (message.substring(0,2).equals("GB")) {
    bytes = 5;
    
  //If MEGA sends GD: Closing doors station1 (Start), 
  //expect formattedAnswer {GE[a-b]}: Doors are (not) closed
  } else if (message.substring(0,2).equals("GD")) {
    bytes = 5;

  //If MEGA sends GF: Opening doors (LDRs DEFECT) (Start)
  //expected formattedAnswer {GG[a-b]}: Train is (not) empty (LDRs DEFECT)
  } else if (message.substring(0,2).equals("GF")) {
    bytes = 5;

  //If MEGA sends GH: Closing doors (LDRs DEFECT) (Start)
  //expect formattedAnswer {GI[a-b]}: Doors are (not) closed (LDRs DEFECT)
  } else if (message.substring(0,2).equals("GH")) {
    bytes = 5;
    
  //If MEGA sends request ZA: ping, 
  //expect formattedAnswer {ZB}: pong
  } else if (message.substring(0,2).equals("ZA")) {
    bytes = 4;
    
  //If MEGA sends request ZC[a-z]: ping, 
  //expect formattedAnswer {ZD[a-z]}: pong pingID
  } else if (message.substring(0,2).equals("ZC")) {
    bytes = 5;
    
  }
  
  String answer = "";
  
  if (bytes > 0) {
    Wire.requestFrom(unoAddress, bytes);    // request x bytes from UNO
  
    String formattedAnswer = "";
    
    while (Wire.available()) { // slave may send less than requested
      char c = Wire.read(); // receive a byte as character
      formattedAnswer.concat(c);         // attach the character
    }

    if (checkInputFormat(formattedAnswer)) {
      answer = formattedAnswer.substring(1,formattedAnswer.length()-1);
      Serial.print("Request answered: ");
      Serial.println(answer);
    } else {
      Serial.print("Invalid format of answer: ");
      Serial.print(formattedAnswer);
      answer = "";
    }
  } else {
    Serial.print("Invalid request: ");
    Serial.print(message);
  }
  
  return answer;
}

