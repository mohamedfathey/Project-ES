#include <Servo.h>
Servo Servo ;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Servo.attach(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  int x= analogRead(A0);
  int y=map(x,0,1023,0,150);

  Servo.write(y);
  delay(400);
  Serial.println(y);

  delay(400);

  Servo.write(A0);
  
  Serial.println(y);
  delay(400);
  
  Servo.write(A0);
  Serial.println(y);
  delay(400);
}