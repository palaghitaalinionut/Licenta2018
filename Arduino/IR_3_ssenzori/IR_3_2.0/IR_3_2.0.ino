
void setup() {
  // put your setup code here, to run once:
pinMode(A3,INPUT);
pinMode(A4,INPUT);
pinMode(A5,INPUT);

Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

Serial.print("Left senzor:");
Serial.println(analogRead(A3));
Serial.print("center senzor:");
Serial.println(analogRead(A4));
Serial.print("right senzor:");
Serial.println(analogRead(A5));
delay(1000);


}
