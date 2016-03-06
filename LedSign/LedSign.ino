/////////////////////////////////////////////////////////
//
// Leds sign has 4 WS2811 strips
// each strip receives a sequence of animations that runs in paralel.

// Arduino Diecimila and Uno PWM: 3, 5, 6, 9, 10, and 11. 
//



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

// custom palette :         black     white     black     red       green     blue
AlaColor mycolors_[6] = { 0x000000, 0xFFFFFF, 0x000000, 0xFF0000, 0x00FF00, 0x0000FF };
AlaPalette mycolors = { 6, mycolors_ };


AlaSeq seq1[] = {
  { ALA_CYCLECOLORS, 3000, 3000, alaPalRgb },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_FADECOLORSLOOP, 4000, 8000, alaPalRgb },
  { ALA_FADECOLORSLOOP, 500, 4000, mycolors },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_FADECOLORS, 5000, 5000, mycolors },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_ENDSEQ }
};

AlaSeq seq2[] = {
  { ALA_CYCLECOLORS, 3000, 3000, alaPalRgb },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_FADECOLORSLOOP, 4000, 8000, alaPalRgb },
  { ALA_FADECOLORSLOOP, 500, 4000, mycolors },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_FADECOLORS, 5000, 5000, mycolors },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_ENDSEQ }
};

AlaSeq seq3[] = {
  { ALA_CYCLECOLORS, 3000, 3000, alaPalRgb },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_FADECOLORSLOOP, 4000, 8000, alaPalRgb },
  { ALA_FADECOLORSLOOP, 500, 4000, mycolors },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_FADECOLORS, 5000, 5000, mycolors },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_ENDSEQ }
};

AlaSeq seq4[] = {
  { ALA_CYCLECOLORS, 3000, 3000, alaPalRgb },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_FADECOLORSLOOP, 4000, 8000, alaPalRgb },
  { ALA_FADECOLORSLOOP, 500, 4000, mycolors },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_FADECOLORS, 5000, 5000, mycolors },
  { ALA_OFF, 1000, 1000, alaPalNull },
  { ALA_ENDSEQ }
};

void setup()
{
  strip1.initWS2812(8, 2);
  strip2.initWS2812(8, 2);
  strip3.initWS2812(8, 2);
  strip4.initWS2812(8, 2);
      
  strip1.setAnimation(seq1);
  strip2.setAnimation(seq2);
  strip3.setAnimation(seq3);
  strip4.setAnimation(seq4);
}

void loop()
{
  strip1.runAnimation();
}

void setBrightness()
{
  // FIXME: get potentiometer value 
  
  strip1.setBrightness();
  strip2.setBrightness();
  strip3.setBrightness();
  strip4.setBrightness();
}

