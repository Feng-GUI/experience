/*  
    based on OctoWS2811 movie2serial.pde - Transmit video data to 1 or more
    Teensy 3.0 boards running OctoWS2811 VideoDisplay.ino
    http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
    Copyright (c) 2013 Paul Stoffregen, PJRC.COM, LLC

*/

// To configure this program, edit the following sections:
//
//  *: edit the serialConfigure() lines in setup() for your
//     serial device names (Mac, Linux) or COM ports (Windows)
//
//  *: if your LED strips have unusual color configuration,
//     edit colorWiring().  Nearly all strips have GRB wiring,
//     so normally you can leave this as-is.
//
//  *: if playing 50 or 60 Hz progressive video (or faster),
//     edit framerate in movieEvent().

import processing.video.*;
import processing.serial.*;
import java.awt.Rectangle;
import java.awt.Color;
 
// **** added syphon for syphon compatibility ***
// *** source is http://code.google.com/p/syphon-implementations/downloads/detail?name=Syphon%20for%20Processing%202.0%20Public%20Beta%202%20r7.zip&can=2&q=
// *** using release for processing 2.0 r7 / syphon r203
import codeanticode.syphon.*;

//*** declare global constructor for canvas and Syphon client - perhaps this shouldn't be global?
//*** syphon doc at http://codeanticode.wordpress.com/2012/02/21/syphon-client-working-in-processing/

PGraphics canvas;
SyphonClient client;

//***creates new syphon client
//*** A Syphon server can be specified by the name of the application that it contains it, its name, or both:
//*** for example where the application name is SendFrames. Declare using Only application name.
//***client = new SyphonClient(this, "SendFrames");
SyphonClient MyClient = new SyphonClient(this, "Arena", "LED"); //change to name of your syphon server. 
  
// back to PS orignal code
int numPorts=0;  // the number of serial ports in use
int maxPorts=24; // maximum number of serial ports

Serial[] ledSerial = new Serial[maxPorts];     // each port's actual Serial port
Rectangle[] ledArea = new Rectangle[maxPorts]; // the area of the movie each port gets, in % (0-100)
boolean[] ledLayout = new boolean[maxPorts];   // layout of rows, true = even is left->right
PImage[] ledImage = new PImage[maxPorts];      // image sent to each port
int errorCount=0;
float framerate=24;  //***changed to 24 as syphon doesn't seem to declare a framerate -- confirm

float gamma = 2.5; // used to be 1.7, but 2.5 works much better.
int[] gammatable = new int[256];

boolean setupCalled = false;
boolean verbose = true;

// print text on the screen
String typedText = "click a number to open serial";
PFont font;

// pixel mapping table csv file 
// the csv file 25x24 contain information about 3 pixel strips 
/*
Table tablePixelsMap;
final int STRIP_LENGTH = 168;
class MyLocation { 
  public int x, y; 
  MyLocation () { x=-1; y=-1;  }
} 
MyLocation[] strip1 = new MyLocation[STRIP_LENGTH];
MyLocation[] strip2 = new MyLocation[STRIP_LENGTH];
MyLocation[] strip3 = new MyLocation[STRIP_LENGTH];
*/

/*
//                                   01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25;
int[] pixelMappingBruno = new int[] {//1 , 8,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,16, 9, 2,
                                     3 ,10,17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,18,11, 4,
                                     5 ,12,19, 0, 0, 0, 
                                     0 , 0, 0,20,13, 6, 
                                     7 ,14,21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
*/

void setup() {
  //for unknown reason, that setup is being called twice
  if (setupCalled) return;
  setupCalled = true;
  
  // load pixel mapping csv table file
  if (verbose) println ("load pixels.csv file");
  //loadPixelsMappingFile();
  
  String[] list = Serial.list();
  delay(20);
  println("Serial Ports List:");
  println(list);
  
  font = createFont("Helvetica", 12);  
  
  // instead of hard coded, use the number keys to open the serial. see keyPressed function
  //serialConfigure("/dev/tty.usbmodem34741");  // change these to your port names  
  
  // init gamma table
  for (int i=0; i < 256; i++) {
    gammatable[i] = (int)((pow((float)i / 255.0, gamma) * 255.0) /* * 0.5 */ ); // added a 0.5 factor to reduce brighness
  }
  
  if (verbose) println("create the window");
  frame.setTitle("Syphon to Terry Poison");
  size(480, 360, P3D);  // create the window // *** add P3D for PGraphics & canvas
  
  // *** lists available syphon servers
  println("Available Syphon servers:");
  println(SyphonClient.listServers());
  
  //*** A Syphon server can be specified by the name of the application that it contains it, its name, or both:
  //***An application can have several servers:
  //*** client = new SyphonClient(this, "Quartz Composer", "Raw Image");
  //*** client = new SyphonClient(this, "Quartz Composer", "Scene");
  
  background(0);
  
  // **** shoudld Syphon really be started in the setup()? Starting here because myMovie.loop was called in setup.
  if (MyClient.available()) //*** disabled to run once
  {
   canvas = MyClient.getGraphics(canvas);
   image(canvas, 0, 0, width, height);    
  }
  
}

// movieEvent runs for each new frame of movie data
// old PS movie event class. renamed as syphonEvent() using MyClient instance of syphon declared globally

//void movieEvent(Movie m) 
void syphonEvent()
{
  
  //if (verbose) println("syphonEvent start ");
  /*
  if (MyClient.available()) {
    canvas = MyClient.getGraphics(canvas);
    image(canvas, 0, 0, width, height);     
  }
  */
  
  //if (framerate == 0) framerate = m.getSourceFrameRate();
  //*** ignoring frame rate for now - changed to a hard 24  as syphon doesn't seem to declare framerate
 framerate = 24.0; // TODO, how to read the frame rate???
 
  /// ***** CHANGED m.XXX to 'canvas' (set above to the MyClient object created by syphon) ********
  for (int i=0; i < numPorts; i++) {    
    // copy a portion of the movie's image to the LED image
 
    int xoffset = percentage(canvas.width, ledArea[i].x);   //*** m.width changed to canvas.width
    int yoffset = percentage(canvas.height, ledArea[i].y);  //*** m.height changed to canvas.height
    int xwidth =  percentage(canvas.width, ledArea[i].width);
    int yheight = percentage(canvas.height, ledArea[i].height);
    ledImage[i].copy(canvas, xoffset, yoffset, xwidth, yheight,
                     0, 0, ledImage[i].width, ledImage[i].height);
    // convert the LED image to raw data
    byte[] ledData =  new byte[(ledImage[i].width * ledImage[i].height * 3) + 3];
    image2data(ledImage[i], ledData, ledLayout[i]);
    if (i == 0) {
      ledData[0] = '*';  // first Teensy is the frame sync master
      int usec = (int)((1000000.0 / framerate) * 0.75); 
      ledData[1] = (byte)(usec);   // request the frame sync pulse
      ledData[2] = (byte)(usec >> 8); // at 75% of the frame time
    } else {
      ledData[0] = '%';  // others sync to the master board
      ledData[1] = 0;
      ledData[2] = 0;
    }
    // send the raw data to the LEDs
    //if (verbose) println("send the raw data to the LEDs");
    ledSerial[i].write(ledData);
  }
}

// image2data converts an image to OctoWS2811's raw data format.
// The number of vertical pixels in the image must be a multiple of 8.  
// The data array must be the proper size for the image.
void image2data(PImage image, byte[] data, boolean layout) {
  int offset = 3;
  int x, y, xbegin, xend, xinc, mask;
  int linesPerPin = image.height / 8;
    
  int pixel[] = new int[8];
  
  for (y = 0; y < linesPerPin; y++) {
    if ((y & 1) == (layout ? 0 : 1)) {
      // even numbered rows are left to right
      xbegin = 0;
      xend = image.width;
      xinc = 1;
    } else {
      // odd numbered rows are right to left
      xbegin = image.width - 1;
      xend = -1;
      xinc = -1;
    }
    for (x = xbegin; x != xend; x += xinc) {
      for (int i=0; i < 8; i++) {
        // fetch 8 pixels from the image, 1 for each pin
        pixel[i] = image.pixels[x + (y + linesPerPin * i) * image.width];

/*
        // bruno pixel mapping
        if (i==1)
        {
          pixel[i] = 0;
          if (x<100)
            pixel[i] = image.pixels[pixelMappingBruno[x] + (y + linesPerPin * i) * image.width];
        }
*/

        // change brightness and whitish the colors for Louise and Anna
        // Row 0: Louise
        // Row 1: bruno
        // Row 3: Anna
        if (i==0) /* louise */
        {
          /* louise leds needs to be full bright */
          // was 1 and changed into 0.2 when moved to Bruno
          pixel[i] = colorWiringCustom(pixel[i], 0.6, 0.2); 
        }
        else if (i==1) /* bruno */
        {
           pixel[i] = colorWiringCustom(pixel[i], 1, 0.2); /* bruno leds needs to be almost no bright */
        }
        else if (i==3) /* Anna */
        {
          pixel[i] = colorWiringCustom(pixel[i], 0.3, 0.5);
        }
      }
      // convert 8 pixels to 24 bytes
      for (mask = 0x800000; mask != 0; mask >>= 1) {
        byte b = 0;
        for (int i=0; i < 8; i++) {
          if ((pixel[i] & mask) != 0) b |= (1 << i);
        }
        data[offset++] = b;
      }
    }
  } 
}

// translate the 24 bit color from RGB to the actual
// order used by the LED wiring.  GRB is the most common.
int colorWiringCustom(int c, float saturationFactor, float brightnessFactor) {
  int red = (c & 0xFF0000) >> 16;
  int green = (c & 0x00FF00) >> 8;
  int blue = (c & 0x0000FF);
  red = gammatable[red];
  green = gammatable[green];
  blue = gammatable[blue];
  
  // convert the rgb into hsb 
  float[] hsb = Color.RGBtoHSB(red, green, blue, null);
  float hue = hsb[0]; 
  float saturation = hsb[1];
  float brightness = hsb[2];

  // white the hsb
  saturation = saturation * saturationFactor;  if (saturation > 1) saturation = 1;
  brightness = brightness * brightnessFactor;  if (brightness > 1) brightness = 1;

  // convert hsb to rgb
  int newRGB = Color.HSBtoRGB(hue, saturation, brightness);
  red = (newRGB & 0xFF0000) >> 16;
  green = (newRGB & 0x00FF00) >> 8;
  blue = (newRGB & 0x0000FF); 
  
  return (green << 16) | (red << 8) | (blue); // GRB - most common wiring
}


// translate the 24 bit color from RGB to the actual
// order used by the LED wiring.  GRB is the most common.
int colorWiring(int c) {
  int red = (c & 0xFF0000) >> 16;
  int green = (c & 0x00FF00) >> 8;
  int blue = (c & 0x0000FF);
  red = gammatable[red];
  green = gammatable[green];
  blue = gammatable[blue];
  return (green << 16) | (red << 8) | (blue); // GRB - most common wiring
}

// ask a Teensy board for its LED configuration, and set up the info for it.
void serialConfigure(String portName) 
{
  if (verbose) println("serialConfigure start");
  
  if (numPorts >= maxPorts) {
    println("too many serial ports, please increase maxPorts");
    errorCount++;
    return;
  }
  try {
    ledSerial[numPorts] = new Serial(this, portName);
    if (ledSerial[numPorts] == null) 
      throw new NullPointerException();
    ledSerial[numPorts].write('?');
  } catch (Throwable e) {
    println("Serial port " + portName + " does not exist or is non-functional");
    println(e.getMessage());
    errorCount++;
    return;
  }
  delay(50);
  String line = ledSerial[numPorts].readStringUntil(10);
  if (line == null) {
    println("Serial port " + portName + " is not responding.");
    println("Is it really a Teensy 3.0 running VideoDisplay?");
    errorCount++;
    return;
  }
  else
  {
    //println("Teensy config: w,h)
    println("Got from Teensy:" +line);
  }
  String param[] = line.split(",");
  if (param.length != 12) {
    println("Error: port " + portName + " did not respond to LED config query");
    errorCount++;
    return;
  }
  // only store the info and increase numPorts if Teensy responds properly
  ledImage[numPorts] = new PImage(Integer.parseInt(param[0]), Integer.parseInt(param[1]), RGB);
  ledArea[numPorts] = new Rectangle(Integer.parseInt(param[5]), Integer.parseInt(param[6]),
                     Integer.parseInt(param[7]), Integer.parseInt(param[8]));
  ledLayout[numPorts] = (Integer.parseInt(param[5]) == 0);
  numPorts++;
  
  if (verbose) println("serialConfigure end");
}

///////////////////////////////////
// draw runs every time the screen is redrawn - show the movie...
void draw() {
  
  syphonEvent();
  
  // show the original video // *** modified to canvas /// unsure if this is correct
  //image(canvas, 0, 80);
  
  //get syphon frame
  if (MyClient.available()) {
    canvas = MyClient.getGraphics(canvas);
    image(canvas, 0, 0, width, height);  
  }
  
  //if (verbose) println("draw");
  
  // then try to show what was most recently sent to the LEDs
  // by displaying all the images for each port.
  for (int i=0; i < numPorts; i++) 
  {
    // compute the intended size of the entire LED array
    int xsize = percentageInverse(ledImage[i].width, ledArea[i].width);
    int ysize = percentageInverse(ledImage[i].height, ledArea[i].height);
    // computer this image's position within it
    int xloc =  percentage(xsize, ledArea[i].x);
    int yloc =  percentage(ysize, ledArea[i].y);
    // show what should appear on the LEDs
    image(ledImage[i], 240 - xsize / 2 + xloc, 10 + yloc);
  }
  
  textFont(font, 12);
  text(typedText, 35, 45);
   
}

// scale a number by a percentage, from 0 to 100
int percentage(int num, int percent) {
  double mult = percentageFloat(percent);
  double output = num * mult;
  return (int)output;
}

// scale a number by the inverse of a percentage, from 0 to 100
int percentageInverse(int num, int percent) {
  double div = percentageFloat(percent);
  double output = num / div;
  return (int)output;
}

// convert an integer from 0 to 100 to a float percentage
// from 0.0 to 1.0.  Special cases for 1/3, 1/6, 1/7, etc
// are handled automatically to fix integer rounding.
double percentageFloat(int percent) {
  if (percent == 33) return 1.0 / 3.0;
  if (percent == 17) return 1.0 / 6.0;
  if (percent == 14) return 1.0 / 7.0;
  if (percent == 13) return 1.0 / 8.0;
  if (percent == 11) return 1.0 / 9.0;
  if (percent ==  9) return 1.0 / 11.0;
  if (percent ==  8) return 1.0 / 12.0;
  return (double)percent / 100.0;
}

void keyPressed() 
{
  // do not let the ESC key to close the app
  if (key == ESC) {
    key = 0;  // Fools! don't let them escape!
  }
  else if ((key >= '0') && (key <= '9')) {
    // try to connect to the serial 
    if (verbose) log(key);
    if ((key - 0x30) < Serial.list().length)
    {
      if (verbose) println("serialConfigure just before");
      serialConfigure(Serial.list()[key - 0x30]);
    }
  }
}

// load the csv information into the 3 strips
/*
void loadPixelsMappingFile()
{
  if (verbose) println("loadPixelsMappingFile");
  tablePixelsMap = loadTable("pixels.csv", "header");

  int columnsCount = tablePixelsMap.getColumnCount();
  int rowsCount = tablePixelsMap.getRowCount();
  if (verbose) println("columns: " + columnsCount + " rows:" + rowsCount);
  
  init_strips();
  
  for (int w=0; w < columnsCount; w++) 
  {
    for (int h=0; h < rowsCount; h++)
    {
      
      int i = tablePixelsMap.getInt(h,w);
      //println("got i " + i);
      if ( (i > 0) && (i <= STRIP_LENGTH) )
      {
        // put the information into the correct strip
        // anna
        if (w < 8)
        {
          strip1[i-1].x = w;
          strip1[i-1].y = h;
        }
        // louise
        else if ((w >= 8) && (w < 16))
        {
          strip2[i-1].x = w;
          strip2[i-1].y = h;          
        }
        // bruno
        else // w >= 16
        {
          strip3[i-1].x = w;
          strip3[i-1].y = h;          
        }
      }
      
    }
  }
}

public void init_strips()
{
  for (int i = 1; i <= STRIP_LENGTH; i++)
  {
    strip1[i-1] = new MyLocation();
    strip2[i-1] = new MyLocation();
    strip3[i-1] = new MyLocation();
  }
}
*/

