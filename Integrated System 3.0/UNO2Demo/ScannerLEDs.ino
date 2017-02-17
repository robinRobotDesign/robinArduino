

void setAllPatterns(int pattern, int repeat) {
  for (int i=0; i < 4; i++) {
    setPattern(i, pattern, repeat);
    
  }
}

void setPattern(int stationID, int pattern, int repeat) {
  if (!(pattern == patterns[stationID] && repeat == repeats[stationID]) ){
    patternStates[stationID] = 0;
    repeats[stationID] = repeat;
    patterns[stationID] = pattern;
  }
}

void updatePatterns() {
  for (int i = 0; i < 4; i++) {
    updatePattern(i);
  }
}

void updatePattern(int stationID) {
  if (repeats[stationID] == 0) {
    setPattern(stationID, DEFAULTPATTERN, -1);
  } else {
    if (patterns[stationID] == ONPATTERN) {
      onPattern(stationID, 1000);
    } else if (patterns[stationID] == OFFPATTERN) {
      offPattern(stationID, 1000);
    } else if (patterns[stationID] == DEFAULTPATTERN) {
      defaultPattern(stationID, 500);
    } else if (patterns[stationID] == ERRORPATTERN) {
      errorPattern(stationID, 200);
    } else if (patterns[stationID] == CORRECTPATTERN) {
      correctPattern(stationID, 50);
    } else if (patterns[stationID] == STARTUPPATTERN) {
      errorPattern(stationID, 1000);
    }
    
  }
}

void onPattern(int stationID, int delayVal) {
  if (arrowTimers[stationID]+delayVal < millis()) {
    if (patternStates[stationID] == 0) {
      digitalWrite(stationArrows[stationID][0], HIGH);
      digitalWrite(stationArrows[stationID][1], HIGH);
      digitalWrite(stationArrows[stationID][2], HIGH);
      arrowTimers[stationID] = millis();
    }
  }
}

void offPattern(int stationID, int delayVal) {
  if (arrowTimers[stationID]+delayVal < millis()) {
    if (patternStates[stationID] == 0) {
      digitalWrite(stationArrows[stationID][0], LOW);
      digitalWrite(stationArrows[stationID][1], LOW);
      digitalWrite(stationArrows[stationID][2], LOW);
      arrowTimers[stationID] = millis();
    }
  }
}

void defaultPattern(int stationID, int delayVal) {
  if (arrowTimers[stationID]+delayVal < millis()) {
    /*Serial.print("Times: ");
    Serial.print(arrowTimers[stationID]);
    Serial.print(" - ");
    Serial.println(arrowTimers[stationID]);*/
    if (patternStates[stationID] == 0) {
      digitalWrite(stationArrows[stationID][0], HIGH);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    } else if (patternStates[stationID] == 1) {
      digitalWrite(stationArrows[stationID][1], HIGH);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    } else if (patternStates[stationID] == 2) {
      digitalWrite(stationArrows[stationID][2], HIGH);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    } else if (patternStates[stationID] == 3) {
      digitalWrite(stationArrows[stationID][0], LOW);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    } else if (patternStates[stationID] == 4) {
      digitalWrite(stationArrows[stationID][1], LOW);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    } else if (patternStates[stationID] == 5) {
      digitalWrite(stationArrows[stationID][2], LOW);
      patternStates[stationID] = 0;
      arrowTimers[stationID] = millis();

      if (repeats[stationID] > 0) {
        repeats[stationID]--;
      }
    }
  }
  
}

void errorPattern(int stationID, int delayVal) {
  if (arrowTimers[stationID]+delayVal < millis()) {
    if (patternStates[stationID] == 0) {
      digitalWrite(stationArrows[stationID][0], HIGH);
      digitalWrite(stationArrows[stationID][1], HIGH);
      digitalWrite(stationArrows[stationID][2], HIGH);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    } else if (patternStates[stationID] == 1) {
      digitalWrite(stationArrows[stationID][0], LOW);
      digitalWrite(stationArrows[stationID][1], LOW);
      digitalWrite(stationArrows[stationID][2], LOW);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    } else if (patternStates[stationID] == 2) {
      digitalWrite(stationArrows[stationID][0], HIGH);
      digitalWrite(stationArrows[stationID][1], HIGH);
      digitalWrite(stationArrows[stationID][2], HIGH);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    } else if (patternStates[stationID] == 3) {
      digitalWrite(stationArrows[stationID][0], LOW);
      digitalWrite(stationArrows[stationID][1], LOW);
      digitalWrite(stationArrows[stationID][2], LOW);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    } else if (patternStates[stationID] == 4) {
      digitalWrite(stationArrows[stationID][0], HIGH);
      digitalWrite(stationArrows[stationID][1], HIGH);
      digitalWrite(stationArrows[stationID][2], HIGH);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    } else if (patternStates[stationID] == 5) {
      digitalWrite(stationArrows[stationID][0], LOW);
      digitalWrite(stationArrows[stationID][1], LOW);
      digitalWrite(stationArrows[stationID][2], LOW);
      patternStates[stationID] = 0;
      arrowTimers[stationID] = millis();

      if (repeats[stationID] > 0) {
        repeats[stationID]--;
      }
    }
  }
}

void correctPattern(int stationID, int delayVal) {
  if (arrowTimers[stationID]+delayVal < millis()) {
    if (patternStates[stationID] == 0) {
      digitalWrite(stationArrows[stationID][0], HIGH);
      digitalWrite(stationArrows[stationID][1], HIGH);
      digitalWrite(stationArrows[stationID][2], HIGH);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    } else if (patternStates[stationID] == 1) {
      digitalWrite(stationArrows[stationID][0], LOW);
      digitalWrite(stationArrows[stationID][1], LOW);
      digitalWrite(stationArrows[stationID][2], LOW);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    } else if (patternStates[stationID] == 2) {
      digitalWrite(stationArrows[stationID][0], HIGH);
      digitalWrite(stationArrows[stationID][1], HIGH);
      digitalWrite(stationArrows[stationID][2], HIGH);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    } else if (patternStates[stationID] == 3) {
      digitalWrite(stationArrows[stationID][0], LOW);
      digitalWrite(stationArrows[stationID][1], LOW);
      digitalWrite(stationArrows[stationID][2], LOW);
      patternStates[stationID]++;
      arrowTimers[stationID] = millis();
    }
  }

  if (arrowTimers[stationID]+2000 < millis()) {
    if (patternStates[stationID] == 4) {
      digitalWrite(stationArrows[stationID][0], LOW);
      digitalWrite(stationArrows[stationID][1], LOW);
      digitalWrite(stationArrows[stationID][2], LOW);
      patternStates[stationID] = 0;
      arrowTimers[stationID] = millis();

      if (repeats[stationID] > 0) {
        repeats[stationID]--;
      }
    }
  }
}

