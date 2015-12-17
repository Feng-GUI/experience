/* MQ-3 Alcohol Sensor Circuit with Arduino */

const int A1_PIN=1;//the AOUT pin of the alcohol sensor goes into analog pin A0 of the arduino
const int A2_PIN=2;
const int A3_PIN=3;
const int A4_PIN=4;

const int DOUTpin=8;//the DOUT pin of the alcohol sensor goes into digital pin D8 of the arduino
const int ledPin=13;//the anode of the LED connects to digital pin D13 of the arduino

int limit;
int v1,v2,v3,v4;

void setup() {
Serial.begin(115200);//sets the baud rate
//pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
//pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino
  
  // initialize control over the keyboard:
  Keyboard.begin();
}

void loop()
{
v1 = analogRead(A1_PIN);//reads the analaog value from the alcohol sensor's AOUT pin
v2 = analogRead(A2_PIN);
v3 = analogRead(A3_PIN);
v4 = analogRead(A4_PIN);

//limit = digitalRead(DOUTpin);//reads the digital value from the alcohol sensor's DOUT pin
//Serial.print(" Alcohol value: ");

if (v1 > 90) Keyboard.write(KEY_LEFT_ARROW);

Serial.print(v1); Serial.print(" ");
Serial.print(v2); Serial.print(" ");
Serial.print(v3); Serial.print(" ");
Serial.print(v4); Serial.print(" ");
Serial.println("");
//Serial.print("Limit: ");
//Serial.print(limit);//prints the limit reached as either LOW or HIGH (above or underneath)
delay(100);
//if (limit == HIGH){
//  digitalWrite(ledPin, HIGH);//if limit has been reached, LED turns on as status indicator
//}
//else{
//  digitalWrite(ledPin, LOW);//if threshold not reached, LED remains off
//}

}
