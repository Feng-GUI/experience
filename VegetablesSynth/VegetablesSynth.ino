#include <MovingAvarageFilter.h>
#include "MIDI.h"

#define MIDI_CHANNEL_OUT 1

MovingAvarageFilter movingAvarageFilter1(20);
MovingAvarageFilter movingAvarageFilter2(20);
MovingAvarageFilter movingAvarageFilter3(20);
MovingAvarageFilter movingAvarageFilter4(20);
MovingAvarageFilter movingAvarageFilter5(20);
 
boolean check1 = false;
boolean check2 = false;
boolean check3 = false;
boolean check4 = false;
boolean check5 = false;
 
void setup() {
  //PERF: make sure that thru is disabled at MIDI.h
  MIDI.begin(MIDI_CHANNEL_OMNI); // Launch MIDI and listen to input channel omny (all channels)
  //Serial.begin(115200);
  //start serial with midi baudrate 31250
  //Serial.begin(31250);
}
 
void loop() {        
  // Call MIDI.read the fastest you can for real-time performance.
  // There is no need to check if there are messages incoming. They are handled at the Callback functions.
  //MIDI.read();
  
  // declare input and output variables
float input1 =  analogRead(1); 
float input2 =  analogRead(2);
float input3 =  analogRead(3);
float input4 =  analogRead(4);
float input5 =  analogRead(5);
 
float output1 = 0;
float output2 = 0;
float output3 = 0;
float output4 = 0;
float output5 = 0;
 
output1 = movingAvarageFilter1.process(input1);
output2 = movingAvarageFilter2.process(input2);
output3 = movingAvarageFilter3.process(input3);
output4 = movingAvarageFilter4.process(input4);
output5 = movingAvarageFilter5.process(input5);
 
 
  if (output1 < 990 ) {   // you can change this parameter to fine tune the sensitivity
    if (!check1){         
      //Serial.print("in1:"); 
      //Serial.println(output1);

      MIDI.sendNoteOn(36,127,MIDI_CHANNEL_OUT);
      MIDI.sendNoteOff(36,64,MIDI_CHANNEL_OUT);
      check1 = !check1;
    }
    return;
  }
 
  if (output1 >1000) {     
    //Serial.print("o2:");
    //Serial.println(output1);

    if (check1){
      check1 = !check1;
    }     
  }
 
/////////////////////////////////

 if (output2 < 990 ) {   // you can change this parameter to fine tune the sensitivity
    if (!check2){         
      //FIXME Keyboard.print("f");         
      //Serial.print("in2:"); 
      //Serial.println(output2);

      MIDI.sendNoteOn(37,127,MIDI_CHANNEL_OUT);
      MIDI.sendNoteOff(37,64,MIDI_CHANNEL_OUT);      
      check2 = !check2;
    }         
    return;
  }
 
  if (output2 >1000) {     
    //Serial.print("o2:");
    //Serial.println(output1);

    if (check2){
      check2 = !check2;   
    }     
  }
 
 ///////////////////////////////////////////////////
 
 if (output3 < 990 ) {   // you can change this parameter to fine tune the sensitivity
    if (!check3){         
      //Serial.print("in3:"); 
      //Serial.println(output3);

      MIDI.sendNoteOn(38,127,MIDI_CHANNEL_OUT);
      MIDI.sendNoteOff(38,64,MIDI_CHANNEL_OUT);
      check3 = !check3;   
    }         
    return;
  }
 
  if (output3 >1000) {     
    //Serial.print("o3:");
    //Serial.println(output3);

    if (check3){
      check3 = !check3;
    }     
  }
 
 //////////////////////////////
 
  if (output4 < 990 ) {   // you can change this parameter to fine tune the sensitivity
    if (!check4){         
      //Serial.print("in4:"); 
      //Serial.println(output4);

      MIDI.sendNoteOn(39,127,MIDI_CHANNEL_OUT);
      MIDI.sendNoteOff(39,64,MIDI_CHANNEL_OUT);
      check4 = !check4;
    }         
   return;
  }
 
  if (output4 >1000) {     
    //Serial.print("o2:");
    //Serial.println(output1);

    if (check4){
      check4 = !check4;   
    }     
  }
 
 /////////////////////////////////////
 
  if (output5 < 990 ) {   // you can change this parameter to fine tune the sensitivity
    if (!check5){         
      //Serial.print("in5:"); 
      //Serial.println(output5);

      MIDI.sendNoteOn(40,127,MIDI_CHANNEL_OUT);
      MIDI.sendNoteOff(40,64,MIDI_CHANNEL_OUT);
      check5 = !check5;
    }         
   return;   
  }
 
  if (output5 >1000) {     
    //Serial.print("o5:");
    //Serial.println(output5);

    if (check5){
      check5 = !check5;   
    }     
  }
 
 
}
