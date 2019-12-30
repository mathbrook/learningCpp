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
  freq = map(val, 0, 1023, 10, 1000);
  lights();
  data();
  // put your main code here, to run repeatedly:

}
void data() {
  Serial.print(freq);
  Serial.println(val);
  //delay(100);
}
void lights() {

  static unsigned long lastTime = 0;
  const long interval = freq;
  static bool state = 0;

  unsigned long now = millis();

  if ( now - lastTime > interval && state == 0) {
    state = 1;
    lastTime = now;
    digitalWrite(ledPin, HIGH);
  }

  if ( now - lastTime > interval && state == 1) {
    state = 0;
    lastTime = now;
    digitalWrite(ledPin, LOW);
  }
}
