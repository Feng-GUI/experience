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


////////////////////////////////////////////
//
// Smoothing
// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.  Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int micAmplitudeAverage = 0;    // the average


void setup() {
  Serial.begin(9600);
  //If it prints less than 700, you can not add SD.h.  
  //SD.h has a 512 byte buffer for SD blocks and you will need about 200 for stack.
  freeMem();

  // Smoothing
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  // init TV
  tvSetup();

  // init leds
  //ledsSetup();
  
}

void loop()
{
  // smooth microphone input avarage
  smoothingLoop();
  
  // set and change red light brightness based on microphone input
  microphoneLoop();
}

void tvSetup() {
  TV.begin(PAL,120,96);
  
  /*
  // test circles
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
*/

}

void smoothingLoop() {
 
 // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(MICROPHONE_PIN);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  micAmplitudeAverage = total / numReadings;
  // send it to the computer as ASCII digits
  Serial.println(micAmplitudeAverage); 
  delay(1);        // delay in between reads for stability
}

void microphoneLoop() {

  // read microphone 
  //micAmplitude = analogRead(MICROPHONE_PIN);
  //TEST: print micAmplitude to serial
  //Serial.println(micAmplitude);
  
  // circle radius
  // map(value, fromLow, fromHigh, toLow, toHigh)
  if (micAmplitudeAverage < 300) micAmplitudeAverage = 300;
  if (micAmplitudeAverage > 600) micAmplitudeAverage = 600;
  uint8_t radius = map (micAmplitudeAverage, 300, 600, 36, 44);

  tvFrameRate++;
  if (tvFrameRate > 10) {
    TV.delay_frame(1);
    TV.clear_screen();
    //radius = TV.vres()/ random (2, 8);
    //TV.draw_circle(TV.hres()/2, TV.vres()/2, radius, WHITE);
    TV.draw_circle(TV.hres()/2, TV.vres()/2, radius, WHITE, INVERT); // add parameter INVERT for full cicle
    
    tvFrameRate = 0;
  }
  
}

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

