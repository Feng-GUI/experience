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

char* openThemeFile = "2001THEM.mp3";
// if switch was pressed during openning theme
bool inOpenTheme = true;
char* normalFiles[]={"hal_9000.mp3", "2010_05.mp3", "foolprof.mp3", "fault.mp3", 
                     "mrngdave.mp3", "operatnl.mp3", "daisy.mp3", "moment2.mp3", 
                     "stress.mp3", "blast.mp3","boing.mp3","point.mp3"
                    };
const int NORMAL_FILES = 11;
int normalFilesCounter = 0;

char* responseFiles[]={"cantdo.mp3", "cantalow.mp3", "skid.mp3", "woop.mp3", 
                       "puzzlin.mp3", "fault.mp3", "beep.mp3", "game.mp3", 
                       "stop2.mp3","banana.mp3","hop.mp3","radar.mp3"
                    };
const int RESPONSE_FILES = 10;
int responseFilesCounter = 0;

////////////////////////////////////////////
//
// Servos
// NOTE: servo original library is using the same timer as the PWM 
//       and always using pins 9 and 10 even when the servos are attached to another pins
#include <Servo.h>
Servo servoDoor;
Servo servoHand;
#define SERVO_HAND_PIN    14 + 3 // analog pin A5
#define SERVO_DOOR_PIN    14 + 4 // analog pin A4
int servoPosDoor = 0;
int servoPosHand = 0;
    
#define SWITCH_PIN    14 + 5 // analog pin A6
int switchState = 0;

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
  
  // play openning theme
  musicPlayer.startPlayingFile(openThemeFile);

}

void loop()
{
  // play random samples
  musicLoop();
    
  // servos
  servosLoop();
}

void servosSetup() {

  // setup switch
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  
  // setup serboxs pins
  servoDoor.attach(SERVO_DOOR_PIN);
  servoHand.attach(SERVO_HAND_PIN);
  
  // move to start position
  servoDoor.write(100);
  servoHand.write(40);
  // wait for the servo to reach its position
  delay(1000);
  //servoDoor.detach();
  //servoHand.detach();
}

void musicShieldSetup() {
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     //while (1);
  }
  Serial.println(F("VS1053 found"));
  
  // initialise the SD card
  if (!SD.begin(CARDCS_PIN)) {
    Serial.println(F("SD failed, or not present"));
    //while (1);  // don't do anything more
  }
  Serial.println(F("SD OK!"));

  // list files
  //printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(10,10); // used to be 20,20

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background audio playing
  if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) // DREQ int
    Serial.println(F("DREQ pin is not an interrupt pin"));
  
  // Play a short tone to indicate VS1053 is working
  //musicPlayer.sineTest(0x44, 30);
  //musicPlayer.GPIO_pinMode(2, INPUT);
}

void musicLoop()
{
//  // if playback stopped, do not play more samples
//  if (stopPlayback)
//  {
//    return;
//  }
//  
//  stopPlayback = (musicPlayer.GPIO_digitalRead(2) == HIGH);
//  if (stopPlayback)
//  {
//    Serial.println(F("playback stopped"));
//  }
  
  // File is not playing in the background
  // Or, switch is ON during openning theme
  if ((musicPlayer.playingMusic == false) || (switchState && inOpenTheme) ) {

    // NOTE: the music shiled and its IIRC loves to sleep a bit between stop and start playback.
    // if no sleep, and the entire playback can crash
    delay (1000);
    
    char* fileName = "";
    // if switch button is ON
    if (switchState == 0) {
      //int rndInt = random(1, NUM_FILE); //random (min,max)
      //rndInt = test++; if (test >=NUM_FILE)test=1;
   
      fileName = normalFiles[normalFilesCounter];
      normalFilesCounter++;    if (normalFilesCounter >= NORMAL_FILES)   normalFilesCounter=0;
      
      // play non-blocking playback
      Serial.print(F("musicLoop play file ")); Serial.println(fileName);
      if(!musicPlayer.startPlayingFile(fileName)) Serial.println(F("failed to play"));
      
    // if switch button is OFF
    } else {

      fileName = responseFiles[responseFilesCounter];      
      responseFilesCounter++;  if (responseFilesCounter >= RESPONSE_FILES) responseFilesCounter=0;

      // play blocking playback
      Serial.print(F("musicLoop play file ")); Serial.println(fileName);
      if(!musicPlayer.startPlayingFile(fileName)) Serial.println(F("failed to play"));
      
      // switch off only after full plaback of response sound
      //delay(3000);
      //switchoff();
    }
    
  }
   
  // exit out of inOpenTheme mode 
  if ((normalFilesCounter > 0) || (responseFilesCounter > 0) ){
    inOpenTheme = false;
  }
}


void servosLoop() { 

   //if the switch is on, then move door and hand to switch it off...
  if(debounceRead(SWITCH_PIN) == HIGH)
  {
    switchState = 1;
    switchoff();
  } else {
    switchState = 0;
  }

/*  
  static unsigned long lastMillis;
  unsigned long now = millis();
  
  // Test servos start
  int servoMinRange = 0; //0 degrees 1000 milli
  int servoMaxRange = 160; //180 dgrees 2000 milli
  
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
*/

}

// basic move 
void switchoff() 
   {
//    servoDoor.attach(SERVO_DOOR_PIN);
//    servoHand.attach(SERVO_HAND_PIN);

   int pos = 0;
     
   //open door
    //for(pos = 100; pos >= 40; pos -= 5) {
      //servoDoor.write(pos);              
      delay(8);                       
    //}
    servoDoor.write(40);

delay(1500);

    //Moving hand
//    for(pos = 40; pos <= 90; pos += 5) {                                   
//      servoHand.write(pos);               
//      delay(8);                       
//    }  
  servoHand.write(90);

delay(1000);

    //hiding hand
//    for(pos = 70; pos>=40; pos-=5) {                                
//      servoHand.write(pos);               
//      delay(2);                        
//    } 
      servoHand.write(40);
      
delay(1000);    

    // close door
    //for(pos = 40; pos <= 100; pos += 5) {
    //  servoDoor.write(pos);              
      delay(2);     
    //}
      servoDoor.write(100);  
    
    //delay(3000);
    
    //servoDoor.detach();
    //servoHand.detach();

} 

// debounce the read of button
// make the read much more stable.
uint8_t debounceRead(int pin) {
   uint8_t pinState= digitalRead(pin);
   uint8_t timeout = millis();
   while (millis() < timeout+10)
   {
     if (digitalRead(pin) != pinState)
     {
       pinState = digitalRead(pin);
       timeout = millis();
     }
   }
   return pinState;
}

uint16_t freeMem() {
  char top;
  extern char *__brkval;
  extern char __bss_end;
  Serial.println( __brkval ? &top - __brkval : &top - &__bss_end);
}
