/////////////////////////////////////////////////////////
//
// Leds sign has 4 WS2811 strips
// each strip receives a sequence of animations that runs in paralel.

// Arduino Diecimila and Uno PWM pins: 3, 5, 6, 9, 10, and 11. 
// WS2812 strip pins
#define STRIP1_PWM 3
#define STRIP2_PWM 5
#define STRIP3_PWM 6
#define STRIP4_PWM 9

// potentiometer input
#define POTENTIOMETER_PIN 2

// Set the number of pixel in each strip
#define STRIP1_NUM_PIXELS 60
#define STRIP2_NUM_PIXELS 20
#define STRIP3_NUM_PIXELS 40
#define STRIP4_NUM_PIXELS 25

/////////////////////////////////////////////////////////
//
// animations
//
//  ALA_ON
//  ALA_OFF
//  ALA_BLINK
//  ALA_BLINKALT
//  ALA_SPARKLE
//  ALA_SPARKLE2
//  ALA_STROBO
//  
//  ALA_CYCLECOLORS
//  
//  ALA_PIXELSHIFTRIGHT
//  ALA_PIXELSHIFTLEFT
//  ALA_PIXELBOUNCE
//  ALA_PIXELSMOOTHSHIFTRIGHT
//  ALA_PIXELSMOOTHSHIFTLEFT
//  ALA_PIXELSMOOTHBOUNCE
//  ALA_COMET
//  ALA_COMETCOL
//  ALA_BARSHIFTRIGHT
//  ALA_BARSHIFTLEFT
//  ALA_MOVINGBARS
//  ALA_MOVINGGRADIENT
//  ALA_LARSONSCANNER
//  ALA_LARSONSCANNER2
//  
//  ALA_FADEIN
//  ALA_FADEOUT
//  ALA_FADEINOUT
//  ALA_GLOW
//  
//  ALA_FADECOLORS
//  ALA_FADECOLORSLOOP
//  ALA_PIXELSFADECOLORS
//  ALA_FLAME
//  
//  ALA_FIRE
//  ALA_BOUNCINGBALLS
//  ALA_BUBBLES
//  
//  ALA_ENDSEQ

/////////////////////////////////////////////////////////
//
// Palettes
//
// Empty palette:            alaPalNull
// Red,Green,Blue sequence:  alaPalRgb
// Rainbow colors:           alaPalRainbow
// Rainbow colors with alternating stripes of black:  alaPalRainbowStripe   alaPalParty   alaPalHeat
// Fire palette to be used with ALA_FIRE effect:      alaPalFire
//

#include "AlaLed.h"
#include "AlaLedRgb.h"

AlaLedRgb strip1;
AlaLedRgb strip2;
AlaLedRgb strip3;
AlaLedRgb strip4;

// custom palette :         black     white     black     red       green     blue
AlaColor mycolors_[6] = { 0x000000, 0xFFFFFF, 0x000000, 0xFF0000, 0x00FF00, 0x0000FF };
AlaPalette mycolors = { 6, mycolors_ };


AlaSeq seq1[] = {
  { ALA_CYCLECOLORS, 3000, 3000, alaPalRgb },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_FADECOLORSLOOP, 4000, 8000, alaPalRgb },
  { ALA_FADECOLORSLOOP, 500, 4000, mycolors },
  { ALA_FADECOLORS, 5000, 5000, mycolors },
  { ALA_ENDSEQ }
};

AlaSeq seq2[] = {
  { ALA_CYCLECOLORS, 3000, 3000, alaPalRgb },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_PIXELSMOOTHBOUNCE, 4000, 8000, alaPalRgb },
  { ALA_FADECOLORS, 4000, 4000, mycolors },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_ENDSEQ }
};

AlaSeq seq3[] = {
  { ALA_BLINKALT, 4000, 8000, alaPalRgb },
  { ALA_ENDSEQ }
};

AlaSeq seq4[] = {
  { ALA_PIXELSMOOTHBOUNCE, 3000, 3000, alaPalRgb },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_FADECOLORSLOOP, 4000, 8000, alaPalRgb },
  { ALA_CYCLECOLORS, 500, 4000, mycolors },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_FADECOLORS, 5000, 5000, mycolors },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_ENDSEQ }
};

void setup()
{
  // init strips PWM pins
  // initWS2812(int numLeds, byte pin, byte type)
  // default type = NEO_GRB + NEO_KHZ800 = 0x01 + 0x02
  strip1.initWS2812(STRIP1_NUM_PIXELS, STRIP1_PWM);
  strip2.initWS2812(STRIP2_NUM_PIXELS, STRIP2_PWM);
  //strip3.initWS2812(STRIP3_NUM_PIXELS, STRIP3_PWM);
  //strip4.initWS2812(STRIP4_NUM_PIXELS, STRIP4_PWM);
  
  // set strip sequences
  strip1.setAnimation(seq1);
  strip2.setAnimation(seq2);
  //strip3.setAnimation(seq3);
  //strip4.setAnimation(seq4);
}

void loop()
{
  // check potentiometer and set brightness
  //setBrightness();
  
  // advance animations of all strips
  strip1.runAnimation();
  strip2.runAnimation();
  //strip3.runAnimation();
  //strip4.runAnimation();
}

// check potentiometer and set brightness of all strips
void setBrightness()
{
  // read potentiometer value 
  int potVal = analogRead(POTENTIOMETER_PIN);
  // map potentiometer value to brightness value
  int brightVal = map(potVal, 0, 1023, 0, 254);
  
  strip1.setBrightness(brightVal);
  strip2.setBrightness(brightVal);
  strip3.setBrightness(brightVal);
  strip4.setBrightness(brightVal);
}

