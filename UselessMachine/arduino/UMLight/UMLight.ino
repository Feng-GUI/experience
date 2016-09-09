////////////////////////////////////////////
//
// UselessMachine HAL9000
// TVOut light and microphone


////////////////////////////////////////////
//
// WS281x Leds
//
/* replaced with TV Out
#include <Adafruit_NeoPixel.h>
#define LEDS_DATA_PIN 8
#define NUMBER_OF_LEDS 20
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS, LEDS_DATA_PIN, NEO_GRB + NEO_KHZ800);
*/

////////////////////////////////////////////
//
// TV Out
//
#include <TVout.h>
TVout TV;
//NOTE: TVOut is ALWAYS using Pin 9 and Pin 7 on arduino Uno
static unsigned long tvFrameRate = 0;

////////////////////////////////////////////
//
// Microphone breakout
// Connect AUD to analog input pin
// Connect VCC to 3.3V (3.3V yields the best results)
//
#define MICROPHONE_PIN    0
    
void setup() {
  Serial.begin(9600);
  //If it prints less than 700, you can not add SD.h.  
  //SD.h has a 512 byte buffer for SD blocks and you will need about 200 for stack.
  freeMem();

  // init TV
  tvSetup();

  // init leds
  //ledsSetup();
  
}

void loop()
{
  // set and change red light brightness based on microphone input
  microphoneLoop();
}

void tvSetup() {
  TV.begin(PAL,120,96);
  
  //circles
  TV.clear_screen();
  TV.draw_circle(TV.hres()/2,TV.vres()/2,TV.vres()/2,WHITE);
  TV.delay(2000);
  TV.draw_circle(TV.hres()/2,TV.vres()/2,TV.vres()/3,WHITE);
  TV.delay(2000);
  TV.draw_circle(TV.hres()/2,TV.vres()/2,TV.vres()/4,WHITE);
  TV.delay(2000);
  //TV.draw_circle(TV.hres()/2,TV.vres()/2,TV.vres()/5,WHITE);
  //TV.delay(2000);
  //TV.draw_circle(TV.hres()/2,TV.vres()/2,TV.vres()/6,WHITE);
  //TV.delay(2000);
  //TV.draw_circle(TV.hres()/2,TV.vres()/2,TV.vres()/7,WHITE);
  //TV.delay(2000);

}

void microphoneLoop() {

  // read microphone 
  //unsigned int micAmplitude = analogRead(MICROPHONE_PIN);
  //TEST: print micAmplitude to serial
  
  //TODO: change leds brightness based on microphone input
  //TODO: scale micAmplitude from 0-1024 to brightness level 600-1024
  //setBrightness(uint8_t);

  tvFrameRate++;
  if (tvFrameRate > 10) {
    //TV.delay_frame(1);
    TV.clear_screen();
    uint8_t radius = TV.vres()/3; // default circle radius
    radius = TV.vres()/ random (2, 8);
    TV.draw_circle(TV.hres()/2, TV.vres()/2, radius, WHITE);
    
    tvFrameRate = 0;
  }
  
}

/*
void musicLoop()
{
  // if playback stopped, do not play more samples
  if (stopPlayback)
  {
    return;
  }
  
  stopPlayback = (musicPlayer.GPIO_digitalRead(2) == HIGH);
  if (stopPlayback)
  {
    Serial.println(F("playback stopped"));
  }
  
  // File is playing in the background
  if (musicPlayer.playingMusic == false) {
    // choose file to play
    int rndInt = random(0, 4); //random (min,max)
    Serial.print(F("musicLoop play file ")); Serial.println(myFILESs[rndInt]);
    if(!musicPlayer.startPlayingFile(myFILESs[rndInt]))
    {//startPlayingFile playFullFile
      Serial.println(F("failed to play"));
    }
  }
    
}
*/

uint16_t freeMem() {
  char top;
  extern char *__brkval;
  extern char __bss_end;
  Serial.println( __brkval ? &top - __brkval : &top - &__bss_end);
}

/* 
void ledsSetup() {
  strip.begin();
  
  // color all leds as red
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i , strip.Color(255, 0, 0));
  }
  strip.show();
}
*/

