const int motorAPin = 9;
const int motorBPin = 10;
//val citita de senzori
int sensorValueL = 0;
int sensorValueC = 0;
int sensorValueR = 0;

void setup() {
  // pinii motoarelor A si B
  pinMode(motorAPin, OUTPUT);
  pinMode(motorBPin, OUTPUT);
  
  //pinii de la senzorul IR
  pinMode(A5,INPUT);
  pinMode(A4,INPUT);
  pinMode(A3,INPUT);

  Serial.begin(9600);
}

void loop() {
  //afisam valoarea senzorilor
  Serial.print("Right sensor:"); 
  Serial.println(analogRead(A5));

  Serial.print("Center sensor:"); 
  Serial.println(analogRead(A4));

  Serial.print("Left sensor:"); 
  Serial.println(analogRead(A3));

  sensorValueR = analogRead(A5);
  sensorValueC = analogRead(A4);
  sensorValueL = analogRead(A3);

  if(sensorValueC < 900){
    digitalWrite(motorAPin, HIGH);
    digitalWrite(motorBPin, HIGH);
    }
    
  if(sensorValueR<900){
    //verifica daca senzorul stang e pe banda neagra
    digitalWrite(motorAPin, HIGH);
    digitalWrite(motorBPin, LOW);
    }
    
   if(sensorValueL<900){
    //verifiva daca senzorul stang e pe banda neagra
    digitalWrite(motorAPin, LOW);
    digitalWrite(motorBPin, HIGH);
    }
    
    if(sensorValueC < 900 && sensorValueR<900 && sensorValueL<900){
    //mergi in dreapta pana gasesti iar banda neagra 
    digitalWrite(motorAPin, HIGH);
    digitalWrite(motorBPin, LOW);
    }
    Serial.print("\nMOTOR A :"); 
    Serial.println(digitalRead(motorAPin));
    
    Serial.print("\nMOTOR B :"); 
    Serial.println(digitalRead(motorBPin));
   
  
}
