#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
//Debounce SM vars
int state_s1=0;
int prevstate_s1=0;
unsigned char pin_s1=A0;
int val_s1=0;
int prevval_s1=0;
unsigned long t_s1=0;
unsigned long t0_s1=0;
unsigned long bounce_delay_s1=5;
unsigned long hold_delay_s1=1000;
//second debounce sm vars
int state_s2=0;
int prevstate_s2=0;
unsigned char pin_s2=A1;
int val_s2=0;
int prevval_s2=0;
unsigned long t_s2=0;
unsigned long t0_s2=0;
unsigned long bounce_delay_s2=5;
unsigned long hold_delay_s2=1000;
//EEPROM Addresses

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
}
void SM_s2(){
    val_s2=digitalRead(pin_s2);
    prevstate_s2=state_s2;
    switch(state_s2){
    case 0:
      state_s2=1;
      break;
    case 1:
      if(val_s2==0){state_s2=2;}
    break;
    case 2:
      t0_s2=millis();
      state_s2=3;
    break;
    case 3:
      t_s2=millis();
      if(t_s2 - t0_s2>bounce_delay_s2){state_s2=4;}
      if(val_s2==HIGH){state_s2=0;}
    break;
    case 4:  
      t_s2=millis();
      if (val_s2 == HIGH) {state_s2 = 5;}
      if (t_s2 - t0_s2 > hold_delay_s2) {state_s2 = 6;}
    break;
    case 5:
      Serial.println("Pressed");
      state_s2 = 0;
    break;
    case 6: //HOLD!
      //proceed to LOW WAIT
      Serial.println("Held");
      state_s2 = 7;
    break;
    case 7: //LOW WAIT
      //wait for switch to go back HIGH, then reset
      if (val_s2 == HIGH) {state_s2 = 8;}
    break;
    case 8:
      state_s2 = 0;
      break;
  }
}
void SM_s1(){
    val_s1=digitalRead(pin_s1);
    prevstate_s1=state_s1;
    switch(state_s1){
    case 0:
      digitalWrite(A1,LOW);
      state_s1=1;
      break;
    case 1:
      if(val_s1==0){state_s1=2;}
    break;
    case 2:
      digitalWrite(A1,HIGH);  
      t0_s1=millis();
      state_s1=3;
    break;
    case 3:
      digitalWrite(A1,HIGH);  
      t_s1=millis();
      if(t_s1 - t0_s1>bounce_delay_s1){state_s1=4;}
      if(val_s1==HIGH){state_s1=0;}
    break;
    case 4:
      digitalWrite(A1,HIGH);  
      t_s1=millis();
      if (val_s1 == HIGH) {state_s1 = 5;}
      if (t_s1 - t0_s1 > hold_delay_s1) {state_s1 = 6;}
    break;
    case 5:
      Serial.println("Pressed");
      state_s1 = 0;
    break;
    case 6: //HOLD!
      Serial.println("Held");
      state_s1 = 7;
    break;
    case 7: //LOW WAIT
    t_s1=millis();
      digitalWrite(A1,HIGH);   //wait for switch to go back HIGH, then reset
      if (val_s1 == HIGH) {state_s1 = 8;}
    break;
    case 8:
      state_s1 = 0;
      break;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  SM_s1();
  SM_s2();
}