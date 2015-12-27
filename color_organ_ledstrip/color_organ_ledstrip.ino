
#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

int analogPin=0;
int strobePin=2;
int resetPin=3;
int spectrumValue[7];
int filter=90;
int iteration = 0;
int marqueeOffset = 0;
bool forward = true;
int color = 7;


void setup(){
  strip.begin();
  strip.show();
  
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
}

void marqueeMusic(int totalLEDs, int speedFactor) {
  if ((iteration % speedFactor) == 0) {
    marqueeOffset++;
  }

  int localOffset = (marqueeOffset % 3);
  
  for (int i = 0; i < totalLEDs; i++){
    //if (i > 2) {
    if (i == 0 || (i % 3) == 0) {
      /* position 1 on strip */
      if (i == 0 && localOffset > 0) {
        if (localOffset == 1) {
          // Blue
          strip.setPixelColor(i, 0, 0, ((spectrumValue[4]+spectrumValue[5]+spectrumValue[6])/3));
        } else {
          // Green
         strip.setPixelColor(i, 0, ((spectrumValue[1]+spectrumValue[3])/2), 0);
        }
      }

      //Red
      strip.setPixelColor(i + localOffset, ((spectrumValue[0]+spectrumValue[2])/2), 0, 0);
    } else if (i == 1 || (i % 3) == 1) {
      /* position 2 on strip */
      if (i == 1 && localOffset > 0) {
        if (localOffset == 1) {
          // Red
          strip.setPixelColor(i, ((spectrumValue[0]+spectrumValue[2])/2), 0, 0);
        } else {
          // Blue
          strip.setPixelColor(i, 0, 0, ((spectrumValue[4]+spectrumValue[5]+spectrumValue[6])/3));
        }
      }

      //Green
      strip.setPixelColor(i + localOffset, 0, ((spectrumValue[1]+spectrumValue[3])/2), 0);
    } else {
      /* position 3 on strip */
      if (i == 2 && localOffset > 0) {
        if (localOffset == 1) {
          // Green
          strip.setPixelColor(i, 0, ((spectrumValue[1]+spectrumValue[3])/2), 0);
        } else {
          // Red
          strip.setPixelColor(i, ((spectrumValue[0]+spectrumValue[2])/2), 0, 0);
        }
      }

      // Blue
      strip.setPixelColor(i + localOffset, 0, 0, ((spectrumValue[4]+spectrumValue[5]+spectrumValue[6])/3));
    }
  }

  strip.show();

  iteration++;
}

void bouncingRainbow() {
  if ((color % 7) == 0) {
    strip.setPixelColor(iteration, 255, 0, 0);
  } else if ((color % 7) == 1) {
    strip.setPixelColor(iteration, 255, 127, 0);
  } else if ((color % 7) == 2) {
    strip.setPixelColor(iteration, 255, 255, 0);
  } else if ((color % 7) == 3) {
    strip.setPixelColor(iteration, 0, 255, 0);
  } else if ((color % 7) == 4) {
    strip.setPixelColor(iteration, 0, 0, 255);
  } else if ((color % 7) == 5) {
    strip.setPixelColor(iteration, 75, 0, 130);
  } else {
    strip.setPixelColor(iteration, 139, 0, 255);
  }

  if (forward) {
    strip.setPixelColor(iteration - 1, 0, 0, 0);
  } else {
    strip.setPixelColor(iteration + 1, 0, 0, 0);
  }

  strip.show();
  
  if (iteration == 59) {
    forward = false;
    color++;
  } else if (iteration == 0 && color != 7) {
    forward = true;
    color++;
  }


  if (forward) {
    iteration++;
  } else {
    iteration--;
  }
}

void loop(){  
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  for (int i=0;i<7;i++){
    digitalWrite(strobePin, LOW);
    delay(2);
    spectrumValue[i]=analogRead(analogPin);
    spectrumValue[i]=constrain(spectrumValue[i], filter, 500);
    spectrumValue[i]=map(spectrumValue[i], filter,500,0,255);
    digitalWrite(strobePin, HIGH);
  }

  marqueeMusic(60, 5);
}



