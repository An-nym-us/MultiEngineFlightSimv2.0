  /* Sweep
   by BARRAGAN <http://barraganstudio.com>
   This example code is in the public domain.
  
   modified 8 Nov 2013
   by Scott Fitzgerald
   http://www.arduino.cc/en/Tutorial/Sweep
  */
  
  #include <Servo.h>
  #include <String.h>
  
  
  Servo myservo;  // create servo object to control a servo
  // twelve servo objects can be created on most boards
  int sensorValue = 0;        // value read from the pot
  int outputValue = 0;        // value output to the PWM (analog out)
  int UserInputPin = 5;       // This variable turns on an LED to the pin specified by the user
  byte leds = 0;
  int x=0;
  
  int pos = 0;    // variable to store the servo position
  
  void setup() {
    myservo.attach(9);  // attaches the servo on pin 9 to the servo object
    Serial.begin(9600);
    while (! Serial); // Wait untilSerial is ready - Leonardo
    Serial.println("Enter LED Number 0 to 7 or 'x' to clear");
    // initialize serial communications at 9600 bps:
    Serial.begin(9600);
  }
  
  void loop() {
    updateServos();

  }
  
  void updateServos()
  {
     Serial.print("sensor = ");
    Serial.print(sensorValue);
    
  
  String output;
  char pass [] =  "777,777,777,777";
    
  
     if (Serial.available())
      {
          output = Serial.readStringUntil('\n');
       // Serial.println("\t output = ");
         // Serial.println(output);
  
          
      //char buf[sizeof(sz)];
     // serialResponse.toCharArray(buf, sizeof(buf));
     // char *p = buf;
      //char *str;
      output.toCharArray(pass, 16);
      String Out1;
      String Out2;
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
        
    float Fnum = Out1.toFloat();
    float Bnum = Out2.toFloat();
    
    myservo.write(Fnum);
    /*
        for (pos = 0; pos <= 180; pos += Fnum) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15ms for the servo to reach the position
        }
  
        
        for (pos = 180; pos >= 0; pos -= Bnum) { // goes from 180 degrees to 0 degrees
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(15);                       // waits 15ms for the servo to reach the position
        }
  */
        
        /*
        char ch = Serial.read();
        if (ch >= '0' && ch <= '7')
        {
          
          int led = ch - '0';
          UserInputPin=led;
          bitSet(leds, led);
          //updateLEDsON();
          Serial.print("Turned on LED ");
          Serial.println(led);
        }
        if (ch == 'x')
        {
          leds = 0;
          //updateLEDsOFF();
          Serial.println("Cleared");
        }
        */
      }
    //delay(4);
  
  }
 
