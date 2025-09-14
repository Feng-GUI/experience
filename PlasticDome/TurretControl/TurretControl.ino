/**
* 
* Turrent Control
* 2 Servos for moving the turret
* 1 Servo for firing missile
* Serial input X,Y,Z
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
    // read x,y,z from serial
    inputString = Serial.readStringUntil('\r');
    //Serial.println(inputString);

    // Find the index of the first comma
    int firstCommaIndex = inputString.indexOf(',');
    
    // Find the index of the second comma, starting after the first one
    int secondCommaIndex = inputString.indexOf(',', firstCommaIndex + 1);

    int x_axis = inputString.substring(0, firstCommaIndex).toInt();
    int y_axis = inputString.substring(firstCommaIndex + 1, secondCommaIndex).toInt();
    // 0-180
    int z_axis = inputString.substring(secondCommaIndex + 1).toInt();

    // map cam resolution to turrent angle
    int y = map(y_axis, 0, 1080, 0, 180);
    int x = map(x_axis, 0, 1920, 0, 180);

    // rotate to aim
    servoLeftRight.write(x);
    servoUpDown.write(y);
    
    // rotate to arm or fire
    servoFire.write(z_axis);

    // debug print
    //Serial.println(x);
    //Serial.print(",");
    //Serial.println(y);
    
  }
}

