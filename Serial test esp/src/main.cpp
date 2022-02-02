#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial nodemcu(D6, D5);
void setup() {
  Serial.begin(115200);
  nodemcu.begin(115200);
  while(!Serial) continue;
  // put your setup code here, to run once:
}

void loop() {
  nodemcu.write("test");
  delay(1000);
  // put your main code here, to run repeatedly:
}