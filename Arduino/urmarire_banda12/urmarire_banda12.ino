#include <RedBot.h>
#include <RedBotSoftwareSerial.h>

RedBotSensor left_sen = RedBotSensor(A3);   
RedBotSensor center_sen = RedBotSensor(A4); 
RedBotSensor right_sen = RedBotSensor(A5); 

int lineStandard = 800;
int delay_motor = 200;
#include <Servo.h> 
int pinLB = 12;     // define pin 12 
int pinLF = 3;     // define pin 3 
int pinRB = 13;    // define pin 13 
int pinRF = 11;    // define pin 11 
////////////////////////////////
int inputPin = 4;    // define pin for sensor echo
int outputPin =5;    // define pin for sensor trig

int Fspeedd = 0;      // forward speed
int Rspeedd = 0;      // right speed
int Lspeedd = 0;      // left speed
int directionn = 0;   // forward=8 backward=2 left=4 right=6 
Servo myservo;        // set myservo
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
  pinMode(inputPin, INPUT);    // define input pin for sensor
  pinMode(outputPin, OUTPUT);  // define output pin for sensor   
  myservo.attach(9);    // Define servo motor output pin to D9 (PWM)
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
   Serial.print(left_sen.read());
  Serial.print("\t");  
  Serial.print(center_sen.read());
  Serial.print("\t"); 
  Serial.print(right_sen.read());
  Serial.println();
   myservo.write(90);  // home set the servo motor, ready for next measurement
  if((center_sen.read() < lineStandard) && (left_sen.read() > lineStandard) && (right_sen.read() > lineStandard)){ //if C<800 and L>800 and R>800 advance
    advance();
    Serial.print("advance \n");
    delay(delay_motor);
    //(c>800 && L>800 && R<800) || (C<800 L>800  R<800)
  } else if(((center_sen.read() > lineStandard) && (left_sen.read() > lineStandard) && (right_sen.read() < lineStandard)) || ((center_sen.read() < lineStandard) && (left_sen.read() > lineStandard) && (right_sen.read() < lineStandard)))
  {
   right();
   Serial.print("right \n");
   delay(delay_motor);
  }
   //(c>800 && L<800 && R>800) || (C<800 L<800  R> 800)
  else if(((center_sen.read() > lineStandard) && (left_sen.read() < lineStandard) && (right_sen.read() > lineStandard)) || ((center_sen.read() < lineStandard) && (left_sen.read() < lineStandard) && (right_sen.read() > lineStandard)))
  {
   left();
   Serial.print("left \n");
   delay(delay_motor); 
  }
  else if(((left_sen.read() > lineStandard) && (center_sen.read() > lineStandard) && (right_sen.read() > lineStandard))||((left_sen.read() < lineStandard) && (center_sen.read() < lineStandard) && (right_sen.read() < lineStandard)) )
  {
    advance();
    Serial.print("advance \n");
    delay(delay_motor);
  }
   
   }
