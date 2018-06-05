const int switchPin = 2; //the number of the switch pin
const int motorPin  = 9; //the number of the motor pin

int switchState = 0; //variable for reading the switch's status
int sensor_value = 0;

void setup() {
  //initailize the motor pin as an output
  pinMode(motorPin, OUTPUT);
  //initialize the switch pin as an input
  pinMode(switchPin,INPUT);

  pinMode(A0,INPUT);     // receive signal from the sensor
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(A0)); //print voltage to outuput pin (resolution 1-1024) 
  

  //check the value of the sensor
  sensor_value = analogRead(A0);
  Serial.println(sensor_value);
  if(sensor_value < 500 ){
    digitalWrite(motorPin, HIGH);
    }
    else{
    //turn motor off:
    digitalWrite(motorPin, LOW);  
    }
 
}
