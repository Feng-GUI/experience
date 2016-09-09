////////////////////////////////////////////
//
// UselessMachine HAL9000
// Servos and Music

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
#define SHIELD_MISO_PIN      12    //
#define SHIELD_MOSI_PIN      11    //
#define SHIELD_SCK_PIN      13    //

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
  "puzzlin.wav","2001song.mp3","mrngdave.wav","cantdo.wav","operatnl.wav"
  };

////////////////////////////////////////////
//
// Servos
// NOTE: servo original library is using the same timer as the PWM 
//       and always using pins 9 and 10 even when the servos are attached to another pins
#include <Servo.h>
Servo servoDoor;
Servo servoHand;
#define SERVO_DOOR_PIN    14 + 3 // analog pin A4
#define SERVO_HAND_PIN    14 + 4 // analog pin A5
int servoPosDoor = 0;
int servoPosHand = 0;
static unsigned long servoFrameRate = 0;
    
    
void setup() {
  Serial.begin(9600);
  //If it prints less than 700, you can not add SD.h.  
  //SD.h has a 512 byte buffer for SD blocks and you will need about 200 for stack.
  freeMem();

  // init music player
  musicShieldSetup();

  // init leds
  //ledsSetup();
  
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
    
  // servos
  servosLoop();
}

void servosSetup() {
  servoDoor.attach(SERVO_DOOR_PIN);
  servoHand.attach(SERVO_HAND_PIN);
  
  // move to start position
  servoDoor.write(0);
  servoHand.write(0);
  // wait for the servo to reach its position
  delay(1000);
  servoDoor.detach();
  servoHand.detach();

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
  musicPlayer.setVolume(30,30); // used to be 20,20

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background audio playing
  if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) // DREQ int
    Serial.println(F("DREQ pin is not an interrupt pin"));
  
  // Play a short tone to indicate VS1053 is working
  //musicPlayer.sineTest(0x44, 30);
  musicPlayer.GPIO_pinMode(2, INPUT);
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
    int rndInt = random(0, 4); //random (min,max)
    Serial.print(F("musicLoop play file ")); Serial.println(myFILESs[rndInt]);
    if(!musicPlayer.startPlayingFile(myFILESs[rndInt]))
    {//startPlayingFile playFullFile
      Serial.println(F("failed to play"));
    }
  }
    
}


void servosLoop() { 
 
  static unsigned long lastMillis;
  unsigned long now = millis();
  
  servoFrameRate++;
  
  // Test servos start
  int servoMinRange = 0; //0 degrees 1000 milli
  int servoMaxRange = 160; //180 dgrees 2000 milli
  int servoJump = 1;//1
  
  // do the test once every 3 seconds
  if (now - lastMillis >= 3 * 1000) {

    lastMillis = now;
    
    servoDoor.attach(SERVO_DOOR_PIN);
    servoHand.attach(SERVO_HAND_PIN);
  
    servoPosHand = random (servoMinRange, servoMaxRange);
    
    servoDoor.write(servoPosHand);
    servoHand.write(servoPosHand);
    delay(1000); // waits for the servo to reach the position 
  
    servoDoor.detach();
    servoHand.detach();
    
  }
   
  // test servos end

}

uint16_t freeMem() {
  char top;
  extern char *__brkval;
  extern char __bss_end;
  Serial.println( __brkval ? &top - __brkval : &top - &__bss_end);
}
