http://www.instructables.com/id/Arduino-Simon-Says/?ALLSTEPS
Game using an Arduino Micro Controller. 

Materials Needed:
    Arduino (I used a Nove)
    Bread Board
    Jumper Wires
    4 different colored LEDs
    4 100 Ohm Resistors
    4 Push Buttons
    Small Speaker

Step 1: Hardware setup

For this setup we will be taking advantage of the internal pull up resistors that the Adruino Nove and newer have built in.

First you'll want to lay out your buttons on the bottom section of your Breadboard. We'll tie one lead of the buttons directly to ground. 
The other lead will be wired to the Arduino's Digital pins 2, 3, 4, and 5. 
I ran a wire to the top half of the board to keep the wires running from the board to the Arduino out of the way. 

Now we want to lay out the LEDs above the buttons. 
We'll plug the LEDs in with the switches so the Cathode is on the same strip as the side of the button that is tied to ground. 
Well arrange it so the Anode plugs in off to the side of the button. 
Now we'll take out 100 Ohm resistor and put it inline with the Anodeto the top section of the breadboard. 
Now we can connect a wire from the resistor to the Arduino.  The LEDs will be using Digital pins 8, 9, 10, and 11. 
So make sure the buttons and LED are lined up as follows:

    Button on pin 2 with LED on pin 8
    Button on pin 3 with LED on pin 9
    Button on pin 4 with LED on pin 10
    Button on pin 5 with LED on pin 11

Finally we connect the Speaker so that the black wire is to ground and the red wire is on pin 12 through. 
You can add a 10K potentiometer in line with the 1K resistor to give yourself a volume control. 
But i found the speaker is just load enough as is. Lets not forget to supply a ground wire between the Arduino and the breadboard. 
Step 2: The Sketch
The sketch was originally made by Robert Spann back in 2009. 
I used an earlier version of his sketch since i didn't like the way he integrated sound in later versions of the sketch. 
To give the game sound i decided to use the Tone.h Library. 
The Library hasn't been updated since the release of Arduino 1.0, so i went ahead and updated it to be compatible with Arduino 1.0. 
In doing so i cut out any backwards compatibility with older IDEs. 
So if your running an older version of the IDE then used the link in the next step to download the original library. 
I've attached a copy of the updated library on this page. 
So be sure to download it and place it in your Arduino/Libraries folder or the sketch will not compile under Arduino 1.0.