100k Ohm Potentiometer
220-470 ohm resistor

http://playground.arduino.cc/Main/MQGasSensors

* If we connect the MQ-3 directly to the arduino , there is a danger of frying it
* Sensor pin B needs to be connected to VCC, not GND. 

const int analogPin = 0; // the pin that the potentiometer is attached to
int sensorReading = analogRead(analogPin);


The preferred wiring is to connect both 'A' pins together and both 'B' pins together. 
It is safer and it is assumed that is has more reliable output results. 
Although many schematics and datasheets show otherwise, 
you are advised to connect both 'A' pins together and connect both 'B' pins together.