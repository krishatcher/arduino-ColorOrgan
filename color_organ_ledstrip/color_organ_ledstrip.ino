
#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

int analogPin=0;
int strobePin=2;
int resetPin=3;
int spectrumValue[7];
int filter=100;
int iteration = 0;
int marqueeOffset = 0;


void setup(){
  
  //Serial.begin(9600);

  strip.begin();
  strip.show();
  
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
}

void setColors(int bulbsInSet, int totalLEDs) {
  switch(bulbsInSet){
    case 7:
      seven(totalLEDs);
      break;
    case 4:
      four(totalLEDs);
      break;
    case 3:
      three(totalLEDs);
      break;
    case 2:
      two(totalLEDs);
      break;
    default:
      three(totalLEDs);
      break;
  }
}

void two(int totalLEDs) {
  for (int i = 0; i < totalLEDs; i++){
    if (i == 0 || (i % 2) == 0) {
      /* position 1 on strip - Red */
      strip.setPixelColor(i, spectrumValue[0], spectrumValue[1], spectrumValue[2]);
    } else {
      /* position 2 on strip - Blue */
      strip.setPixelColor(i, spectrumValue[6], spectrumValue[5], spectrumValue[4]);
    }
  }

  strip.show();
}

void three(int totalLEDs) {
  for (int i = 0; i < totalLEDs; i++){
    if (i == 0 || (i % 3) == 0) {
      /* position 1 on strip - Red */
      strip.setPixelColor(i, spectrumValue[4], spectrumValue[2], spectrumValue[0]);
    } else if (i == 1 || (i % 3) == 1) {
      /* position 2 on strip - Green */
      strip.setPixelColor(i, spectrumValue[1], spectrumValue[5], spectrumValue[3]);      
    } else {
      /* position 3 on strip - Blue */
      strip.setPixelColor(i, spectrumValue[2], spectrumValue[4], spectrumValue[6]);
    }
  }

  strip.show();
}

void four(int totalLEDs) {
  for (int i = 0; i < totalLEDs; i++){
    if (i == 0 || (i % 3) == 0) {
      /* position 1 on strip - Red */
      strip.setPixelColor(i, spectrumValue[1], 0, 0);
    } else if (i == 1 || (i % 3) == 1) {
      /* position 2 on strip - Green */
      strip.setPixelColor(i, 0, ((spectrumValue[2] + spectrumValue[3]) / 2), 0);      
    } else {
      /* position 3 on strip - Blue */
      strip.setPixelColor(i, 0, 0, ((spectrumValue[4] + spectrumValue[5] + spectrumValue[6]) / 3));
    }
  }

  strip.show();
}

void seven(int totalLEDs) {
  for (int i = 0; i < totalLEDs; i++){
    if (i == 0 || (i % 7) == 0) {
      /* position 1 on strip */
      strip.setPixelColor(i, spectrumValue[0], 0, 0);
    } else if (i == 1 || (i % 7) == 1) {
      /* position 2 on strip */
      strip.setPixelColor(i, 0, spectrumValue[1], 0);
    } else if (i == 2 || (i % 7) == 2) {
      /* position 3 on strip */
      strip.setPixelColor(i, 0, 0, spectrumValue[2]);
    } else if (i == 3 || (i % 7) == 3) {
      /* position 4 on strip */
      strip.setPixelColor(i, spectrumValue[3], 0, 0);
    } else if (i == 4 || (i % 7) == 4) {
      /* position 5 on strip */
      strip.setPixelColor(i, 0, spectrumValue[4], 0);
    } else if (i == 5 || (i % 7) == 5) {
      /* position 6 on strip */
      strip.setPixelColor(i, 0, 0, spectrumValue[5]);
    } else {
      /* position 7 on strip */
      strip.setPixelColor(i, spectrumValue[6], 0, 0);
    }
  }

  strip.show();
}

void threeMarquee(int totalLEDs) {
  if (iteration == 10) {
    /* ensure that we only change the offset once every 100 iterations, so that you can see the marquee effect */
    iteration = 0;

    if (marqueeOffset < totalLEDs) {
      /* we can only offset the count up to the total number of LEDs we're processing */
      marqueeOffset++;
    } else {
      marqueeOffset = 0;
    }
  }

  for (int i = 0; i < totalLEDs; i++){  
    int ledPosition = i + marqueeOffset;  

    if (ledPosition > totalLEDs) {
      ledPosition = ledPosition - totalLEDs;
    }
    
    if (ledPosition == 0 || (ledPosition % 3) == 0) {
      /* position 1 on strip - Red */
      strip.setPixelColor(ledPosition, spectrumValue[0], 0, 0);
    } else if (ledPosition == 1 || (ledPosition % 3) == 1) {
      /* position 2 on strip - Green */
      strip.setPixelColor(ledPosition, 0, ((spectrumValue[1]+spectrumValue[2])/2), 0);      
    } else {
      /* position 3 on strip - Blue */
      strip.setPixelColor(ledPosition, 0, 0, ((spectrumValue[3]+spectrumValue[4]+spectrumValue[5]+spectrumValue[6])/4));
    }
  }

  strip.show();

  iteration++;
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
    //Serial.print(spectrumValue[i]);
    //Serial.print(" ");
    digitalWrite(strobePin, HIGH);
  }

  //setColors(7, 60);
  threeMarquee(60);
}



