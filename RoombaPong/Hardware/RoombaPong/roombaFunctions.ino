
// wake up the robot using ddPin (BRC at the connector)
// instead, you can change the boud rate manually
void wakeUp (void)
{
  //setWarningLED(ON);
  digitalWrite(ddPin, HIGH);
  delay(100);
  digitalWrite(ddPin, LOW);
  delay(500);
  digitalWrite(ddPin, HIGH);
  delay(2000);
}

// Use the ddPin Baud Rate Change pin (pin 5 on the Mini-DIN connector) to change Roomba’s baud rate. After turning on Roomba, 
//wait 2 seconds and then pulse the Baud Rate Change low three times. 
//Each pulse should last between 50 and 500 milliseconds. Roomba will communicate at 19200 baud 
//until the processor loses battery power or the baud rate is explicitly changed by way of the OI.

void roombaChangeBoudRateTo19200 (void)
{
  digitalWrite(ddPin, HIGH);  delay(100);  
  digitalWrite(ddPin, LOW);
  delay(100);
  digitalWrite(ddPin, HIGH);  delay(100);  
  digitalWrite(ddPin, LOW);
  delay(100);
  digitalWrite(ddPin, HIGH);  delay(100);  
  digitalWrite(ddPin, LOW);
  delay(100);
  digitalWrite(ddPin, HIGH);  delay(100);  
  digitalWrite(ddPin, LOW);
  delay(2000);
}

/*--------------------------------------------------------------------------
This command resets the robot, as if you had removed and reinserted the battery. */
void reset(void)
{
  Roomba.write(7);
}

/*--------------------------------------------------------------------------
This command gives you control over Roomba turning on the cliff, wheel-drop and internal charger safety features.*/
void startSafe()
{  
  Roomba.write(128);  //Start
  delay(150);
  Roomba.write(131);  //Safe mode
  delay(1000);
}

/*--------------------------------------------------------------------------
This command gives you complete control over Roomba by putting the OI into Full mode, and turning off the cliff, wheel-drop and internal charger safety features.*/
void startFull()
{  
  Roomba.write(128);  //Start
  Roomba.write(132);  //Full mode
  delay(1000);
}

/*--------------------------------------------------------------------------
This command stops the OI. All streams will stop and the robot will no longer respond to commands. Use this command when you are finished working with the robot. */
void roombaStop(void)
{
  Roomba.write(173);
}

/*--------------------------------------------------------------------------
This command powers down Roomba.*/
void powerOff(void)
{
  Roomba.write(133);
}

/*--------------------------------------------------------------------------
This command sends Roomba to seek for its dock.*/
void seekDock(void)
{
  Roomba.write(143);
}

/*-----------------------------------------------------
 This command Play song
 [140] [song #] [lenght] [note#] [note duration]
 [141] [song #]                                      */
void playSound (int num) 
{
  switch (num)
  { 
    case 1: 
      Roomba.write("\x8c\x01\x04\x42\x20\x3e\x20\x42\x20\x3e\x20"); // [140] [1] [4] [68] [32] ... place "start sound" in slot 1
      Roomba.write("\x8d\x01"); // [141] [1] play it (in slot 1)
      break;
 
    case 2: 
      Roomba.write("\x8c\x01\x01\x3c\x20"); // place "low freq sound" in slot 2
      Roomba.write("\x8d\x01"); // play it (in slot 2)
      break;

    case 3:
      Roomba.write("\x8c\x01\x01\x48\x20"); // place "high freq sound" in slot 3
      Roomba.write("\x8d\x01"); // play it (in slot 3)
      break;
  }
}


void drive(int velocity, int radius)
{
  clamp(velocity, -500, 500); //def max and min velocity in mm/s
  clamp(radius, -2000, 2000); //def max and min radius in mm
  
  Roomba.write(137);
  Roomba.write(velocity >> 8);
  Roomba.write(velocity);
  Roomba.write(radius >> 8);
  Roomba.write(radius);
}

//---------------------------------------------------------------

void driveWheels(int right, int left)
{
  clamp(right, -500, 500);
  clamp(left, -500, 500);
  
  Roomba.write(145);
  Roomba.write(right >> 8);
  Roomba.write(right);
  Roomba.write(left >> 8);
  Roomba.write(left);
  }

//---------------------------------------------------------------
void driveWheelsPWM(int rightPWM, int leftPWM)
{
  clamp(rightPWM, -255, 255);
  clamp(leftPWM, -255, 255);
  
  Roomba.write(146);
  Roomba.write(rightPWM >> 8);
  Roomba.write(rightPWM);
  Roomba.write(leftPWM >> 8);
  Roomba.write(leftPWM);
}

//---------------------------------------------------------------
void turnCW(unsigned short velocity, unsigned short degrees)
{
  drive(velocity, -1);
  clamp(velocity, 0, 500);
  //delay(6600);
  delay(9.15*degrees);
  //delay((1580 + 2.25*velocity)/velocity*degrees);
  //delay((-0.03159720835 * velocity + 21.215270835) * degrees);
  drive(0,0);
}

//---------------------------------------------------------------
void turnCCW(unsigned short velocity, unsigned short degrees)
{
  drive(velocity, 1);
  clamp(velocity, 0, 500);
  //delay(6600);
  delay(9.15*degrees);
  //delay((1580 + 2.25*velocity)/velocity*degrees);
  //delay((-0.03159720835 * velocity + 21.215270835) * degrees);
  drive(0,0);
}

//---------------------------------------------------------------
void driveStop(void)
{
  drive(0,0);
}

//---------------------------------------------------------------
void driveLeft(int left)
{
  driveWheels(left, 0);
}

//---------------------------------------------------------------
void driveRight(int right)
{
  driveWheels(0, right);
}


//---------------------------------------------------------------
void motorSquareTest(void)
{  
  setPowerLED(0,128);
  drive (60, 0);
  delay(5000);
  driveStop();
  turnCCW (30, 90);
  
  setPowerLED(64,128);
  drive (60, 0);
  delay(5000);
  driveStop();
  turnCCW (30, 90);
  
  setPowerLED(128,128);
  drive (60, 0);
  delay(5000);
  driveStop();
  turnCCW (30, 90);

  setPowerLED(190,128);
  drive (60, 0);
  delay(5000);
  driveStop();
  setPowerLED(255,128);
  turnCCW (30, 90);
  setPowerLED(0,0);
}

void motorTrapezTest()
{
  setPowerLED(0,128);
  drive (motorSpeed, 0);
  delay(5000);
  driveStop();
  turnCCW (motorSpeed, 30);
  
  setPowerLED(64,128);
  drive (motorSpeed, 0);
  delay(5000);
  driveStop();
  turnCCW (motorSpeed, 120);
  
  setPowerLED(128,128);
  drive (motorSpeed, 0);
  delay(5000);
  driveStop();
  turnCCW (motorSpeed, 30);

  setPowerLED(190,128);
  drive (motorSpeed, 0);
  delay(5000);
  driveStop();
  turnCCW (motorSpeed, 90);
  setPowerLED(255,128);
  setPowerLED(0,0);
}
 
//blocking sensor functions - these will request data and wait until a response is recieved, then return the response

/**
  Returns the value from the requested id
**/

int getSensorData(byte sensorID)
{
  int returnVal;
  byte packetID = 0;
  if (sensorID > 100)
  {
    switch (sensorID)
    {
      case 101:
      case 102:
      case 103:
      case 104:
        packetID = 7;
        break;
      case 105:
      case 106:
      case 107:
      case 108:
        packetID = 14;
        break;
      case 109:
      case 110:
      case 111:
      case 112:
      case 113:
      case 114:
      case 115:
      case 116:
        packetID = 18;
        break;
      case 117:
      case 118:
      case 119:
      case 120:
      case 121:
      case 122:
        packetID = 45;
        break;
    }

  }
  else {
    packetID = sensorID;
  }
  byte MSB = 0;
  byte LSB = 0;
  Roomba.write(142);
  Roomba.write(packetID);
    if(is_in_array(packetID)){
      while (!Roomba.available());
      returnVal = Roomba.read();
    } else {
      while (!Roomba.available());
      MSB = Roomba.read();
      LSB = Roomba.read();
      returnVal = (MSB << 7) | LSB;
    }
    
  return returnVal;
}

//-------------------------------------------------------------------------
bool is_in_array(byte val)
{
  for (int i=0;i<22;i++){
    if (val == single_byte_packets[i])
    {
      return true;
    }
  }
  return false;
}


/*  
 This command controls the Power LED by two data bytes: color and intensity.
 color: Green = 0; 128 = Orange; red=255 
 intensity low=0; max=255 */
void setPowerLED(byte setColor, byte setIntensity)
{
  color = setColor;
  intensity = setIntensity;
  Roomba.write(139);
  Roomba.write((byte)0x00);
  Roomba.write((byte)color);
  Roomba.write((byte)intensity);
}

//---------------------------------------------------------------
void setDebrisLED(bool enable)
{
  debrisLED = enable;
  Roomba.write(139);
  Roomba.write((debrisLED ? 1 : 0) + (spotLED ? 2 : 0) + (dockLED ? 4 : 0) + (warningLED ? 8 : 0));
  Roomba.write((byte)color);
  Roomba.write((byte)intensity);
}

//---------------------------------------------------------------
void setSpotLED(bool enable)
{
  spotLED = enable;
  Roomba.write(139);
  Roomba.write((debrisLED ? 1 : 0) + (spotLED ? 2 : 0) + (dockLED ? 4 : 0) + (warningLED ? 8 : 0));
  Roomba.write((byte)color);
  Roomba.write((byte)intensity);
}

//---------------------------------------------------------------
void setDockLED(bool enable)
{
  dockLED = enable;
  Roomba.write(139);
  Roomba.write((debrisLED ? 1 : 0) + (spotLED ? 2 : 0) + (dockLED ? 4 : 0) + (warningLED ? 8 : 0));
  Roomba.write((byte)color);
  Roomba.write((byte)intensity);
}

//---------------------------------------------------------------
void setWarningLED(bool enable)
{
  warningLED = enable;
  Roomba.write(139);
  Roomba.write((debrisLED ? 1 : 0) + (spotLED ? 2 : 0) + (dockLED ? 4 : 0) + (warningLED ? 8 : 0));
  Roomba.write((byte)color);
  Roomba.write((byte)intensity);
}

/*--------------------------------------------------------------------------
This command controls the four 7 segment displays on the Roomba.*/
void setDigitLEDs(byte digit1, byte digit2, byte digit3, byte digit4)
{
    Roomba.write(163);
    Roomba.write(digit1);
    Roomba.write(digit2);
    Roomba.write(digit3);
    Roomba.write(digit4);
}

/*--------------------------------------------------------------------------
This command controls the four 7 segment displays on the Roomba using ASCII character codes.*/
void setDigitLEDFromASCII(byte digit, char letter)
{
  switch (digit){
  case 1:
    digit1 = letter;
    break;
  case 2:
    digit2 = letter;
    break;
  case 3:
    digit3 = letter;
    break;
  case 4:
    digit4 = letter;
    break;
  }
  Roomba.write(164);
  Roomba.write(digit1);
  Roomba.write(digit2);
  Roomba.write(digit3);
  Roomba.write(digit4);
}

//---------------------------------------------------------------
void cleanDigitLED (void)
{
  setDigitLEDFromASCII(1, ' ');
  setDigitLEDFromASCII(2, ' ');
  setDigitLEDFromASCII(3, ' ');
  setDigitLEDFromASCII(4, ' ');
}
//---------------------------------------------------------------
void writeLEDs (char a, char b, char c, char d)
{
  setDigitLEDFromASCII(1, a);
  setDigitLEDFromASCII(2, b);
  setDigitLEDFromASCII(3, c);
  setDigitLEDFromASCII(4, d);
}
