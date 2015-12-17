/*************************************************** 
  This is an example for the Adafruit VS1053 Codec Breakout
  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381
 ****************************************************/

// Music Shield: Pins 6, 7, 8, 9 and the 6 analog in pins (also known as digital i/o pins 14-20) are available.

/*
 7 Segment:
 This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 Serial7Segment is an open source seven segment display. 
 This is example code that shows how to display basic numbers and control the decimal, colon, and AM/PM dot.

 Note: This code works with displays the support the center colon and extra dots (AM/PM, not decimals).
 
 Note: This code expects the display to be listening at 9600bps. If your display is not at 9600bps, you can
 do a software or hardware reset. See the Wiki for more info: 
 http://github.com/sparkfun/Serial7SegmentDisplay/wiki/Special-Commands#wiki-baud
*/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <SoftwareSerial.h>

// Serial7Segment software seiral
//#define VS1053_RX 2 // This is the pin that connects to the RX pin on VS1053
const int softwareTx = 8; // connects this arduino pin to RX at 7Seg
const int softwareRx = 9; // We only use the TX pin. do not use this 'rx' side
SoftwareSerial Serial7Segment(softwareRx, softwareTx);

//From https://github.com/sparkfun/Serial7SegmentDisplay/wiki/Special-Commands#wiki-decimal
//#define APOSTROPHE  5
//#define COLON       4
//#define DECIMAL4    3
//#define DECIMAL3    2
#define DECIMAL2    1
//#define DECIMAL1    0

long millisTimer;
byte seconds = 0;
byte minutes = 0;
byte hours = 0;
boolean amTime = true;
boolean colonOn = false;
char tempString[100]; //Used for sprintf
boolean stopAlarm = false;
//boolean speedClock = false;

// These are the pins used for the music maker shield
//#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
  
char* myFILESs[]={
  "BLKRAIN2.MP3","BLKRAIN1.MP3","SPOON1.MP3","OUTSHIN3.MP3","SPOON2.MP3",
  "JESUS3.MP3"  ,"BLKRAIN3.MP3","BLKRAIN5.MP3","BLKRAIN4.MP3","BHS2.MP3",
  "SPOON3.MP3","JESUS4.MP3","OUTSHIN4.MP3","JESUS2.MP3", "SPOON4.MP3",
  "OUTSHIN6.MP3","BURDEN2.MP3","BURDEN4.MP3","OUTSHIN5.MP3","BURDEN1.MP3",
  "BURDEN3.MP3","JESUS5.MP3","OUTSHIN2.MP3","BRITUAL2.MP3","OUTSHIN1.MP3",
  "BLOWUP2.MP3","BRITUAL1.MP3","BLKRAIN6.MP3","BLOWUP1.MP3","BHS1.MP3",
  "BRITUAL3.MP3","JESUS1.MP3"};


void setup() {
  Serial.begin(9600);
  //If it prints less than 700, you can not add SD.h.  SD.h has a 512 byte buffer for SD blocks and you will need about 200 for stack.
  freeMem();

  //
  // music shield
  //
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
  // initialise the SD card
  if (!SD.begin(CARDCS)) {
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
  
  //musicPlayer.sineTest(0x44, 30);    // Make a tone to indicate VS1053 is working

  musicPlayer.GPIO_pinMode(2, INPUT);

  // Play one file, don't return until complete
  /*
  Serial.println(F("setup play file"));
  if (! musicPlayer.startPlayingFile("BHS1.MP3")) {
    Serial.println(F("Could not open file BHS1.MP3"));
  } 
 */ 
  //musicPlayer.playFullFile("SPOON3.MP3");
  //musicPlayer.startPlayingFile("SPOON3.MP3");

  setup7SegmentSerial();
  //setup7SegmentI2C();
  
  // initialize the variables to the current time
  seconds = 55;
  minutes = 59;
  hours = 06;

}

void setup7SegmentSerial()
{
  // 7Segment
  Serial7Segment.begin(9600); //Talk to the Serial7Segment at 9600 bps
  Serial7Segment.write('v'); //Reset the display - this forces the cursor to return to the beginning of the display
  Serial7Segment.print("0659"); //Send the hour and minutes to the display
  millisTimer = millis();
}

void loop()
{
  // should speed the clock
//  if ((hours < 7) && !((hours==0)&&(minutes==15)) )
//    speedClock = true;
//  else
//    speedClock = false;
    
  // if its alarm time
  if (isAlarm())
  {
    // play the alarm
    musicLoop();
  }
  
  //Every second update the various variables and blink colon/apos/decimal
  if( (millis() - millisTimer) > 1000)
  {
    millisTimer += 1000; //Adjust the timer forward 1 second

    seconds++;
    if(seconds > 59)
    {
      seconds -= 60; //Reset seconds and increment minutes
      minutes++;
      if(minutes > 59)
      {
        minutes -= 60; //Reset minutes and increment hours
        hours++;
        if(hours > 12)
        {
          hours -= 12; //Reset hours and flip AM/PM
          if(amTime == true) 
            amTime = false; //Flip AM to PM
          else
            amTime = true;
        }
      }
    }

    //Blink the colon every other second
    if(colonOn == true)
    {
      colonOn = false;
      Serial7Segment.write(0x77);  // Decimal, colon, apostrophe control command
      Serial7Segment.write((byte) 0); // Turns off colon, apostrophoe, and all decimals
    }
    else
    {
      colonOn = true; 
      Serial7Segment.write(0x77);  // Decimal, colon, apostrophe control command
      Serial7Segment.write( (1<<DECIMAL2) ); // Turns second decimal
    }

  }

  //Debug print the time
  //sprintf(tempString, "HH:MM:SS %02d:%02d:%02d", hours, minutes, seconds);
  //Serial.println(tempString);

  //orig sprintf(tempString, "%02d%02d", minutes, seconds); 
  sprintf(tempString, "%02d%02d", hours, minutes); 
  Serial7Segment.print(tempString); //Send serial string out the soft serial port to the S7S

//  if (speedClock)
//     millisTimer += 100;
//  else  
    delay(100);
}

boolean isAlarm()
{
    if ((hours == 7) && (minutes == 0))
    {
      //Serial.println("isAlarm true");
      return true;
    }
    else
    {
      return false;
    }
}

void musicLoop()
{
  // if alarm stopped, do not play more samples
  if (stopAlarm)
  {
    return;
  }
  
  stopAlarm = (musicPlayer.GPIO_digitalRead(2) == HIGH);
  if (stopAlarm)
  {
    Serial.println(F("alarm stopped"));
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
    
  /*
  if (Serial.available()) {
    char c = Serial.read();
    
    // if we get an 's' on the serial console, stop!
    if (c == 's') {
      musicPlayer.stopPlaying();
    }
    
    // if we get an 'p' on the serial console, pause/unpause!
    if (c == 'p') {
      if (! musicPlayer.paused()) {
        Serial.println("Paused");
        musicPlayer.pausePlaying(true);
      } else { 
        Serial.println("Resumed");
        musicPlayer.pausePlaying(false);
      }
    }
  }
  */

}

uint16_t freeMem() {
  char top;
  extern char *__brkval;
  extern char __bss_end;
  Serial.println( __brkval ? &top - __brkval : &top - &__bss_end);
}
