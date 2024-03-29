

void readBTcmd() 
{ 
  if (BT1.available()) 
  { 
    strCmd = BT1.readString();
    BT1.flush();

    if (strCmd.equals("go"))
    {
      drive (motorSpeed, 0); 
      writeLEDs (' ', 'g', 'o', ' ');
        
    }
    else if (strCmd.equals("stop"))
    {
      driveStop();
      writeLEDs ('s', 't', 'o', 'p');
    }
    else if (strCmd.startsWith("ccw"))
    {
      driveStop();
      String degreeStr = strCmd.substring(2);
      int degree = degreeStr.toInt();
      turnCCW(motorSpeed, degree);
      writeLEDs ('c', 'c', 'w', ' ');
      drive (motorSpeed, 0); 
    }
    else if (strCmd.startsWith("cw"))
    {
      driveStop();
      String degreeStr = strCmd.substring(2);
      int degree = degreeStr.toInt();
      turnCW(motorSpeed, degree);
      writeLEDs ('c', 'w', ' ', ' ');
      drive (motorSpeed, 0); 
    }
  }
}




void checkBTcmd()  // verify if a command is received from BT remote control
 { 
    if (BT1.available()) 
    { 
      command = BT1.read();
      BT1.flush();
    }
 }


void manualCmd()
{
  switch (command)
  {
    
    case 'm': 
//      startSafe();
      Serial.print("Roomba in Safe mode");
//      BT1.print("Roomba BT Ctrl OK - Safe mode");
//      BT1.println('\n');
      command = 'f';
      playSound (3);
      break;
 
    case 'f': 
      driveStop(); //turn off both motors
      writeLEDs ('s', 't', 'o', 'p');
      state = command;
      Serial.println("command f"); 
      break;

    case 'w':  
      drive (motorSpeed, 0); 
      writeLEDs (' ', 'g', 'o', ' ');
      state = command;  
      break;

    case 'd':     
      driveRight(motorSpeed);
      writeLEDs ('r', 'i', 'g', 'h');
      break;

   case 'a': 
      driveLeft(motorSpeed);
      writeLEDs ('l', 'e', 'f', 't');
      break;
    
    case 's':  
      drive (-motorSpeed, 0);
      writeLEDs ('b', 'a', 'c', 'k');
      state = command;
      break;

    case '+': 
      if (state == 'w')
      {
        motorSpeed = motorSpeed + 10;
        if (motorSpeed > MAX_SPEED) 
        { 
          motorSpeed = MAX_SPEED;
        }  
        command = 'w';
      } else {command = state;}
      break;

    case '-': 
 
      if (state == 'w')
      {
        motorSpeed = motorSpeed - 10;
      }     
      if (motorSpeed < MIN_SPEED ) 
      { 
        motorSpeed = MIN_SPEED;
      }
      Serial.println(motorSpeed); 
      command = state;
      break;
  }
}
