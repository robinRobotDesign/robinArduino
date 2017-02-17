int convertToID(char c) {
  return (int) (c - 97);
}

char convertToParam(int i) {
  return (char) (i + 97);
}

//Event function for incoming messages. Unwritten Arduino rule for delays when receiving: delay(x) -> delay(1)
void receiveEvent(int nBytes){
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

void requestEvent() {
  Serial.println("Sending answer to request");
  String answer = "";
  boolean knownRequest = false;
  String messageType = lastMessage.substring(0,2);
  String params = lastMessage.substring(2,lastMessage.length());
  messageType.toUpperCase();
  params.toLowerCase();


  //If MEGA sends BD[a-d]: Check laser sensors (Main), answer BE[a-b]: LDRs are (not) working
  if(messageType.equals("BD")) {
    knownRequest = true;
    //TODO: UNO1: open doors, check sensors, close doors, answer
  //If MEGA sends CG[a-d]: Open doors (Main), answer CH[a-b]: Train is (not) empty
  } else if(messageType.equals("CG")) {
    knownRequest = true;
    //TODO: UNO1: open doors, check sensors, close doors, answer
  //If MEGA sends CI[a-d]: Close doors (Main), answer CJ[a-b]: Doors are (not) closed
  } else if(messageType.equals("CI")) {
    knownRequest = true;
    //TODO: UNO1: open doors, check sensors, close doors, answer
  //If MEGA sends CK[a-d]: Open doors (LDRs DEFECT) (Main), answer CL[a-b]: Train is (not) empty (LDRs DEFECT)
  } else if(messageType.equals("CK")) {
    knownRequest = true;
    //TODO: UNO1: open doors, check sensors, close doors, answer
  //If MEGA sends CM[a-d]: Close doors (LDRs DEFECT) (Main), answer CN[a-b]: Doors are (not) closed (LDRs DEFECT)
  } else if(messageType.equals("CM")) {
    knownRequest = true;
    //TODO: UNO1: open doors, check sensors, close doors, answer
    
  //If MEGA sends FD: Closing all doors (Start), answer FE[a-b]: Doors are (not) closed
  } else if(messageType.equals("FD")) {
    knownRequest = true;
    //TODO: UNO1: check sensors, close doors, answer
    
  //If MEGA sends FL[a-d]: Check laser sensors station (Start), answer FM[a-b]: LDRs are (not) working
  } else if(messageType.equals("FL")) {
    knownRequest = true;
    //TODO: UNO1: check sensors, close doors, answer
    
  //If MEGA sends FN: Check train empty in station1 (Start), answer FO[a-b]: Train is (not) empty
  } else if(messageType.equals("FN")) {
    knownRequest = true;
    //TODO: UNO1: check sensors, close doors, answer
    
  //If MEGA sends GB: Opening doors (Start), answer GC[a-b]: Train is (not) empty
  } else if(messageType.equals("GB")) {
    knownRequest = true;
    //TODO: UNO1: open doors, check sensors, close doors, answer
    
  //If MEGA sends GD: Closing doors station1 (Start), answer GE[a-b]: Doors are (not) closed
  } else if(messageType.equals("GD")) {
    knownRequest = true;
    //TODO: UNO1: open doors, check sensors, close doors, answer

  //If MEGA sends GF: Opening doors (LDRs DEFECT) (Start), answer GG[a-b]: Train is (not) empty (LDRs DEFECT)
  } else if(messageType.equals("GF")) {
    knownRequest = true;
    //TODO: UNO1: check sensors, close doors, answer
    
  //If MEGA sends GH: Closing doors (LDRs DEFECT) (Start), answer GI[a-b]: Doors are (not) closed (LDRs DEFECT)
  } else if(messageType.equals("GH")) {
    knownRequest = true;
    //TODO: UNO1: check sensors, close doors, answer
    
  //If MEGA sends request ZA: ping, answer ZB: pong
  } else if(messageType.equals("ZA")) {
    knownRequest = true;
    answer = "ZB";
    
  //If MEGA sends request ZC[a-z]: ping, answer ZD[a-z]: pong pingID
  } else if(messageType.equals("ZC")) {
    knownRequest = true;
    answer = "ZD";
    answer.concat(lastMessage.substring(2,3));
    
  } else {
    Serial.print("Incorrect request sent: ");
    Serial.println(messageType);
  }

  Serial.print("Answer: ");
  Serial.println(answer);

  //Prepare for sending
  String formattedAnswer = "{";
  formattedAnswer.concat(answer);
  formattedAnswer.concat("}");
  
  //Send answer to MEGA
  //if(formattedAnswer.length() > 2) {
    char charArray[formattedAnswer.length()+1];
    formattedAnswer.toCharArray(charArray, formattedAnswer.length()+1);
    Wire.write(charArray); // respond with message of x bytes, as expected by master
  //}
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

  //Save the message
  lastMessage = message;
  
  if (message.length()>0){
    
    Serial.print("Arduino Received message of type: ");
    Serial.println(message.substring(0,2));
    
    char nextChar = message.charAt(0);  //Select the first char
    message = message.substring(1);     //Remove first char from message

    
    if(isUpperCase(nextChar)){          //Only use if nextChar is indeed Upper Case
      switch (nextChar) {               //Determine message-type
        case 'A':
          decodeMainIdle(message);
          break;
        case 'B':
          decodeMainOrder(message);
          break;
        case 'C':
          decodeMainDescription(message);
          break;
        case 'D':
          decodeMainHelp(message);
          break;
        case 'E':
          decodeMainError(message);
          break;
        case 'F':
          Serial.println("decodeStartDescription");
          decodeStartDescription(message);
          break;
        case 'G':
          decodeStartHelp(message);
          break;
        case 'H':
          decodeStartError(message);
          break;
        case 'I':
          decodeDiagnosticsDescription(message);
          break;
        case 'J':
          decodeDiagnosticsHelp(message);
          break;
        case 'K':
          decodeDiagnosticsError(message);
          break;
        case 'Z':
          decodeCommTest(message);
          break;
        default:
             Serial.println("Default Decoding");
      }
    }
  }
}


//Decode all Main Idle messages (first letter of the message code is A)
void decodeMainIdle(String remainderMessage){
  char nextChar = remainderMessage.charAt(0);       //Select the first char
  String parameters = remainderMessage.substring(1); //Remove first char from message
  
  if(isUpperCase(nextChar)){
    switch (nextChar) {                           //Determine message-type
      //AA: No order, params: none
      case 'A':
        displayAll(F("Scan uw ticket hieronder"));
        setAllPatterns(DEFAULTPATTERN, -1);
        break;
      //AB[a-d]: Station out of order
      case 'B':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Station buiten gebruik"));
        break;
      default:
           Serial.println(msgDefault);
    }
  }
}


//Decode all Main Order messages (first letter of the message code is B)
void decodeMainOrder(String remainderMessage){
  char nextChar = remainderMessage.charAt(0);       //Select the first char
  String parameters = remainderMessage.substring(1); //Remove first char from message
  
  if(isUpperCase(nextChar)){
    switch (nextChar) {                           //Determine message-type
      //BA[a-d][a-l]+: Ticket scan succesful, params: tableID, n * drinkID
      case 'A':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Scan succesvol"));
                                                                     /********** Possible upgrade: display drinks on display ***********/
        break;
      //BB: Ticket already scanned, params: none
      case 'B':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Ticket is reeds gebruikt"));
        break;
      //BC: Ticket invalid, params: none
      case 'C':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Ticket is ongeldig"));
        break;
      //BD[a-d]: Checking LDRs station stationID, params: stationID
      case 'D':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Checking LDR's"));      
        break;
      //BE[a-b]: LDRs station stationID are (not) working, params: Confirmation
      case 'E':
        //Do nothing, is answer to request BD[a-d], answer sent by UNO1
        break;
      //BF[a-d][a-z]: Station stationID is (rank)th in queue, params: stationID, rank
      case 'F':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        d = parameters.charAt(1);
        rank = convertToID(d) + 1;

        msg = "In de wachtij, uw plek is ";
        msg.concat(rank);
        displayText(lcdID, msg);
        break;
      default:
           Serial.println(msgDefault);
    }
  }
}

//Decode all Main Description messages (first letter of the message code is C)
void decodeMainDescription(String remainderMessage){
  char nextChar = remainderMessage.charAt(0);       //Select the first char
  String parameters = remainderMessage.substring(1); //Remove first char from message
  
  if(isUpperCase(nextChar)){
    switch (nextChar) {                           //Determine message-type
      //CA[a-z][a-z]: Order part x out of y, params: x,y --------------------------- Werkt dit?
      case 'A':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        d = parameters.charAt(0);
        part = convertToID(d);
        e = parameters.charAt(0);
        total = convertToID(e);

        msg = "Bezig met deel ";
        msg.concat(part);
        msg = msg + " van de ";
        msg.concat(total);
        displayText(lcdID, msg); 
        
        break;
      //CB: Driving to cupDispenser, params: none
      case 'B':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Trein rijdt naar beker dispenser"));
        
        break;
      //CC[a-d]: Dispensing x cups, params: x
      case 'C':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Trein rijdt naar beker dispenser"));
        
        break;
      //CD[a-d]: Making soda, params: SodaID
      case 'D':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Frisdrank wordt bereidt"));
        
        break;
      //CE[a-l]: Making warm drinks, params: CoffeeID
      case 'E':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Koffie wordt bereidt"));
        
        break;
      //CF: Delivering order, params: none
      case 'F':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Bestelling wordt bezorgd"));
        
        break;
      //CG[a-d]: Doors open, empty train, params: StationID
      case 'G':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Neem uw bekers uit"));
        
        break;
      //CH[a-b]: Train is (not) empty, params: Confirmation
      case 'H':
        //Do nothing, is answer to request CG[a-d], answer sent by UNO1
        break;
      //CI[a-d]: Doors close, params: StationID
      case 'I':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Deuren sluiten"));
        
        break;
      //CJ[a-b]: Doors are (not) closed, params: Confirmation
      case 'J':
        //Do nothing, is answer to request CI[a-d], answer sent by UNO1
        break;
      //CK[a-d]: Doors open (LDRs DEFECT), params: StationID
      case 'K':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Neem uw bekers uit"));
        
        break;
      //CL[a-b]: Train is (not) empty (LDRs DEFECT), params: Confirmation
      case 'L':
        //Do nothing, is answer to request CK[a-d], answer sent by UNO1
        break;
      //CM[a-d]: Doors close (LDRs DEFECT), params: StationID
      case 'M':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        displayText(lcdID, F("Deuren sluiten"));
        break;
      //CN[a-b]: Doors are (not) closed (LDRs DEFECT), params: Confirmation
      case 'N':
        //Do nothing, is answer to request CM[a-d], answer sent by UNO1
        break;
      default:
           Serial.println(msgDefault);
    }
  }
}

//Decode all Main Help messages (first letter of the message code is D)
void decodeMainHelp(String remainderMessage){
  char nextChar = remainderMessage.charAt(0);       //Select the first char
  String parameters = remainderMessage.substring(1); //Remove first char from message
  
  if(isUpperCase(nextChar)){
    switch (nextChar) {                           //Determine message-type
      //DA: Train disconnected from track, params: none
      case 'A':
        displayAll(F("De trein is ontspoord"));
        break;
      //DB: Not enough cups in cupDispenser reservoir, params:none
      case 'B':
        displayAll(F("Niet genoeg bekers in dispenser"));
        break;
      //DC: Limit switch hit longer than 5 seconds, params: none
      case 'C':
        displayAll(F("Trein reageert niet meer"));
        break;
      default:
           Serial.println(msgDefault);
    }
  }
}

//Decode all Main Error messages (first letter of the message code is E)
void decodeMainError(String remainderMessage){
  char nextChar = remainderMessage.charAt(0);       //Select the first char
  String parameters = remainderMessage.substring(1); //Remove first char from message
  
  if(isUpperCase(nextChar)){
    switch (nextChar) {                           //Determine message-type
      //EA[a-b]: Switch SwitchID not working, params: SwitchID
      case 'A':
        c = parameters.charAt(0);
        switchID = convertToID(c);
        msg = "Wissel ";
        msg.concat(switchID);
        msg = msg + " werkt niet";
        displayAll(msg);
        break;
      //EB: CupDispenser not working, params: none
      case 'B':
        displayAll(F("Beker dispenser werkt niet"));        
        break;
      //EC: Coffee machine not working, params: none
      case 'C':
        displayAll(F("Koffie machine werkt niet")); 
        break;
      //ED[a-b]: Time-out internal communication Arduino UnoID, params: UnoID
      case 'D':
        c = parameters.charAt(0);
        unoID = convertToID(c);
        msg = "Communicatie met Uno-";
        msg.concat(unoID);
        msg = msg + " werkt niet";
        displayAll(msg);
        break;
      default:
           Serial.println(msgDefault);
    }
  }
}

//Decode all Start Description messages (first letter of the message code is F)
void decodeStartDescription(String remainderMessage){
  char nextChar = remainderMessage.charAt(0);       //Select the first char
  String parameters = remainderMessage.substring(1); //Remove first char from message
  
  if(isUpperCase(nextChar)){
    switch (nextChar) {                           //Determine message-type
      //FA: Starting... checking..., params: none
      case 'A':      
        Serial.println("Het syteem is bezig met opstarten");
        setAllPatterns(STARTUPPATTERN, -1);
        
        displayAll(F("Het systeem"));
        updateDisplays(1000);
        updatePatterns();
        break;
      //FB: Checking internal communication, params: none
      case 'B':
        displayAll(F("Interne communicatie controleren..."));
        break;
      //FC[a-b]: Internal Communication OK, params: none
      case 'C':
        displayAll(F("Interne communcatie gelukt"));
        break;
      //FD: Closing doors, params: none
      case 'D':
        displayAll(F("Deuren sluiten..."));
        break;
      //FE[a-b]: Doors closed, params: Confirmation
      case 'E':
        //Do nothing, is answer to request FD, answer sent by UNO1
        break;
      //FF: Determining position train, params: none
      case 'F':
        displayAll(F("Positie van trein bepalen..."));
        break;
      //FG: Position train determined, params: none
      case 'G':
        displayAll(F("Trein gepositioneerd"));
        break;
      //FH: Checking sensors between rails, params: none
      case 'H':
        displayAll(F("Sensoren controleren..."));
        break;
      //FI: Sensors between rails checked, params: none
      case 'I':
        displayAll(F("Sensoren controleren gelukt"));
        break;
      //FJ: Checking laser sensors cupDispenser, params: none
      case 'J':
        displayAll(F("Beker dispenser controleren..."));
        break;
      //FK: Laser sensors checked cupDispenser, params: none
      case 'K':
        displayAll(F("Beker dispenser controleren gelukt"));
        break;
      //FL[a-d]: Checking laser sensors station stationID, params: stationID
      case 'L':
        c = parameters.charAt(0);
        lcdID = convertToID(c);
        msg = "Station ";
        msg.concat(lcdID);
        msg += " controleren...";
        displayAll(msg);
        break;
      //FM[a-b]: Laser sensors are (not) working, params: Confirmation
      case 'M':
        //Do nothing, is answer to request FL, answer sent by UNO1        
        break;
      //FN: Checking whether train empty, params: none
      case 'N':
        displayAll(F("Controleren of de trein leeg is..."));
        break;
      //FM: Train is (not) empty, params: Confirmation
      case 'O':
        //Do nothing, is answer to request FN, answer sent by UNO1
        break;
      //FN: System works correctly, params: none
      case 'P':
        displayAll(F("Het systeem is opgestart"));
        break;
      default:
           Serial.println(msgDefault);
    }
  }
}

//Decode all Start Help messages (first letter of the message code is G)
void decodeStartHelp(String remainderMessage){
  char nextChar = remainderMessage.charAt(0);       //Select the first char
  String parameters = remainderMessage.substring(1); //Remove first char from message
  
  if(isUpperCase(nextChar)){
    switch (nextChar) {                           //Determine message-type
      //GA: Train disconnected from track, params: none
      case 'A':
        displayAll(F("De trein kan niet gevonden worden"));
        break;
      //GB: Empty train, take cups out of station1, params: none
      case 'B':
        displayAll(F("De trein is niet leeg, bekers s.v.p. uitnemen"));
        break;
      //GC[a-b]: Train empty, params: Confirmation
      case 'C':
        //Do nothing, is answer to request GB, answer sent by UNO1
        break;
      //GD: Doors station1 are closing, params: none
      case 'D':
        displayText(0,F("Deuren sluiten"));
        break;
      //GE[a-b]: Doors closed, params: Confirmation
      case 'E':
        //Do nothing, is answer to request GD, answer sent by UNO1
        break;
      //GF: Empty train, take cups out of station1 (LDRs DEFECT), params: none
      case 'F':
        displayAll(F("De trein is niet leeg, bekers s.v.p. uitnemen"));        
        break;
      //GG[a-b]: Train empty (LDRs DEFECT), params: Confirmation
      case 'G':
        //Do nothing, is answer to request GF, answer sent by UNO1
        break;
      //GH: Doors station1 are closing (LDRs DEFECT), params: none
      case 'H':
        displayText(0,F("Deuren sluiten"));        
        break;
      //GI[a-b]: Doors closed (LDRs DEFECT), params: Confirmation
      case 'I':
        //Do nothing, is answer to request GH, answer sent by UNO1
        break;
      //GJ: Empty coffee residu bin, params: none
      case 'J':
        displayAll(F("Koffieresidu is vol"));
        break;
      //GK: Coffee residu emptied, params: none
      case 'K':
        displayAll(F("Koffieresidu is geleegd"));
        break;
      default:
           Serial.println(msgDefault);
    }
  }
}

//Decode all Start Error messages (first letter of the message code is H)
void decodeStartError(String remainderMessage){
  char nextChar = remainderMessage.charAt(0);       //Select the first char
  String parameters = remainderMessage.substring(1); //Remove first char from message
  
  if(isUpperCase(nextChar)){
    switch (nextChar) {                           //Determine message-type
      //HA[a-b]+: Internal communication Arduino UnoID1 (and UnoID2) failed, params: UnoID+
      case 'A':
        c = parameters.charAt(0);
        unoID = convertToID(c);
        msg = "Interne commmunicatie tussen Uno ";
        msg.concat(unoID);
        msg += " mislukt";
        displayAll(msg);
        break;
      //HB[a-b]: Time-out internal communication UnoID, params: UnoID
      case 'B':
        c = parameters.charAt(0);
        unoID = convertToID(c);
        msg = "Time-out intere communicatie Uno ";
        msg.concat(unoID);
        displayAll(msg);
        break;
      //HC[a-z]+: Defect Hall sensors, params: HallID+
      case 'C':
        c = parameters.charAt(0);
        sensorID = convertToID(c);
        msg = "Hall-effect sensor ";
        msg.concat(sensorID);
        msg += " is defect";
        displayAll(msg);
        break;
      //HD[a-j]+: Defect Laser sensors cupDispenser, params: LdrID+
      case 'D':
        c = parameters.charAt(0);
        sensorID = convertToID(c);
        msg = "LDR sensor ";
        msg.concat(sensorID);
        msg += " van de beker dispenser is defect";
        displayAll(msg);
        break;
      default:
           Serial.println(msgDefault);
    }
  }
}

//Decode all Diagnostics Description messages (first letter of the message code is I)
void decodeDiagnosticsDescription(String remainderMessage){
  char nextChar = remainderMessage.charAt(0);       //Select the first char
  String parameters = remainderMessage.substring(1); //Remove first char from message
  
  if(isUpperCase(nextChar)){
    switch (nextChar) {                           //Determine message-type
      default:
           Serial.println(msgDefault);
    }
  }
}

//Decode all Diagnostics Help messages (first letter of the message code is J)
void decodeDiagnosticsHelp(String remainderMessage){
  char nextChar = remainderMessage.charAt(0);       //Select the first char
  String parameters = remainderMessage.substring(1); //Remove first char from message
  
  if(isUpperCase(nextChar)){
    switch (nextChar) {                           //Determine message-type
      default:
           Serial.println(msgDefault);
    }
  }
}

//Decode all Diagnostics Error messages (first letter of the message code is K)
void decodeDiagnosticsError(String remainderMessage){
  char nextChar = remainderMessage.charAt(0);       //Select the first char
  String parameters = remainderMessage.substring(1); //Remove first char from message
  
  if(isUpperCase(nextChar)){
    switch (nextChar) {                           //Determine message-type
      default:
           Serial.println(msgDefault);
    }
  }
}

//Decode all Communication test messages (first letter of the message code is Z)
void decodeCommTest(String remainderMessage){
  char nextChar = remainderMessage.charAt(0);       //Select the first char
  String parameters = remainderMessage.substring(1); //Remove first char from message
  
  if(isUpperCase(nextChar)){
    switch (nextChar) {                           //Determine message-type
      //ZA: MEGA sends ping, params: 0
      case 'A':
        
        break;
      //ZB: Uno sends pong, params: 0
      case 'B':
        //Do nothing, is answer to request ZA, answer sent by UNO1 and UNO2
        break;
      //ZC[a-z]: MEGA sends ping with pingID, params: pingID
      case 'C':
        
        break;
      //ZD[a-z]: Uno sends pong with same pingID as request, params: pingID
      case 'D':
        //Do nothing, is answer to request ZC[a-z], answer sent by UNO1 and UNO2
        break;
      default:
           Serial.println(msgDefault);
    }
  }
}



