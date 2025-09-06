/**
* 
* Turrent Control
* 2 Servos for moving the turret
* 1 Servo for firing missile
* Serial input X,Y
* 
* Servo orange is signal 
* 
*/

#include <Servo.h>

Servo servoLeftRight;
Servo servoUpDown;
Servo servoFire;

String inputString;

void setup()
{
  servoLeftRight.attach(2);
  servoUpDown.attach(3);
  servoFire.attach(4);
  Serial.begin(9600);
}

void loop()
{
  while(Serial.available())
  {
    // read x,y from serial
    inputString = Serial.readStringUntil('\r');
    Serial.println(inputString);
    int x_axis = inputString.substring(0, inputString.indexOf(',')).toInt();
    int y_axis = inputString.substring(inputString.indexOf(',') + 1).toInt();

    // map cam resolution to turrent angle
    int y = map(y_axis, 0, 1080, 0, 180);
    int x = map(x_axis, 0, 1920, 0, 180);

    servoLeftRight.write(x);
    servoUpDown.write(y);
    
    // rotate to fire
    //servoFire.write(180);

    // waits for the servos to reach their position before rotating back to start position
    //delay(1000);
    // rotate back to start position
    //servoLeftRight.write(0);
    //servoUpDown.write(0);
    //servoFire.write(0);

    // debug print
    Serial.println(x);
    Serial.print(",");
    Serial.println(y);
    
  }
}

