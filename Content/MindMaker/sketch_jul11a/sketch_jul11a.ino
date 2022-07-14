  /* Sweep
   by BARRAGAN <http://barraganstudio.com>
   This example code is in the public domain.
  
   modified 8 Nov 2013
   by Scott Fitzgerald
   http://www.arduino.cc/en/Tutorial/Sweep
  */
  
  #include <Servo.h>
  #include <String.h>
  
  
  Servo servoFront;
  Servo servoLeft;
  Servo servoRight;
  Servo servoRear;// create servo object to control a servo
  

  int sensorValue = 0;        // value read from the pot
  int outputValue = 0;        // value output to the PWM (analog out)
  int UserInputPin = 5;       // This variable turns on an LED to the pin specified by the user
  byte leds = 0;
  int x=0;
  
  int pos = 0;    // variable to store the servo position
  
  void setup() {
    servoFront.attach(9);  // attaches the servo on pin 9 to the servo object
    servoLeft.attach(10);  // attaches the servo on pin 9 to the servo object
    servoRight.attach(11);  // attaches the servo on pin 9 to the servo object
    servoRear.attach(6);  // attaches the servo on pin 9 to the servo object
    
    Serial.begin(9600);
    while (! Serial); // Wait untilSerial is ready - Leonardo
    // initialize serial communications at 9600 bps:
      Serial.begin(9600);
  }
  
  void loop() 
  {
    updateServos();
  }


  
  void updateServos()
  {
    String output;
    char pass [] =  "777,777,777,777";
    
    
    if (Serial.available())
    {
      output = Serial.readStringUntil('\n');
      output.toCharArray(pass, 16);
      String Out1;
      String Out2;
      String Out3;
      String Out4;
      
      for (int i = 0; i <= 2; i++)
        {
          String temp = String(pass[i]);
          Out1.concat(temp); 
        }
      for (int i = 4; i <= 6; i++)
        {
          String temp = String(pass[i]);
          Out2.concat(temp); 
        }
      for (int i = 8; i <= 10; i++)
        {
          String temp = String(pass[i]);
          Out3.concat(temp); 
        }
      for (int i = 12; i <= 14; i++)
        {
          String temp = String(pass[i]);
          Out4.concat(temp); 
        }
            
        float Frontnum = Out1.toFloat();
        float Rightnum = Out2.toFloat();
        float Lightnum = Out3.toFloat();
        float Rearnum = Out4.toFloat();
        
      servoFront.write(Frontnum);
      servoLeft.write(Rightnum);
      servoRight.write(Lightnum);
      servoRear.write(Rearnum);
   
      }

  
  }
 
