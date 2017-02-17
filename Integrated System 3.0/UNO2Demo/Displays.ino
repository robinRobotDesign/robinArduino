/*
- Bestelling in behandeling. Een ogenblik geduld a.u.b.
- Bestel bij de balie en scan uw ticket hieronder.
- Ticket gescand. \n Uw bestelling: \n 1x koffie \n 2x chocolademelk
- Dit ticket is al gebruikt, doe een nieuwe bestelling bij de balie.
- Ongeldige ticket, doe een nieuwe bestelling bij de balie.
- Bestelling verwerken \n\n Bestelling verwerken. \n\n Bestelling verwerken.. \n\n Bestelling verwerken....
- Verwachtte tijd: xxx seconden.
- Wagentje verplaatst zich naar de bekerdispenser.
- Bekers worden uitgegeven.
- Koffie (Thee, etc) wordt ingeschonken.
- Fris wordt ingeschonken.
- Bestelling komt naar u toe.
- Geniet van uw drankje(s)!

*/

void prepareString(int lcdId, String string) { // "abc\n\nde"
  string.trim();
  
  int lineStart = 0;
  int lastSpace = 0;
  totalLines[lcdId] = 0;

  String prepared = "";

  for(int currentChar = 0; currentChar < string.length(); currentChar++) {
    //space
    if (string.charAt(currentChar) == ' ') {
      lastSpace = currentChar;
    }
    
    //enter
    if (string.charAt(currentChar) == '\n') {
      prepared += string.substring(lineStart, currentChar+1);
      lineStart = currentChar+1;
      totalLines[lcdId]++;
    }
    
    //line overflow
    if (currentChar-lineStart >= 16 && currentChar-lastSpace < 16) {
      prepared += string.substring(lineStart, lastSpace) + "\n";
      lineStart = lastSpace+1;
      totalLines[lcdId]++;
    }

    //word overflow
    if (currentChar-lineStart >= 16 && currentChar-lastSpace >= 16) {
      prepared += string.substring(lineStart, currentChar) + "\n";
      lineStart = currentChar+1;
      totalLines[lcdId]++;
    }
  }
  
  //last line
  prepared += string.substring(lineStart, string.length());
  totalLines[lcdId]++;


  
  lineTimers[lcdId] = millis();
  
  lcdStrings[lcdId] = prepared;
}



String getLine(int lcdId, int line) {
  
  int lineStart = 0;
  int lineEnd = 0;
  
  for (int i = 0; i < line; i++) {
    if(lineEnd > 0) {
      lineStart = lineEnd + 1;
    }
    int index = lcdStrings[lcdId].indexOf('\n', lineStart+1);
    
    if(index > 0 && lineEnd < lcdStrings[lcdId].length()) {
      lineEnd = index;
    } else {
      lineEnd = lcdStrings[lcdId].length();
    }
  }
  Serial.print(lcdId);
  Serial.print(" - ");
  Serial.println(lcdStrings[lcdId].substring(lineStart, lineEnd));
  return lcdStrings[lcdId].substring(lineStart, lineEnd);
}

void updateDisplay(int lcdId, int del) {
  //Serial.println("displayString");
  long currentTime = millis();
  lcd = lcds[lcdId];
  
  if (currentTime - lineTimers[lcdId] > currentLines[lcdId] * del) {
    if (totalLines[lcdId] == 0) {
      lcd.clear();
      lineTimers[lcdId] = millis();
      
      Serial.print(lcdId);
      Serial.print(" - ");
      Serial.println("clear");
    } else if (currentLines[lcdId] > totalLines[lcdId]) {
      lineTimers[lcdId] = millis();
      currentLines[lcdId] = 1;
      Serial.print(lcdId);
      Serial.print(" - ");
      Serial.println("repeat");
    } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(getLine(lcdId, currentLines[lcdId]));
  
      if (currentLines[lcdId]+1 <= totalLines[lcdId]) {
         lcd.setCursor(0,1);
         lcd.print(getLine(lcdId, currentLines[lcdId]+1));
      }
      currentLines[lcdId] = currentLines[lcdId] + 2;
      Serial.print(lcdId);
      Serial.print(" - ");
      Serial.println("update");
    }
  }
}


void updateDisplays() {
  updateDisplays(1000);
}

void updateDisplays(int del) {
  for (int i = 0; i < 4; i++) {
    updateDisplay(i, del);
  }
}


void displayAll(String text) {
  for (int i = 0; i < 4; i++) {
  Serial.print("Display all - ");
  Serial.println(text);
    displayText(i, text);
  }
}

void displayText(int lcdId, String text) {
  if (!lcdText[lcdId].equals(text)) {
    Serial.print("Display text - ");
    Serial.print(text);
    Serial.println(" hoihoi");
    /*char c[text.length()+1];
    text.toCharArray(c,text.length()+1);
    String s = String(c);*/
    Serial.println(lcdText[lcdID]);
    prepareString(lcdId, text);
    lcdText[lcdId] = text;
    Serial.print("Erna: ");
    Serial.println(text);
  }
  
}
