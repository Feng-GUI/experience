/*Simon Says game. Now with sound effects. 

Originaly made by Robert Spann
Code trimmed and sound effects added by digimike

Buttons are to be set on there designated pins without pull down resistors
and connected to ground rather then +5. 
*/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

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
  
char* files1[]={ "1ANNA1.MP3"  , "1ANNA2.MP3"  , "1ANNA3.MP3"  , "2ELSA1.MP3"  ,   "2ELSA2.MP3" , "2ELSA3.MP3"}; //  6
char* files2[]={ "3MULAN1.MP3" , "3MULAN2.MP3" , "3MULAN3.MP3" , "4ARIEL1.MP3" ,   "4ARIEL2.MP3" };              //  5
char* files3[]={ "5YASMIN1.MP3", "5YASMIN2.MP3", "6TANGLED1.MP3" , "6TANGLED2.MP3","6TANGLED3.MP3" };            //  5
char* files4[]={ "7BELL1.MP3"  , "7BELL2.MP3"  , "7BELL3.MP3"    , "8FROG1.MP3"   ,"8FROG2.MP3","8FROG3.MP3" };  //  6

//#include <Tone.h>
//Tone speakerpin;
//int starttune[] = {NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4};
//int duration2[] = {100, 200, 100, 200, 100, 400, 100, 100, 100, 100, 200, 100, 500};
//int note[] = {NOTE_C4, NOTE_C4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5};
//int duration[] = {100, 100, 100, 300, 100, 300};
boolean button[] = {2, 3, 4, 5}; //The four button input pins
boolean ledpin[] = {8, 9, 10, 11};  // LED pins
int turn = 0;  // turn counter
int buttonstate = 0;  // button state checker
int randomArray[100]; //Intentionally long to store up to 100 inputs (doubtful anyone will get this far)
int inputArray[100];  


void setup() 
{
  Serial.begin(9600);
  // If it prints less than 700, you can not add SD.h.  
  // SD.h has a 512 byte buffer for SD blocks and you will need about 200 for stack.
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
  musicPlayer.setVolume(1,1); // used to be 20,20

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background audio playing
  if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) // DREQ int
    Serial.println(F("DREQ pin is not an interrupt pin"));
  
  //musicPlayer.sineTest(0x44, 30);    // Make a tone to indicate VS1053 is working
  musicPlayer.GPIO_pinMode(2, INPUT);

  //musicPlayer.playFullFile("FOO.MP3");
  //musicPlayer.startPlayingFile("FOO.MP3");
  
  //speakerpin.begin(12); // speaker is on pin 13

  for(int x=0; x<4; x++)  // LED pins are outputs
  {
    pinMode(ledpin[x], OUTPUT);
  }
  
  for(int x=0; x<4; x++) 
  {
    pinMode(button[x], INPUT);  // button pins are inputs
    digitalWrite(button[x], HIGH);  // enable internal pullup; buttons start in high position; logic reversed
  }

  randomSeed(analogRead(0)); //Added to generate "more randomness" with the randomArray for the output function
  
  // play the start tune
  musicPlayer.playFullFile("PASS.MP3");
  /*
  for (int thisNote = 0; thisNote < 13; thisNote ++) {
     // play the next note:
     speakerpin.play(starttune[thisNote]);
     // hold the note:
     if (thisNote==0 || thisNote==2 || thisNote==4 || thisNote== 6)
     {
       digitalWrite(ledpin[0], HIGH);
     }
     if (thisNote==1 || thisNote==3 || thisNote==5 || thisNote== 7 || thisNote==9 || thisNote==11)
     {
       digitalWrite(ledpin[1], HIGH);
     }
     if (thisNote==8 || thisNote==12)
     {
       digitalWrite(ledpin[2], HIGH);
     }  
     if (thisNote==10)
     {   
       digitalWrite(ledpin[3], HIGH);
     }
     delay(duration2[thisNote]);
     // stop for the next note:
     //rafi speakerpin.stop();
     digitalWrite(ledpin[0], LOW);
     digitalWrite(ledpin[1], LOW);
     digitalWrite(ledpin[2], LOW);
     digitalWrite(ledpin[3], LOW);
     delay(25);
    }
    */
  delay(1000);
}
 
void loop() 
{   
  for (int y=0; y<=99; y++)
  {
    //function for generating the array to be matched by the player
    /*
    digitalWrite(ledpin[0], HIGH);
    digitalWrite(ledpin[1], HIGH);
    digitalWrite(ledpin[2], HIGH);
    digitalWrite(ledpin[3], HIGH);
  */
  
    for (int thisNote = 0; thisNote < 6; thisNote ++) {
     // play the next note:
     //rafi speakerpin.play(note[thisNote]);
     // hold the note:
     //delay(duration[thisNote]);
     // stop for the next note:
     //rafi speakerpin.stop();
     delay(25);
    }
    
    /*
    digitalWrite(ledpin[0], LOW);
    digitalWrite(ledpin[1], LOW);
    digitalWrite(ledpin[2], LOW);
    digitalWrite(ledpin[3], LOW);
    */
    delay(1000);
  
    for (int y=turn; y <= turn; y++)
    { //Limited by the turn variable
      Serial.println(""); //Some serial output to follow along
      Serial.print(F("Turn: "));
      Serial.print(y);
      Serial.println("");
      randomArray[y] = random(1, 5); //Assigning a random number (1-4) to the randomArray[y], y being the turn count
      for (int x=0; x <= turn; x++)
      {
        Serial.print(randomArray[x]);
      
        for(int y=0; y<4; y++)
        {
      
          if (randomArray[x] == 1 && ledpin[y] == 8) 
          {  //if statements to display the stored values in the array
            digitalWrite(ledpin[y], HIGH);
            //rafi speakerpin.play(NOTE_G3, 100);
            playRandomFile(1);
            delay(400);
            digitalWrite(ledpin[y], LOW);
            delay(100);
          }

          if (randomArray[x] == 2 && ledpin[y] == 9) 
          {
            digitalWrite(ledpin[y], HIGH);
            //rafi speakerpin.play(NOTE_A3, 100);
            playRandomFile(2);
            delay(400);
            digitalWrite(ledpin[y], LOW);
            delay(100);
          }
  
          if (randomArray[x] == 3 && ledpin[y] == 10) 
          {
            digitalWrite(ledpin[y], HIGH);
            //rafi speakerpin.play(NOTE_B3, 100);
            playRandomFile(3);
            delay(400);
            digitalWrite(ledpin[y], LOW);
            delay(100);
          }

          if (randomArray[x] == 4 && ledpin[y] == 11) 
          {
            digitalWrite(ledpin[y], HIGH);
            //rafi speakerpin.play(NOTE_C4, 100);
            playRandomFile(4);
            delay(400);
            digitalWrite(ledpin[y], LOW);
            delay(100);
          }
        }
      }
    }
    input();
  }
}
 
 
 
void input() { //Function for allowing user input and checking input against the generated array

  for (int x=0; x <= turn;)
  { //Statement controlled by turn count

    for(int y=0; y<4; y++)
    {
      
      buttonstate = digitalRead(button[y]);
    
      if (buttonstate == LOW && button[y] == 2)
      { //Checking for button push
        digitalWrite(ledpin[0], HIGH);
        //rafi speakerpin.play(NOTE_G3, 100);
        delay(200);
        digitalWrite(ledpin[0], LOW);
        inputArray[x] = 1;
        delay(250);
        Serial.print(" ");
        Serial.print(1);
        if (inputArray[x] != randomArray[x]) { //Checks value input by user and checks it against
          fail();                              //the value in the same spot on the generated array
        }                                      //The fail function is called if it does not match
        x++;
      }
       if (buttonstate == LOW && button[y] == 3)
      {
        digitalWrite(ledpin[1], HIGH);
        //rafi speakerpin.play(NOTE_A3, 100);
        delay(200);
        digitalWrite(ledpin[1], LOW);
        inputArray[x] = 2;
        delay(250);
        Serial.print(" ");
        Serial.print(2);
        if (inputArray[x] != randomArray[x]) {
          fail();
        }
        x++;
      }

      if (buttonstate == LOW && button[y] == 4)
      {
        digitalWrite(ledpin[2], HIGH);
        //rafi speakerpin.play(NOTE_B3, 100);
        delay(200);
        digitalWrite(ledpin[2], LOW);
        inputArray[x] = 3;
        delay(250);
        Serial.print(" ");
        Serial.print(3);
        if (inputArray[x] != randomArray[x]) {
          fail();
        }
        x++;
      }

      if (buttonstate == LOW && button[y] == 5)
      {
        digitalWrite(ledpin[3], HIGH);
        //rafi speakerpin.play(NOTE_C4, 100);
        delay(200);
        digitalWrite(ledpin[3], LOW);
        inputArray[x] = 4;
        delay(250);
        Serial.print(" ");
        Serial.print(4);
        if (inputArray[x] != randomArray[x]) 
        {
          fail();
        }
        x++;
      }
    }
  }
  delay(500);
  turn++; //Increments the turn count, also the last action before starting the output function over again
}

void fail() { //Function used if the player fails to match the sequence
  musicPlayer.playFullFile("FAIL.MP3");
  
  /*
  for (int y=0; y<=2; y++)
  { //Flashes lights for failure
    
    digitalWrite(ledpin[0], HIGH);
    digitalWrite(ledpin[1], HIGH);
    digitalWrite(ledpin[2], HIGH);
    digitalWrite(ledpin[3], HIGH);
    //rafi speakerpin.play(NOTE_G3, 300);
    delay(200);
    digitalWrite(ledpin[0], LOW);
    digitalWrite(ledpin[1], LOW);
    digitalWrite(ledpin[2], LOW);
    digitalWrite(ledpin[3], LOW);
    //rafi speakerpin.play(NOTE_C3, 300);
    delay(200);
  }
  */
  delay(500);
  turn = -1; //Resets turn value so the game starts over without need for a reset button
}

void playRandomFile(int group)
{
  // 6 items
  if(group == 1) 
    musicPlayer.playFullFile(files1[random(0,6)]); // 6 items
  else if (group == 4)
    musicPlayer.playFullFile(files4[random(0,6)]); // 6 items
  else if (group == 2)
    musicPlayer.playFullFile(files2[random(0,5)]); // 5 items
  else // (group == 3)
    musicPlayer.playFullFile(files3[random(0,5)]); // 5 items
  
}

uint16_t freeMem() {
  char top;
  extern char *__brkval;
  extern char __bss_end;
  Serial.println( __brkval ? &top - __brkval : &top - &__bss_end);
}

