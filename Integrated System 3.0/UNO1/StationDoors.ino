
boolean isAllOpen() {
  return isOpen[0] && isOpen[1] && isOpen[2] && isOpen[3];
}

boolean isAllClosed() {
  return isClosed[0] && isClosed[1] && isClosed[2] && isClosed[3];
}

boolean isAllEmptied() {
  return isEmptied[0] && isEmptied[1] && isEmptied[2] && isEmptied[3];
}

boolean isAllUnobstructed() {
  return isUnobstructed[0] && isUnobstructed[1] && isUnobstructed[2] && isUnobstructed[3];
}

void updateAllDoors() {
  for (int i = 0; i < 4; i++) {
    updateDoors(i);
  }
}

void updateDoors(int stationID) {
  if (state[stationID] == STOP) {
    //Main state, rest
  } else if (state[stationID] == OPEN) {
    //openDoors(stationID);
    openDoorsDefect(stationID);
  } else if (state[stationID] == CLOSE) {
    //closeDoors(stationID);
    closeDoorsDefect(stationID);
  } else if (state[stationID] == OPENDEFECT) {
    openDoorsDefect(stationID);
  } else if (state[stationID] == CLOSEDEFECT) {
    closeDoorsDefect(stationID);
  } else if (state[stationID] == CLOSEALL) {
    closeAllDoorsDefect();
  } else if (state[stationID] == CLOSEALLDEFECT) {
    closeAllDoorsDefect();
  } else if (state[stationID] == CHECKLDRS) {
    //This is checked automaticly
  }
}

void setAllState(int stateID) {
  for (int i = 0; i < 4; i++) {
    setState(i, stateID);
  }
}

void setState(int stationID, int stateID) {
  state[stationID] = stateID;
  Serial.println(stateID);
}

/* Opens doors of corresponding station */
void openDoors(int stationID) {
  openDoors(stationID, 25);
}

  /* Opens doors of corresponding station */
void openDoorsDefect(int stationID) {
  if (!isOpen[stationID]) {
    openDoors(stationID, 50);
  } else {
    //int wait = 15000;
    int wait = 20000;
    if (servoTimers[stationID]+wait <= millis()) {
      isEmptied[stationID] = true;
      isUnobstructed[stationID] = true;
    }  
  }
}


/* Opens doors of corresponding station */
void openDoors(int stationID, int delayVal) {
  
  if (servoTimers[stationID]+delayVal <= millis()) {
    if (servoPos[stationID] <= endPos) {
      //Serial.print("Open: ");
      //Serial.println(servoPos[stationID]);
      isOpen[stationID] = true;
    } else if (servoPos[stationID] > endPos) {
      servoPos[stationID]--;
      isClosed[stationID] = false;
      //Serial.print("Opening: ");
      //Serial.println(servoPos[stationID]);
      servos[stationID].write(servoPos[stationID]);
      
    }
    servoTimers[stationID] = millis();
  }
}

void updateAllLDRs() {
  for (int i = 0; i < 4; i++) {
    updateLDRs(i);
  }
}
void updateLDRs(int stationID) {
  if (state[stationID] != OPENDEFECT && state[stationID] != CLOSEDEFECT) {
    updateEmptied(stationID);
    //updateUnobstructed(stationID);
    isUnobstructed[stationID] = true;
  }
}

/* Dynamic delay that lasts until the train is empty */
void updateEmptied(int stationID) {
  int delayVal = 1;
  
  //Wait for empty train for at least n ms
  if (emptyTimers[stationID]+delayVal <= millis()) {
    if (emptyCounters[stationID] < emptyThreshold) {
      if (digitalRead(bottomLDR[stationID]) == HIGH) {
        emptyCounters[stationID]++;
      } else {
        emptyCounters[stationID] = 0;
        isEmptied[stationID] = false;
      }
    } else {
      emptyCounters[stationID] = 0;
      isEmptied[stationID] = true;
    }
    emptyTimers[stationID] = millis();
  }
}



/* Dynamic delay that lasts until the doors are free to close */
void updateUnobstructed(int stationID) {
  int delayVal = 1;
  
  //Wait for empty train for at least n ms
  if (obstructionTimers[stationID]+delayVal <= millis()) {
    if (obstructionCounters[stationID] < obstructionThreshold) {
      if (digitalRead(bottomLDR[stationID]) == HIGH) {
        //Serial.println(digitalRead(bottomLDR[stationID]));
        obstructionCounters[stationID]++;
      } else {
        obstructionCounters[stationID] = 0;
        isUnobstructed[stationID] = false;
      }
    } else {
      obstructionCounters[stationID] = 0;
      isUnobstructed[stationID] = true;
    }
    obstructionTimers[stationID] = millis();
  }
}

/* Closes doors of corresponding station */
void closeDoors(int stationID) {
  updateLDRs(stationID);
  closeDoors(stationID, 25);
}

/* Closes doors of corresponding station */
void closeDoorsDefect(int stationID) {
  isEmptied[stationID] = true;
  isUnobstructed[stationID] = true;
  closeDoors(stationID, 50);
}

/* Closes doors of corresponding station */
void closeDoors(int stationID, int delayVal) {
  
  if (servoTimers[stationID]+delayVal <= millis()) {
    if (servoPos[stationID] >= startPos) {
      isClosed[stationID] = true;
    } else if (servoPos[stationID] < startPos && isEmptied[stationID] == true
               && isUnobstructed[stationID] == true) {
      servoPos[stationID]++;
      servos[stationID].write(servoPos[stationID]);
      Serial.println(servoPos[stationID]);
      isOpen[stationID] = false;
    } else if ((isEmptied[stationID] == false || isUnobstructed[stationID] == false)
               && servoPos[stationID] > endPos) {
      servoPos[stationID]--;
      servos[stationID].write(servoPos[stationID]);
      Serial.println(servoPos[stationID]);
    }
    servoTimers[stationID] = millis();
  }
}

/* Closes doors simultaneously, while checking for cups and obstructions */
void closeAllDoors() {
  for(int i = 0; i < 4; i++) {
    closeDoors(i);
  }
}

void closeAllDoorsDefect() {
  for(int i = 0; i < 4; i++) {
    closeDoorsDefect(i);
  }
}

