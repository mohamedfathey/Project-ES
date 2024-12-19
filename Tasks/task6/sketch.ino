const int ledpin = 12;
const int ldrpin = A0;
int threshold =500;

void setup() {


  Serial.begin(9600);
  pinMode(ledpin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int ldrstatus =analogRead(ldrpin);

  if (ldrstatus<= threshold){
    digitalWrite(ledpin, HIGH);
    Serial.print("led turned on");
    Serial.println(ldrstatus);
  }else{
    digitalWrite(ledpin, LOW);
    Serial.print("led turned off");
    Serial.println(ldrstatus);
  }

}