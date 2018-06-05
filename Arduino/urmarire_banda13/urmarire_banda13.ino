#include <RedBot.h>
#include <RedBotSoftwareSerial.h>

RedBotSensor left_sen = RedBotSensor(A3);   
RedBotSensor center_sen = RedBotSensor(A4); 
RedBotSensor right_sen = RedBotSensor(A5); 

int lineStandard = 800;
int delay_motor = 200;
int speed_motor = 255;
int direction_robot = 0;
#include <Servo.h> 
int pinLB = 12;     // define pin 12 
int pinLF = 3;     // define pin 3 
int pinRB = 13;    // define pin 13 
int pinRF = 11;    // define pin 11 
////////////////////////////////
int inputPin = 4;    // define pin for sensor echo
int outputPin = 5;    // define pin for sensor trig

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
int advance(int pwm_advance)     // move forward
    { 
    digitalWrite(pinLB, LOW);    // right wheel moves forward
    digitalWrite(pinRB, LOW);  // left wheel moves forward
    analogWrite(pinLF,pwm_advance);   
    analogWrite(pinRF,pwm_advance);
    }
void stopp()         // stop 
    {
     digitalWrite(pinLB,HIGH);
     digitalWrite(pinRB,HIGH);
     analogWrite(pinLF,0);
     analogWrite(pinRF,0); 
    }
void right(int pwm_right)        // turn right (single wheel)
    {
   digitalWrite(pinLB,LOW);  // wheel on the left moves forward
   digitalWrite(pinRB,LOW); // wheel on the right moves backward
   analogWrite(pinLF, pwm_right);
   analogWrite(pinRF, speed_motor);   
    }
void left(int pwm_left)         // turn left (single wheel)
    {
   digitalWrite(pinLB,LOW);  // wheel on the left moves backward
   digitalWrite(pinRB,LOW); // wheel on the right moves forward
   analogWrite(pinLF, speed_motor);
   analogWrite(pinRF, pwm_left);  
    }
  
void back(int pwm_back)          // move backward
    {
     digitalWrite(pinLB,HIGH);  // motor moves to left rear
     digitalWrite(pinRB,HIGH);  // motor moves to right rear
     analogWrite(pinLF,pwm_back);  
     analogWrite(pinRF,pwm_back);     
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
  
  int i;
  int counter = 0;
  int pwm_right;
  int pwm_left;
   //advnace(L>800 && C<800 &&  R>800) 
  if((left_sen.read() > lineStandard) && (center_sen.read() < lineStandard) &&  (right_sen.read() > lineStandard)){ 
    //for(i=0;i<255;i++)
    //{
    // advance(i);
    //  delay(1);
    //}
    while((left_sen.read() > lineStandard) && (center_sen.read() < lineStandard) &&  (right_sen.read() > lineStandard))
    {    
      advance(speed_motor);
      Serial.print("advance \n"); 
    }
    
    //right(L>800 && C>800 &&  R<800) || (L>800 && C<800 &&  R<800)
  } else if(((left_sen.read() > lineStandard) && (center_sen.read() > lineStandard) &&  (right_sen.read() < lineStandard)) || ((left_sen.read() > lineStandard) && (center_sen.read() < lineStandard) &&  (right_sen.read() < lineStandard)))
  {
        pwm_right = speed_motor;
        counter = 0;
        while(((left_sen.read() > lineStandard) && (center_sen.read() > lineStandard) &&  (right_sen.read() < lineStandard)) || ((left_sen.read() > lineStandard) && (center_sen.read() < lineStandard) &&  (right_sen.read() < lineStandard)))
        {
               right(pwm_right);
               //delay(5);
               pwm_right--;
               counter++;
               if(counter >= 10)
                    pwm_right = pwm_right-10;
               Serial.print(pwm_right);
               Serial.print(" right \n");
               if(pwm_right<20)
                    pwm_right = 0;               
               
        }
        counter = 0;
        direction_robot = 0;
        
  }

  
   //left(L<800 && C>800 &&  R>800) || (L<800 && C<800 &&  R> 800)
  else if(((left_sen.read() < lineStandard) && (center_sen.read() > lineStandard) && (right_sen.read() > lineStandard)) || ((left_sen.read() < lineStandard) && (center_sen.read() < lineStandard) &&  (right_sen.read() > lineStandard)))
  {
       pwm_left = speed_motor;
       counter = 0;
       while(((left_sen.read() < lineStandard) && (center_sen.read() > lineStandard) && (right_sen.read() > lineStandard)) || ((left_sen.read() < lineStandard) && (center_sen.read() < lineStandard) &&  (right_sen.read() > lineStandard)))
       {
            left(pwm_left);
            //delay(5);
            pwm_left--;
            counter++;
            if(counter >= 10)
                pwm_left = pwm_left - 10;
            Serial.print(pwm_left);
            Serial.print(" left \n");
            if(pwm_left<20)
                pwm_left = 0;
       }
        counter = 0;
        direction_robot = 1;
  }

  
  //(L>800 && C>800 && R>800) || (L<800 && C<800 && R<800)
  else if(((left_sen.read() > lineStandard) && (center_sen.read() > lineStandard) && (right_sen.read() > lineStandard)))
  {
      if(direction_robot == 0)
      {
            right(0);
      }
      else
      {
            left(0);  
      }
      Serial.print("stop \n");
      delay(delay_motor);
  }
  else if((left_sen.read() < lineStandard) && (center_sen.read() < lineStandard) && (right_sen.read() < lineStandard))
  {
      stopp();  
  }
   
   }
