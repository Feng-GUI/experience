////////////////////////////////////////////
//
// UselessMachine HAL9000
//
// Arduino Uno + Music Shield
// 2 Servos, WS2812 Leds, Microphone
//
//

////////////////////////////////////////////
//
// music playback
//
// Music Shield VS1053 Codec Breakout: Pins 6, 7, 8, 9 and the 6 analog in pins 
// (also known as digital i/o pins 14-20) are available.

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

boolean stopPlayback = false;
// These are the pins used for the music maker shield
//#define SHIELD_RESET_PIN  -1   // VS1053 reset pin (unused!)
#define SHIELD_CS_PIN       7    // VS1053 chip select pin (output)
#define SHIELD_DCS_PIN      6    // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS_PIN   4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ_PIN     3     // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_CS_PIN, SHIELD_DCS_PIN, DREQ_PIN, CARDCS_PIN);
  
char* myFILESs[]={
  "2001song.mp3","puzzlin.wav","mrngdave.wav","cantdo.wav","operatnl.wav"
  };

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

////////////////////////////////////////////
//
// Microphone breakout
// Connect AUD to analog input pin
// Connect VCC to 3.3V (3.3V yields the best results)
//
#define MICROPHONE_PIN    0

////////////////////////////////////////////
//
// Servos
//
#include <Servo.h>
Servo servoDoor;
Servo servoHand;
#define SERVO_DOOR_PIN    10
#define SERVO_HAND_PIN    11

void setup() {
  Serial.begin(9600);
  //If it prints less than 700, you can not add SD.h.  
  //SD.h has a 512 byte buffer for SD blocks and you will need about 200 for stack.
  freeMem();

  // init music player
  musicShieldSetup();

  // init leds
  //ledsSetup();
  
  // init TV
  tvSetup();
  
  // init servos
  servosSetup();
  
  // Play one file, don't return until complete
  /*
  Serial.println(F("setup play file"));
  if (! musicPlayer.startPlayingFile("BHS1.MP3")) {
    Serial.println(F("Could not open file BHS1.MP3"));
  } 
 */ 
  //musicPlayer.playFullFile("SPOON3.MP3");
  //musicPlayer.startPlayingFile("SPOON3.MP3");

  
}

void loop()
{
  // play random samples
  musicLoop();
  
  // set and change red light brightness based on microphone input
  microphoneLoop();
  
  //TODO: servos logic
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

void tvSetup() {
  TV.begin(PAL,120,96);
}

void servosSetup() {
  servoDoor.attach(SERVO_DOOR_PIN);
  servoHand.attach(SERVO_HAND_PIN);
  
  servoDoor.write(0);
  servoHand.write(0);
}

void musicShieldSetup() {
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
  // initialise the SD card
  if (!SD.begin(CARDCS_PIN)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  Serial.println(F("SD OK!"));

  // list files
  //printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20); // used to be 20,20

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background audio playing
  if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) // DREQ int
    Serial.println(F("DREQ pin is not an interrupt pin"));
  
  // Play a short tone to indicate VS1053 is working
  //musicPlayer.sineTest(0x44, 30);

  musicPlayer.GPIO_pinMode(2, INPUT);
}

void microphoneLoop() {

  // read microphone 
  unsigned int micAmplitude = analogRead(MICROPHONE_PIN);
  //TEST: print micAmplitude to serial
  
  //TODO: change leds brightness based on microphone input
  //TODO: scale micAmplitude from 0-1024 to brightness level 600-1024
  //setBrightness(uint8_t);
  
  TV.clear_screen();
  uint8_t radius = TV.vres()/3; // default circle radius
  TV.draw_circle(TV.hres()/2, TV.vres()/2, TV.vres()/radius, WHITE);
  
}

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
    int rndInt = random(1, 32); //FIXME
    Serial.print(F("musicLoop play file ")); Serial.println(myFILESs[rndInt]);
    if(!musicPlayer.startPlayingFile(myFILESs[rndInt]))
    {//startPlayingFile playFullFile
      Serial.println(F("failed to play"));
    }
  }
    
}

void servosLoop() {
 //servoDoor.write(position); 
}

uint16_t freeMem() {
  char top;
  extern char *__brkval;
  extern char __bss_end;
  Serial.println( __brkval ? &top - __brkval : &top - &__bss_end);
}
