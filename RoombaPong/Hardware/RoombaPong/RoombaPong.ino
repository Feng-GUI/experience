/*------------------------------------------------------------------
  Roomba Control via Bluetooth
  ==> Main commands according IRobot Open Interface doc:  http://www.irobotweb.com/~/media/MainSite/PDFs/About/STEM/Create/iRobot_Roomba_600_Open_Interface_Spec.pdf?la=enBasic 
  ==> Using Android app: https://play.google.com/store/apps/details?id=appinventor.ai_mjrovai.MJRoBot_II_BT_Command_V2&hl=en
  ==> Several commands based on Create2 library developed by Dom Amato: https://github.com/brinnLabs/Create2
  Marcelo Jose Rovai - 30 June, 2016 - Visit: http://mjrobot.org
-------------------------------------------------------------------*/

#include "roombaDefines.h"
#include <SoftwareSerial.h>

// Roomba Create2 connection
int txPin=10;
int rxPin=11;

SoftwareSerial Roomba(rxPin,txPin);

// BT Module (HC-06 or HC-05)
// Key in default passwords: 1234 on [Passkey] to connect HC-05 Bluetooth Module
SoftwareSerial BT1(8, 9); // rx, tx


 //By default, Roomba communicates at 115200 baud. If you are using a microcontroller that does not
//support 115200 baud, there are two ways to force Roomba to switch to 19200:
//Method 1 (manual):
//While powering off Roomba, continue to hold down the Clean/Power button after the light has turned off.
//After about 10 seconds, Roomba plays a tune of descending pitches. Roomba will communicate at 19200
//baud until the processor loses battery power or the baud rate is explicitly changed by way of the OI.

//Method 2 (using BRC pin):
//Use the Baud Rate Change pin (pin 5 on the Mini-DIN connector) to change Roomba’s baud rate. After
//turning on Roomba, wait 2 seconds and then pulse the Baud Rate Change low three times. Each pulse
//should last between 50 and 500 milliseconds. Roomba will communicate at 19200 baud until the
//processor loses battery power or the baud rate is explicitly changed by way of the OI. 

//---------------------------------------------
void setup() 
{
  Roomba.begin(19200);
  BT1.begin(9600);
  Serial.begin(19200);
  
  pinMode(ddPin, OUTPUT);
  pinMode(ledPin, OUTPUT); //spare if needed for test purpose
  //pinMode(buttonPin, INPUT_PULLUP); //used for "start" for test purposes

  delay(2000);
  Serial.println("Roomba Remote Control");
  BT1.print("Roomba Remote Control");
  BT1.println('\n');

  // wakeup changes the boud rate
  wakeUp ();   // Wake-up Roomba
  //roombaChangeBoudRateTo19200();
  startSafe(); // Start Roomba in Safe Mode

  // turn-off all LEDs and Display
  setPowerLED(128,0);
  setDebrisLED(OFF);
  setDockLED(OFF);
  setSpotLED(OFF);
  setWarningLED(OFF);
  cleanDigitLED ();

  playSound (1); // start sound
  playSound (1);
  //while (digitalRead(buttonPin))
  //{  
  //  setDebrisLED(ON);
  //  writeLEDs ('-', '-', '-', '-');
  //}
  //setDebrisLED(OFF);
  //writeLEDs ('s', 't', 'o', 'p');

  setDebrisLED(ON);
  writeLEDs ('e', 'y', 'a', 'l');
  
  playSound (2);

  //motorSquareTest(); // un-comment if you want to test Roomba doing a square 
  //turnCW (40, 180);  //un-comment if you want to test Roomba spin clock-wise 180 degrees and stop
  //driveWheels(20, -20); //un-comment if you want to test Roomba spin
  //driveLeft(20); //un-comment if you want to test Roomba turning left 

  Serial.println("setup finished");
}

void loop() 
{
  readBTcmd();
  delay(50);
   //motorTrapezTest();
   //checkBTcmd();  // verify if a comand is received from BT remote control
   //manualCmd ();
}
