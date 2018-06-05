
void setup() {
 pinMode(A0,INPUT);     // receive signal from the sensor
 
 pinMode(11,OUTPUT);    //LED pin
 pinMode(10,OUTPUT);    //LED pin


Serial.begin(9600); //
}

void loop() {
  Serial.println(analogRead(A0)); //print voltage to outuput pin (resolution 1-1024)
  delay(1000); //wait a second
  
  if(analogRead(A0)<700)
  digitalWrite(11,HIGH); 
  else
  digitalWrite(11,LOW);
}

