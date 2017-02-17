

void defaultPath() {
  pathLength = 0;
  
  for (int i = 0; i < NUMPIXELS; i++) {
    pathArray[i] = i;
    pathLength++;
  }
}

void createPath(int from, int to) {
  //Fill array with -1
  for (int i = 0; i < NUMPIXELS; i++) {
    pathArray[i] = -1;
  }

  int d = 1;
  
  if (from > to) {
    d = -1;
  }
  
  int j = from;
  pathLength = 0;
  
  while(j != to) {
    pathArray[pathLength] = j;
    
    if (to != TABLE_1 && j == TUNNEL_A && d == 1) {
      j = TABLE_1 + 1;
    } else if (to != TABLE_2 && j == TUNNEL_B && d == 1) {
      j = TABLE_2 + 1;
    } else if (j == TABLE_1 + 1 && d == -1) {
      j = TUNNEL_A;
    } else if (j == TABLE_2 + 1 && d == -1) {
      j = TUNNEL_B;
    } else {
      j += d;
    }
    
    pathLength++;
  }

  pathArray[pathLength] = j;
  pathLength++;
}

int map(int i) {
  return pathArray[i];
}

// ledTimer currentStep
void monochrome(int del, uint32_t color, boolean reverse) {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  
  //for(int k=0;k<pathLength;k++){
  
  long currentTime = millis();
  
  if (currentStep >= pathLength) {
    ledTimer = millis();
    currentStep = 0;
  } else if (currentTime - ledTimer > currentStep * del) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    int k = currentStep;
    int i;
    
    if (reverse == true) {
      i = pathLength - k;
    } else {
      i = k;
    }
    
    pixels.setPixelColor(map(i), color); // Moderately bright red color.
    
    pixels.show(); // This sends the updated pixel color to the hardware.

    currentStep++;
    //delay(del); // Delay for a period of time (in milliseconds).

  }
}




void test(int del, boolean reverse) {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  
  //for(int k=0;k<pathLength;k++){
  long currentTime = millis();
  
  if (currentStep >= pathLength) {
    ledTimer = millis();
    currentStep = 0;
  } else if (currentTime - ledTimer > currentStep * del) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    int k = currentStep;
    int i;
    
    if (reverse == true) {
      i = pathLength - k;
    } else {
      i = k;
    }
    
    if ((i+1) % 100 == 0) {
      pixels.setPixelColor(map(i), pixels.Color(150,0,0)); // Moderately bright red color.
    } else if ((i+1) % 50 == 0) {
      pixels.setPixelColor(map(i), pixels.Color(0,0,150)); // Moderately bright blue color.
    } else if ((i+1) % 10 == 0) {
      pixels.setPixelColor(map(i), pixels.Color(150,150,0)); // Moderately bright yellow color.
    } else {
      pixels.setPixelColor(map(i), pixels.Color(0,150,0)); // Moderately bright green color.
    }
    
    pixels.show(); // This sends the updated pixel color to the hardware.

    currentStep++;
    //delay(del); // Delay for a period of time (in milliseconds).

  }
}

void singleDataFlow(int del, int amount, boolean reverse) {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  
  //for(int k=0;k<pathLength+amount;k++){
  long currentTime = millis();
  
  if (currentStep >= pathLength+amount) {
    ledTimer = millis();
    currentStep = 0;
  } else if (currentTime - ledTimer > currentStep * del) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    int k = currentStep;
    int i;
    
    if (reverse == true) {
      i = pathLength - k;
    } else {
      i = k;
    }
    if (k < pathLength) {
      if ((i+1) % 100 == 0) {
        pixels.setPixelColor(map(i), pixels.Color(150,0,0)); // Moderately bright red color.
      } else if ((i+1) % 50 == 0) {
        pixels.setPixelColor(map(i), pixels.Color(0,0,150)); // Moderately bright blue color.
      } else if ((i+1) % 10 == 0) {
        pixels.setPixelColor(map(i), pixels.Color(150,150,0)); // Moderately bright yellow color.
      } else {
        pixels.setPixelColor(map(i), pixels.Color(0,150,0)); // Moderately bright green color.
      }
    }

    if (reverse) {
      pixels.setPixelColor(map((i+amount)%pathLength), pixels.Color(0,0,0));
    } else {
      pixels.setPixelColor(map((i-amount)%pathLength), pixels.Color(0,0,0));
    }
    pixels.show(); // This sends the updated pixel color to the hardware.

    currentStep++;
    //delay(del); // Delay for a period of time (in milliseconds).
  }
}

void doubleDataFlow(int del, int amount) {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  
  //for(int k=0;k<pathLength+amount;k++){
  long currentTime = millis();
  
  if (currentStep >= pathLength+amount) {
    ledTimer = millis();
    currentStep = 0;
  } else if (currentTime - ledTimer > currentStep * del) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    int k = currentStep;
    int i;
    
    i = pathLength - k;

    if (k < pathLength) {
      if ((i+1) % 100 == 0) {
        pixels.setPixelColor(map(i), pixels.Color(150,0,0)); // Moderately bright red color.
        pixels.setPixelColor(map(k), pixels.Color(150,0,0)); // Moderately bright red color.
      } else if ((i+1) % 50 == 0) {
        pixels.setPixelColor(map(i), pixels.Color(0,0,150)); // Moderately bright blue color.
        pixels.setPixelColor(map(k), pixels.Color(0,0,150)); // Moderately bright blue color.
      } else if ((i+1) % 10 == 0) {
        pixels.setPixelColor(map(i), pixels.Color(150,150,0)); // Moderately bright yellow color.
        pixels.setPixelColor(map(k), pixels.Color(150,150,0)); // Moderately bright yellow color.
      } else {
        pixels.setPixelColor(map(i), pixels.Color(0,150,0)); // Moderately bright green color.
        pixels.setPixelColor(map(k), pixels.Color(0,150,0)); // Moderately bright green color.
      }
    }
    
    pixels.setPixelColor(map((i+amount)%pathLength), pixels.Color(0,0,0));
    pixels.setPixelColor(map((k-amount)%pathLength), pixels.Color(0,0,0));
    
    pixels.show(); // This sends the updated pixel color to the hardware.

    currentStep++;
    //delay(del); // Delay for a period of time (in milliseconds).

  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t cycle(byte cyclePos) {
  cyclePos = 255 - cyclePos;
  if(cyclePos < 85) {
    return pixels.Color(255 - cyclePos * 3, cyclePos * 3, 0);
  }
  if(cyclePos < 170) {
    cyclePos -= 85;
    return pixels.Color(0, 255 - cyclePos * 3, cyclePos * 3);
  }
  cyclePos -= 170;
  return pixels.Color(cyclePos * 3, 0, 255 - cyclePos * 3);
}

void rainbow(int del, int delta) {
  uint16_t i, j;
  
  //for(j=0; j<256; j++) {
  long currentTime = millis();
  
  if (currentStep >= 256) {
    ledTimer = millis();
    currentStep = 0;
  } else if (currentTime - ledTimer > currentStep * del) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    int j = currentStep;
    
    for(i=0; i<pathLength; i++) {
      pixels.setPixelColor(map(i), cycle(delta*i + (j*2)));
    }
    
    pixels.show();

    currentStep++;
    //delay(del);
  }
}

void glow(int del, uint32_t color, int delta) {
  uint16_t i, j;

  uint8_t r = red(color);
  uint8_t g = green(color);
  uint8_t b = blue(color);

  uint8_t rNew, gNew, bNew;
  
  //for(j=0; j<256; j++) {
  long currentTime = millis();
  
  if (currentStep >= 256) {
    ledTimer = millis();
    currentStep = 0;
  } else if (currentTime - ledTimer > currentStep * del) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    int j = currentStep;
    
    for(i=0; i<pathLength; i++) {
      int k = (delta*i+(j*5))%255;
      if(k < 128) {
        rNew = 2*k*((float) r/255.0);
        gNew = 2*k*((float) g/255.0);
        bNew = 2*k*((float) b/255.0);
        pixels.setPixelColor(map(i), pixels.Color(rNew, gNew, bNew));
      } else {
        rNew = 2*(255-k)*((float) r/255.0);
        gNew = 2*(255-k)*((float) g/255.0);
        bNew = 2*(255-k)*((float) b/255.0);
        pixels.setPixelColor(map(i), pixels.Color(rNew, gNew, bNew));
      }
    }
    pixels.show();

    currentStep++;
    //delay(del);
  }
}

//Theatre-style crawling lights.
void multiDataFlow(int del, int spacing, int amount, uint32_t color) {
  
  //for (int k = 0; k < spacing; k++) {
  long currentTime = millis();
  
  if (currentStep >= spacing) {
    ledTimer = millis();
    currentStep = 0;
  } else if (currentTime - ledTimer > currentStep * 2*del) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    int k = currentStep;
    
    for (int i = 0; i < pathLength; i=i+spacing) {
      for (int j = 0; j < amount; j++) {
        if (j < spacing) {
          pixels.setPixelColor(map(i+j+k), color);    //turn every third pixel on
        }
      }
    }
    

    for (uint16_t i=0; i < pathLength; i=i+spacing) {
      if (i+k-1 > 0) {
        pixels.setPixelColor(map(i+k-1), 0);        //turn every third pixel off
      } else {
        pixels.setPixelColor(map(i+pathLength+k-1), 0);        //turn every third pixel off
      }
    }
    pixels.show();

    currentStep++;
  }
}

//Theatre-style crawling lights.
void xmas(int del, int spacing, int amount) {
  
  //for (int k = 0; k < spacing; k++) {
  long currentTime = millis();
  
  if (currentStep >= spacing) {
    ledTimer = millis();
    currentStep = 0;
  } else if (currentTime - ledTimer > currentStep * 2*del) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    int k = currentStep;
    
    for (int i = 0; i < pathLength; i=i+spacing) {
      for (int j = 0; j < amount; j++) {
        if (j < spacing) {
          pixels.setPixelColor(map(i+j+k), pixels.Color(255, 0, 0));    //turn every third pixel on
        }
      }
    }
    

    for (uint16_t i=0; i < pathLength; i=i+spacing) {
      if (i+k-1 > 0) {
        pixels.setPixelColor(map(i+k-1), pixels.Color(0, 255, 0));        //turn every third pixel off
      } else {
        //pixels.setPixelColor(map(i+pathLength+k-1), pixels.Color(0, 255, 0));        //turn every third pixel off
      }
    }
    pixels.show();

    currentStep++;
  }
}


uint8_t red(uint32_t c) {
  return (c >> 8);
}
uint8_t green(uint32_t c) {
  return (c >> 16);
}
uint8_t blue(uint32_t c) {
  return (c);
}

void setEffect(int effect) {
  if (ledEffect != effect) {
    currentStep = 0;
    pixels.clear();
    ledEffect = effect;
  }
  
}

void showEffect() {
  if (ledEffect == ORDERLESS) {
    glow(100, pixels.Color(0, 0, 255), 3);
  } else if (ledEffect == TEST) {
    test(100, false);
  } else if (ledEffect == SINGLEDATAFLOW) {
    singleDataFlow(100, 10, false);
  } else if (ledEffect == DOUBLEDATAFLOW) {
    doubleDataFlow(10, 10);
  } else if (ledEffect == MULTIDATAFLOW) {
    multiDataFlow(100, 5, 2, pixels.Color(255,127,0));
  } else if (ledEffect == RAINBOW) {
    rainbow(10, -10);
  } else if (ledEffect == GLOW) {
    glow(100, pixels.Color(255,255,255), 3);
  } else if (ledEffect == XMAS) {
    xmas(100, 180, 90);
  } else if (ledEffect == RED) {
    monochrome(10, pixels.Color(255,0,0), false);
  } else if (ledEffect == GREEN) {
    monochrome(10, pixels.Color(0,255,0), false);
  } else if (ledEffect == BLUE) {
    monochrome(10, pixels.Color(0,0,255), false);
  } else if (ledEffect == OFF) {
    monochrome(10, pixels.Color(0,0,0), false);
  }
}

