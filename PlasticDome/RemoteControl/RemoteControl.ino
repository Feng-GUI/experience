int forwardsPin = 3;
int backwardsPin = 2;
int leftPin = 4;
int rightPin = 5;

int incomingByte = 0;

bool forwardsPressed = false;
bool backwardsPressed = false;
bool leftPressed = false;
bool rightPressed = false;

const int FORWARDS_PRESSED = 1;
const int FORWARDS_RELEASED = 2;
const int BACKWARDS_PRESSED = 3;
const int BACKWARDS_RELEASED = 4;
const int LEFT_PRESSED = 5;
const int LEFT_RELEASED = 6;
const int RIGHT_PRESSED = 7;
const int RIGHT_RELEASED = 8;

void detectKeyPresses() {
  if (incomingByte == FORWARDS_PRESSED) {
    forwardsPressed = true;
  }
  else if (incomingByte == BACKWARDS_PRESSED) {
    backwardsPressed = true;
  }
  else if (incomingByte == RIGHT_PRESSED) {
    rightPressed = true;
  }
  else if (incomingByte == LEFT_PRESSED) {
    leftPressed = true;
  }
  else if (incomingByte == FORWARDS_RELEASED) {
    forwardsPressed = false;
  }
  else if (incomingByte == BACKWARDS_RELEASED) {
    backwardsPressed = false;
  }
  else if (incomingByte == RIGHT_RELEASED) {
    rightPressed = false;
  }
  else if (incomingByte == LEFT_RELEASED) {
    leftPressed = false;
  }
  
}

void handlePinOutputs() {
  if (forwardsPressed == true) {
    digitalWrite(forwardsPin, HIGH);
  }
  else {
    digitalWrite(forwardsPin, LOW);
  }

  if (backwardsPressed == true) {
    digitalWrite(backwardsPin, HIGH);
  }
  else {
    digitalWrite(backwardsPin, LOW);
  }

  if (rightPressed == true) {
    digitalWrite(rightPin, HIGH);
  }
  else {
    digitalWrite(rightPin, LOW);
  }

  if (leftPressed == true) {
    digitalWrite(leftPin, HIGH);
  }
  else {
    digitalWrite(leftPin, LOW);
  }
}

void setup() {
  pinMode(forwardsPin, OUTPUT);
  pinMode(backwardsPin, OUTPUT);
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);

//  digitalWrite(forwardsPin, HIGH);
//  delay(1000);
//  digitalWrite(forwardsPin, LOW);
//  delay(1000);
//  digitalWrite(forwardsPin, HIGH);
//  delay(1000);
//  digitalWrite(forwardsPin, LOW);

  allOff();
  eight();
  Serial.begin(115200);

}

void loop() {
  eight();
  if (Serial.available() > 0) {
    delay(10);
    incomingByte = Serial.parseInt();//Serial.read());
    detectKeyPresses();
    handlePinOutputs();
    Serial.print(incomingByte);
  }
}

void allOff()
{
  digitalWrite(forwardsPin, LOW);
  digitalWrite(backwardsPin, LOW);
  digitalWrite(leftPin, LOW); 
  digitalWrite(rightPin, LOW);
}

void eight()
{
  // forward and left for 1 second
  digitalWrite(forwardsPin, HIGH);
  digitalWrite(rightPin, LOW);
  digitalWrite(leftPin, HIGH);
  delay(2000);

  // right
  digitalWrite(leftPin, LOW);
  digitalWrite(rightPin, HIGH);
  delay(2000); 
}
