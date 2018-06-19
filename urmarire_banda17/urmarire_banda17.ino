#include <Bridge.h>
#include <Console.h>
#include <FileIO.h>
#include <HttpClient.h>
#include <Mailbox.h>
#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>

#include <RedBot.h>
#include <RedBotSoftwareSerial.h>
//citirea valorilor cu cei 3 senzori IR
RedBotSensor left_sen = RedBotSensor(A3);   
RedBotSensor center_sen = RedBotSensor(A4); 
RedBotSensor right_sen = RedBotSensor(A5); 

//initializare variabile
int lineStandard = 800;
int delay_motor = 200;
int speed_motor = 200;
int direction_robot = 0;

#include <Servo.h> 
int pinLB = 12;     
int pinLF = 3;      
int pinRB = 13;    
int pinRF = 11;     

int inputPin = 4;    // definire pin senzor Echo
int outputPin = 5;    // definire pin senzor Trig

const int buzzer = 9; // definire pin Buzzer

Servo myservo;        
int delay_time = 250; //timp de intarziere
void setup()
 {
  Serial.begin(9600);      
  pinMode(pinLB,OUTPUT); // pin 12
  pinMode(pinLF,OUTPUT); // pin 3 (PWM)
  pinMode(pinRB,OUTPUT); // pin 13
  pinMode(pinRF,OUTPUT); // pin 11 (PWM) 
  pinMode(inputPin, INPUT);    // definire pin de intrare al senzorului
  pinMode(outputPin, OUTPUT);  // definire pin de iesire al senzorului 
  pinMode(buzzer,OUTPUT);      //definire pin de iesire al buzzerului  
  myservo.attach(9);  
 }  
 
int distance()
    {
    long duration;
    int distance;
    //curata pinul Trig
    digitalWrite(outputPin,LOW);
    delayMicroseconds(2);
    
   // seteaza pinul trig pe nivel HIGH pentru 10 micro secunde
    digitalWrite(outputPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(outputPin, LOW);
    
    //citeste pinul Echo, ce returneaza unda transmisa
    duration = pulseIn(inputPin, HIGH);
    
    //calculeaza distanta
    distance = duration*0.034/2;
    return distance;    
    }
    
int advance(int pwm_advance)     // mergi inainte
    { 
    digitalWrite(pinLB, LOW);    // roata dreapta merge inainte
    digitalWrite(pinRB, LOW);    // roata stanga merge inainte
    analogWrite(pinLF,pwm_advance);   //puterea rotii drepte(0-255) PWM
    analogWrite(pinRF,pwm_advance);   //puterea rotii stangi(0-255) PWM
    }
    
    
void stopp()         // sta pe loc
    {
     digitalWrite(pinLB,HIGH);  //roata dreapta merge inapoi
     digitalWrite(pinRB,HIGH);  //roata stanga merge inapoi
     analogWrite(pinLF,0);      //puterea este 0
     analogWrite(pinRF,0); 
    }
    
    
void right(int pwm_right)        // merge la dreapta
    {
   digitalWrite(pinLB,LOW);            //roata drepta merge inainte
   digitalWrite(pinRB,LOW);           //roata drepta merge inainte
   analogWrite(pinLF, pwm_right);      //puterea rotii drepte(0-255) PWM
   analogWrite(pinRF, speed_motor);    //puterea rotii data de valoarea initiala
    }
    
    
void left(int pwm_left)         // merge la stanga
    {
   digitalWrite(pinLB,LOW);          //roata drepta merge inainte
   digitalWrite(pinRB,LOW);          //roata drepta merge inainte 
   analogWrite(pinLF, speed_motor);
   analogWrite(pinRF, pwm_left);  
    }
  
  
void back(int pwm_back)          // merge inapoi
    {
     digitalWrite(pinLB,HIGH);      // motorul drept merge inapoi
     digitalWrite(pinRB,HIGH);      // motorul stang merge inapoi
     analogWrite(pinLF,pwm_back);   //puterea rotii drepte(0-255) PWM
     analogWrite(pinRF,pwm_back);   //puterea rotii drepte(0-255) PWM
    }


void return180degrees()
    {
//     digitalWrite(pinLB,LOW);  // motor moves to left rear
//     digitalWrite(pinRB,HIGH);  // motor moves to right rear
//     analogWrite(pinLF,255);  
//     analogWrite(pinRF,255); 
//     delay(1600);
//     Serial.print("Rotate 180 degrees \n");
    }

//functie de mers la stanga pentru 1,1 secunde
void move_left()
    {
     digitalWrite(pinLB,LOW);  
     digitalWrite(pinRB,HIGH);  
     analogWrite(pinLF,255);  
     analogWrite(pinRF,120); 
     delay(1100);
     Serial.print("Move to left \n");
    }

//functia de mers la dreapta pentru 1,1 secunde
void move_right()
    {
     digitalWrite(pinLB,HIGH);  
     digitalWrite(pinRB,LOW);  
     analogWrite(pinLF,120);  
     analogWrite(pinRF,255); 
     delay(1100);
     Serial.print("Move to right \n");
    }
    
//functia de mers inainte pentru 0.5 secunde    
void move_forward()
    {
     digitalWrite(pinLB,LOW);  
     digitalWrite(pinRB,LOW);  
     analogWrite(pinLF,speed_motor);  
     analogWrite(pinRF,speed_motor); 
     delay(500);
     Serial.print("Move forward \n");
    }
    
void follow_line()
    {
      //afisarea valorilor citite de pe senzorul IR
  //Serial.print(left_sen.read());
  //Serial.print("\t");  
  //Serial.print(center_sen.read());
  //Serial.print("\t"); 
  //Serial.print(right_sen.read());
  
  Serial.print("follow_line \n");
  Serial.println();
   myservo.write(90);  
  
  int i;
  int counter = 0;
  int pwm_right;
  int pwm_left;
  int distance_sensor;
  
  //inaintare(L>800 && C<800 &&  R>800)
  //in momentul in care senzorul central este pe zona neagra acesta va returna o valoare mai mica de 800
  //ceea ce va face ca aplicatia sa mearga inainte 
  if((left_sen.read() > lineStandard) && (center_sen.read() < lineStandard) &&  (right_sen.read() > lineStandard)){ 
      while((left_sen.read() > lineStandard) && (center_sen.read() < lineStandard) &&  (right_sen.read() > lineStandard))
      {    
      advance(speed_motor);
      
      distance_sensor = distance();
      while(distance_sensor < 10)
          {
          stopp();
          distance_sensor = distance();
          tone(buzzer, 1000);
          }
       noTone(buzzer);
      }
    
    //dreapta(L>800 && C>800 &&  R<800) || (L>800 && C<800 &&  R<800)
    //in momentul in care banda neagra se va gasi pe senzorul din dreapta  sau pe cel central si cel din dreapta
    //aplicatia va merge la dreapta
  } else if(((left_sen.read() > lineStandard) && (center_sen.read() > lineStandard) &&  (right_sen.read() < lineStandard)) || ((left_sen.read() > lineStandard) && (center_sen.read() < lineStandard) &&  (right_sen.read() < lineStandard)))
      { 
      pwm_right = speed_motor;
      counter = 0;
      //in aceasta situatia s-a utilizat o metoda pentru o variere inceata a robotului, valoarea PWM va creste treptat in functie de cat timp necesita 
      //pentru modificarea directiei. 
      //la inceput PWM va creste cu cate o val, dupa care din 10 in 10, dupa o perioada de tipm
      //ceea ce inseamna ca necesita o curba mai puternica
      while(((left_sen.read() > lineStandard) && (center_sen.read() > lineStandard) &&  (right_sen.read() < lineStandard)) || ((left_sen.read() > lineStandard) && (center_sen.read() < lineStandard) &&  (right_sen.read() < lineStandard)))
          {
          right(pwm_right);
          pwm_right--;
          counter++;
          if(counter >= 10)
               pwm_right = pwm_right-10;
          if(pwm_right<20)
               pwm_right = 0;
          distance_sensor = distance();
          while(distance_sensor < 10)
              {
              stopp();
              distance_sensor = distance();
              tone(buzzer, 1000);
              }               
           noTone(buzzer);
        }
     counter = 0;
     direction_robot = 0;        
     }

  
  //stanga(L<800 && C>800 &&  R>800) || (L<800 && C<800 &&  R> 800)
  //in momentul in care banda neagra se va gasi pe senzorul din stanga  sau pe cel central si cel din stanga
  //aplicatia va merge catre stanga
  else if(((left_sen.read() < lineStandard) && (center_sen.read() > lineStandard) && (right_sen.read() > lineStandard)) || ((left_sen.read() < lineStandard) && (center_sen.read() < lineStandard) &&  (right_sen.read() > lineStandard)))
      {
       pwm_left = speed_motor;
       counter = 0;
      //in aceasta situatia s-a utilizat o metoda pentru o variere inceata a robotului, valoarea PWM va creste treptat in functie de cat timp necesita 
      //pentru modificarea directiei. 
      //la inceput PWM va creste cu cate o val, dupa care din 10 in 10, dupa o perioada de tipm
      //ceea ce inseamna ca necesita o curba mai puternica
       while(((left_sen.read() < lineStandard) && (center_sen.read() > lineStandard) && (right_sen.read() > lineStandard)) || ((left_sen.read() < lineStandard) && (center_sen.read() < lineStandard) &&  (right_sen.read() > lineStandard)))
           {
            left(pwm_left);
            //delay(5);
            pwm_left--;
            counter++;
            if(counter >= 10)
                pwm_left = pwm_left - 10;
            if(pwm_left<20)
                pwm_left = 0;
            distance_sensor = distance();
            while(distance_sensor < 10)
                {
                stopp();
                distance_sensor = distance();
                tone(buzzer, 1000);
                }
            noTone(buzzer);
           }
       counter = 0;
       direction_robot = 1;
      }

  
  //(L>800 && C>800 && R>800)
  // in momentul in caer robotul paraseste linia neagra, acesta memoreaza ultima directie din cauza curbei mult prea abrupte
  // si va continua sa avanseze pe acea directie 

  else if(((left_sen.read() > lineStandard) && (center_sen.read() > lineStandard) && (right_sen.read() > lineStandard)))
      {
      if(direction_robot == 0)
          {
          right(0);
          distance_sensor = distance();
          while(distance_sensor < 10)
              {
              stopp();
              distance_sensor = distance();
              tone(buzzer, 1000);
              }
          noTone(buzzer);
          }
      else
          {
          left(0);
          distance_sensor = distance();
          while(distance_sensor < 10)
              {
              stopp();
              distance_sensor = distance();
              tone(buzzer, 1000);
              }
          noTone(buzzer);  
          }
      delay(delay_motor);
      }
  //(L<800 && C<800 && R<800)
  // in acest caz, toti 3 senzorii citesc o valoare foarte mica, sub 800
  //aplicatia trebuie sa se opreasca in acest caz
  else if((left_sen.read() < lineStandard) && (center_sen.read() < lineStandard) && (right_sen.read() < lineStandard))
      {
      stopp();
      }
   
}


//in functia void loop am creat acele cazuri posibile transmise prin modulul bluetooth
//pentru fiecare caz in parte, exista un anumit caz de executat diferentiat de celelalte
void loop()
    {    
    int data = 0;
    
    if(Serial.available() > 0)      //trimite informatii doar in momentul in care receptioneaza
       {
        data = Serial.read();        //citeste informatia si o atribui variabilei data
        Serial.print(data);          //afisarea valorii pe ecran
        Serial.print(" \n");
        
        //in aceste cazuri robotul este programat sa urmareasca linia neagra, sa ia o decizie in momentul in care intalneste o intersectie
        //sa mearga in stanga, dreapta sau inainte in functie de caz si sa se opreasca la locatia dorita
        // se va folosi de functiile follow_line(), move_left(), move_right()si stopp()
        //in momentul in care se va citi valoarea 0, robotul se opreste
        switch(data)
            {
              case 12:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line(); 
                  move_left();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  stopp();
                  data = 0;
                  break;
                  }
              
              case 13:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line(); 
                  move_right();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  stopp();
                  data = 0;
                  break;
                  }
              case 14:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line(); 
                  stopp();
                  delay(500);
                  move_forward();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  move_right();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  data = 0;
                  break;
                  }
                  
              case 15:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line(); 
                  move_forward();
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  move_left();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  data = 0;
                  break;
                  }
              
              case 21:
                 {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line(); 
                  move_right();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  stopp();
                  data = 0;
                  break;
                  }
              
              case 23:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  move_forward();
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);    
                  stopp();
                  data = 0;  
                  break;
                  }
                  
              case 24:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  move_left();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  move_right();
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);    
                  stopp();
                  data = 0;
                  break;
                  }
              case 25:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  move_left();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  move_left();
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);    
                  stopp();
                  data = 0;
                  break;
                  }
              
              case 31:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line(); 
                  move_left();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  stopp();
                  data = 0;
                  break;
                  }
              
              case 32:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  move_forward();
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);    
                  stopp();
                  data = 0;  
                  break;
                  }
              
              case 34:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  move_right();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  move_right();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();    
                  stopp();
                  data = 0;
                  break;
                  }
              
              case 35:
                {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  move_right();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  move_left();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();    
                  stopp();
                  data = 0;
                  break;
                  }
              
              case 41:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  move_left();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  move_forward();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();    
                  stopp();
                  data = 0;
                  break;
                  }
              
              case 42:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  move_left();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  move_right();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();    
                  stopp();
                  data = 0;
                  break;
                  }
              
              case 43:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  move_left();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  move_left();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();    
                  stopp();
                  data = 0;
                  break;
                  }
              
              case 45:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  move_forward();
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);    
                  stopp();
                  data = 0;  
                  break;
                  } 
              
              case 51:
              {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  move_right();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  move_forward();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();    
                  stopp();
                  data = 0;
                  break;
                  }
              
              case 52:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  move_right();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  move_right();
                  stopp();
                  delay(500); 
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();   
                  stopp();
                  data = 0;
                  break;
                  }
              
              case 53:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  move_right();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);
                  move_left();
                  stopp();
                  delay(500);
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();    
                  stopp();
                  data = 0;
                  break;
                  }
              
              case 54:
                  {
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  move_forward();
                  while((left_sen.read()>100 || center_sen.read()>100 || right_sen.read()>100) && !(left_sen.read()>100 && center_sen.read()>100 && right_sen.read()>100))
                      follow_line();
                  stopp();
                  delay(500);    
                  stopp();
                  data = 0;  
                  break;
                  } 
              
              case 0:
                  {
                  stopp();
                  break;
                  }
            }
       }
    
    }
