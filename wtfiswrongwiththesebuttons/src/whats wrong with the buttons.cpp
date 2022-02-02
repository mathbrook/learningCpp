#include <Arduino.h>
unsigned long t_s1=0;
unsigned long t0_s1=0;
void setup() {
  pinMode(A0,INPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,INPUT);
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  if(digitalRead(A0)){
    digitalWrite(A1,LOW);   
  }
  if(!digitalRead(A0)){
    digitalWrite(A1,HIGH);
  }
  t_s1=millis();
  if(t_s1-t0_s1>100){Serial.println(analogRead(A2)* (5.0 / 1023.0)); t0_s1=t_s1;}

  // put your main code here, to run repeatedly:
}