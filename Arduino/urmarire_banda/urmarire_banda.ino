#include <RedBot.h>
#include <RedBotSoftwareSerial.h>
//#include <Servo.h>

RedBotSensor left_sen = RedBotSensor(A3);   
RedBotSensor center_sen = RedBotSensor(A4); 
RedBotSensor right_sen = RedBotSensor(A5);
int lineStandard = 800;

 
int pinLB = 12;     // define pin 12 
int pinLF = 3;     // define pin 3 
int pinRB = 13;    // define pin 13 
int pinRF = 11;    // define pin 11 
////////////////////////////////


//Servo myservo;        // set myservo
int delay_time = 250; // settling time after steering servo motor moving B
int Fgo = 8;         // Move F
int Rgo = 6;         // move to the R
int Lgo = 4;         // move to the L
int Bgo = 2;         // move B
void setup()
 {
  Serial.begin(9600);     // Define motor output pin 
  pinMode(pinLB,OUTPUT); // pin 12
  pinMode(pinLF,OUTPUT); // pin 3 (PWM)
  pinMode(pinRB,OUTPUT); // pin 13
  pinMode(pinRF,OUTPUT); // pin 11 (PWM)  
  //myservo.attach(9);    // Define servo motor output pin to D9 (PWM)

 }
void advance()     // move forward
    { 
    digitalWrite(pinLB,LOW);    // right wheel moves forward
    digitalWrite(pinRB, LOW);  // left wheel moves forward
    analogWrite(pinLF,255);   
    analogWrite(pinRF,255);
    }
void stopp()         // stop 
    {
     digitalWrite(pinLB,HIGH);
     digitalWrite(pinRB,HIGH);
     analogWrite(pinLF,0);
     analogWrite(pinRF,0); 
    }
void right()        // turn right (single wheel)
    {
   digitalWrite(pinLB,HIGH);  // wheel on the left moves forward
   digitalWrite(pinRB,LOW); // wheel on the right moves backward
   analogWrite(pinLF, 255);
   analogWrite(pinRF,255);   
    }
void left()         // turn left (single wheel)
    {
   digitalWrite(pinLB,LOW);  // wheel on the left moves backward
   digitalWrite(pinRB,HIGH); // wheel on the right moves forward
   analogWrite(pinLF, 255);
   analogWrite(pinRF,255);  
    }
  
void back()          // move backward
    {
     digitalWrite(pinLB,HIGH);  // motor moves to left rear
     digitalWrite(pinRB,HIGH);  // motor moves to right rear
     analogWrite(pinLF,255);  
     analogWrite(pinRF,255);     
    }

void loop()
{
//    myservo.write(90);  // home set the servo motor, ready for next measurement
    Serial.print(left_sen.read());
    Serial.print("\t");  
    Serial.print(center_sen.read());
    Serial.print("\t"); 
    Serial.print(right_sen.read());
    Serial.println();
    //data_sheet
  // if on the line drive left and right at the same speed (left is CCW / right is CW)
  if(center_sen.read() > lineStandard)
  {//right
   right();
  }
  //data_sheet
  // if the line is under the right sensor, adjust relative speeds to turn to the right
  else if(right_sen.read() > lineStandard)
  {//left
  left();
  }
  //data_sheet
  // if the line is under the left sensor, adjust relative speeds to turn to the left
  else if(left_sen.read() > lineStandard)
  {//right
  right();
  }
  //data_sheet
  // if all sensors are on black or up in the air, stop the motors.
  // otherwise, run motors given the control speeds above.
  if((left_sen.read() > lineStandard) && (center_sen.read() > lineStandard) && (right_sen.read() > lineStandard) )
  {
    //stop
    stopp();
  }
  else
  {
  //any dir
    advance();
  }
  delay(0);  // add a delay to decrease sensitivity.
}
