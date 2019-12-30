const int potpin = A3;
const int ledPin = 4;
int val = 0;
int freq = 0;
void setup() {
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
  val = analogRead(potpin);
  freq = map(val, 0, 1023, 100, 1000);
  digitalWrite(ledPin, HIGH);
  delay(freq);
  digitalWrite(ledPin, LOW);
  delay(freq);
  Serial.println(freq);
  Serial.println(val);
  // put your main code here, to run repeatedly:

}
