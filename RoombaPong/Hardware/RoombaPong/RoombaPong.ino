
#include "roombaDefines.h"
#include <SoftwareSerial.h>

SoftwareSerial wifiSerial(2, 3);      // RX, TX for ESP8266
//SoftwareSerial wifiSerial(0, 1);      // RX, TX for UNO WIFI ESP8266

bool DEBUG = true;   //show more logs
int responseTime = 10; //communication timeout


// Roomba Create2 connection
int rxPin=10;
int txPin=11;
SoftwareSerial Roomba(rxPin,txPin);



void setup()
{
  pinMode(13,OUTPUT);  //set build in led as output
  // Open serial communications and wait for port to open esp8266:
  Serial.begin(115200);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}
  wifiSerial.begin(115200);
  //while (!wifiSerial) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}

  //UNO WIFI ESP8266
  //pinMode(14,OUTPUT);
  //delay(500);
  
  sendToWifi("AT+CWMODE=2",responseTime,DEBUG); // configure as access point
  sendToWifi("AT+CIFSR",responseTime,DEBUG);    // get ip address
  sendToWifi("AT+CIPMUX=1",responseTime,DEBUG); // configure for multiple connections
  sendToWifi("AT+CIPSERVER=1,80",responseTime,DEBUG); // turn on server on port 80
  sendToUno("Wifi connection is running!",responseTime,DEBUG);

  Roomba.begin(115200);
  pinMode(ddPin, OUTPUT);
  pinMode(ledPin, OUTPUT); //spare if needed for test purpose
  delay(2000);
  Serial.println("Roomba Remote Control");

  arduinoLedBlink();

  Serial.println("wakeUp");
  wakeUp ();   // Wake-up Roomba

  Serial.println("changeBoudRateTo19200");
  roombaChangeBoudRateTo19200();

  Serial.println("startSafe");
  startSafe(); // Start Roomba in Safe Mode

  arduinoLedBlink();
  
  // turn-off all LEDs and Display
  Serial.println("setPowerLED");
  setPowerLED(128,0);
  setDebrisLED(OFF);
  setDockLED(OFF);
  setSpotLED(OFF);
  setWarningLED(OFF);
  Serial.println("cleanDigitLED");
  cleanDigitLED ();

  Serial.println("playSound");
  playSound (1); // start sound
  delay(1000);
  //while (digitalRead(buttonPin))
  //{  
    setDebrisLED(ON);
    writeLEDs ('-', '-', '-', '-');
  //}
  setDebrisLED(OFF);
  writeLEDs ('s', 't', 'o', 'p');

  Serial.println("playSound");
  playSound (2);
  delay(1000);

  Serial.println("test Roomba");
  motorSquareTest(); // un-comment if you want to test Roomba doing a square 
  //turnCW (40, 180);  //un-comment if you want to test Roomba spin clock-wise 180 degrees and stop
  //driveWheels(20, -20); //un-comment if you want to test Roomba spin
  //driveLeft(20); //un-comment if you want to test Roomba turning left 

  Serial.println("setup done");
}


void loop()
{
  if(Serial.available()>0){
     String message = readSerialMessage();
    if(find(message,"debugEsp8266:")){
      String result = sendToWifi(message.substring(13,message.length()),responseTime,DEBUG);
      if(find(result,"OK"))
        sendData("\nOK");
      else
        sendData("\nEr");
    }
  }
  if(wifiSerial.available()>0){
    
    String message = readWifiSerialMessage();
    
    if(find(message,"esp8266:")){
       String result = sendToWifi(message.substring(8,message.length()),responseTime,DEBUG);
      if(find(result,"OK"))
        sendData("\n"+result);
      else
        sendData("\nErrRead");               //At command ERROR CODE for Failed Executing statement
    }else
    if(find(message,"HELLO")){  //receives HELLO from wifi
        sendData("\\nHI!");    //arduino says HI
    }else if(find(message,"LEDON")){
      //turn on built in LED:
      digitalWrite(13,HIGH);
    }else if(find(message,"LEDOFF")){
      //turn off built in LED:
      digitalWrite(13,LOW);
    }
    else{
      sendData("\nErrRead");                 //Command ERROR CODE for UNABLE TO READ
    }
  }
  delay(responseTime);
}


/*
* Name: sendData
* Description: Function used to send string to tcp client using cipsend
* Params: 
* Returns: void
*/
void sendData(String str){
  String len="";
  len+=str.length();
  sendToWifi("AT+CIPSEND=0,"+len,responseTime,DEBUG);
  delay(100);
  sendToWifi(str,responseTime,DEBUG);
  delay(100);
  sendToWifi("AT+CIPCLOSE=5",responseTime,DEBUG);
}


/*
* Name: find
* Description: Function used to match two string
* Params: 
* Returns: true if match else false
*/
boolean find(String string, String value){
  return string.indexOf(value)>=0;
}


/*
* Name: readSerialMessage
* Description: Function used to read data from Arduino Serial.
* Params: 
* Returns: The response from the Arduino (if there is a reponse)
*/
String  readSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(Serial.available()>0){
    value[index_count]=Serial.read();
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
  }
  String str(value);
  str.trim();
  return str;
}



/*
* Name: readWifiSerialMessage
* Description: Function used to read data from ESP8266 Serial.
* Params: 
* Returns: The response from the esp8266 (if there is a reponse)
*/
String  readWifiSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(wifiSerial.available()>0){
    value[index_count]=wifiSerial.read();
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
  }
  String str(value);
  str.trim();
  return str;
}



/*
* Name: sendToWifi
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendToWifi(String command, const int timeout, boolean debug){
  String response = "";
  wifiSerial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(wifiSerial.available())
    {
    // The esp has data so display its output to the serial window 
    char c = wifiSerial.read(); // read the next character.
    response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}

/*
* Name: sendToUno
* Description: Function used to send data to Arduino.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendToUno(String command, const int timeout, boolean debug){
  String response = "";
  Serial.println(command); // send the read character to the esp8266
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(Serial.available())
    {
      // The esp has data so display its output to the serial window 
      char c = Serial.read(); // read the next character.
      response+=c;
    }  
  }
  if(debug)
  {
    Serial.println(response);
  }
  return response;
}

// Use the Baud Rate Change pin (pin 5 on the Mini-DIN connector) to change Roomba’s baud rate. After turning on Roomba, 
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

void arduinoLedBlink()
{
  digitalWrite(ledPin, HIGH);   // sets the LED on
  delay(1000);                  // waits for a second
  digitalWrite(ledPin, LOW);    // sets the LED off
  delay(1000);                  // waits for a second
}
