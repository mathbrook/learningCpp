#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//#include <EEPROM.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>
File myFile;
LiquidCrystal_I2C lcd(0x27,16,2);
RTC_DS3231 rtc;
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
unsigned char pin_s2=A2;
int val_s2=0;
int prevval_s2=0;
unsigned long t_s2=0;
unsigned long t0_s2=0;

unsigned long bounce_delay_s2=5;
unsigned long hold_delay_s2=1000;
//misc
unsigned long t_eep=0;
unsigned long t0_eep=0;
int eepState=0;
int preveepState=0;
int debugging = 1;
int addr1=0;
//Time tracking fuckery
int prevstate_t1=0; int state_t1=0,prevstate_t2=0; int state_t2=0;
unsigned long t_t1;
unsigned long t0_t1;
unsigned long t_t2;
unsigned long t0_t2;
unsigned int loggingDelay=1000;
float elapsed = 0;
float duration =0;
//
int targetPosition =0;
int successCount=0;
void setup() {
  Serial.setTimeout(0);
  lcd.init(); lcd.backlight();
  pinMode(pin_s1,INPUT); pinMode(A1,OUTPUT); pinMode(10,OUTPUT);  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600); Serial.println("Debugging is enabled");
  rtc.begin();
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  DateTime now = rtc.now();
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print((now.second(), DEC));
  Serial.println();
  String str = String((now.year())-2000) + '-' + String(now.month(), DEC) + '-' + String(now.day(), DEC);
  String newLog = str + ".txt";
  int n = newLog.length();
  char newLogName[n+1];
  strcpy(newLogName,newLog.c_str());
  Serial.print("New log: ");
  Serial.println(newLogName);
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  myFile = SD.open(newLogName, FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing time to new log... ");
    myFile.println();
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print((now.second(), DEC));
    myFile.println();
    // close the file:
    targetPosition=myFile.position()+1;
    myFile.close();
    Serial.println("done.");
    lcd.print("success");
    delay(100);
    lcd.clear();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error creating new log");
    lcd.print("fail");
    }
  // put your setup code here, to run once:
}
void TimeLog(){
  prevstate_t2=state_t2;
  switch(state_t2){
    case 0: t0_t2=millis();//if(state_t1==0)
    { state_t2=1;}
    break;
    case 1: t_t2=millis();
    if(t_t2-t0_t2>loggingDelay/* && state_t1==0*/){state_t2=2;}
    break;
    case 2:
    myFile=SD.open("22-1-4.txt", FILE_WRITE);
    if(myFile){
    //myFile=SD.open("22-1-4.txt", O_RDWR);
    myFile.seek(targetPosition);
    myFile.println(elapsed+duration);
    myFile.close();
    successCount++;
    //Serial.print(successCount);
    //Serial.println("Write successful.");
    }else{Serial.println("SD Write failed.");}
    state_t2=0;
    break;
  }
}
void SM_eep(){
  preveepState=eepState;
  switch(eepState){
    case 0:
      eepState=1;
    break;
    case 1:
      if(state_s2==2){eepState=2;}
    break;
    case 2:
      t0_eep=millis();
      eepState=3;
    break;
    case 3:
      t_eep=millis();
      if(t_eep-t0_eep>100 & state_t1==0){eepState=4;}
    break;
    case 4:
      myFile = SD.open("22-1-4.txt");
  if (myFile) {
    Serial.println("22-1-4.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    Serial.println();
    // close the file:
    myFile.close();
  }else{Serial.println("Failed to check file.");}
      //addr1=0;
      //Serial.println("Printing...");
      //String lol=Serial.read(); //Debugging stuff
      //addr1=lol.toInt();
      //int len = Serial.available();
      //Serial.println(millis());
      //while(Serial.available()>0){
        //addr1+=Serial.parseInt();
        //Serial.print(".");
        //}
      //Serial.println(addr1);
      //Serial.println(EEPROM.read(addr1));
      //Serial.println(millis());
      eepState=0;
    break;
  }
}
void SM_s2(){
    val_s2=digitalRead(pin_s2);
    prevstate_s2=state_s2;
    switch(state_s2){
    case 0:
      digitalWrite(LED_BUILTIN,LOW);
      state_s2=1;
      break;
    case 1:
      if(val_s2==0){state_s2=2;}
    break;
    case 2:
      digitalWrite(LED_BUILTIN,HIGH);  
      t0_s2=millis();
      state_s2=3;
    break;
    case 3:
      digitalWrite(LED_BUILTIN,HIGH);  
      t_s2=millis();
      if(t_s2 - t0_s2>bounce_delay_s2){state_s2=4;}
      if(val_s2==HIGH){state_s2=0;}
    break;
    case 4:
      digitalWrite(LED_BUILTIN,HIGH);  
      t_s2=millis();
      if (val_s2 == HIGH) {state_s2 = 5;}
      if (t_s2 - t0_s2 > hold_delay_s2) {state_s2 = 6;}
    break;
    case 5: //TRIGGERED!
      //reset the State Machine
      digitalWrite(LED_BUILTIN,HIGH);   
      //Serial.println("Pressed");
      state_s2 = 0;
    break;
    case 6: //HOLD!
      //proceed to LOW WAIT
      //Serial.println(EEPROM.read(addr));
      //Serial.println("Held");
      state_s2 = 7;
    break;
    case 7: //LOW WAIT
    t_t1=millis();
     // lcd.clear();
      //lcd.print(duration);
      digitalWrite(LED_BUILTIN,HIGH);   
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
    case 5: //TRIGGERED!
      //reset the State Machine
      digitalWrite(A1,HIGH);   
      //Serial.println("Pressed");
      state_s1 = 0;
    break;
    case 6: //HOLD!
      //proceed to LOW WAIT
      //Serial.println(EEPROM.read(addr));
      //Serial.println("Held");
      state_s1 = 7;
    break;
    case 7: //LOW WAIT
    t_t1=millis();
     // lcd.clear();
      //lcd.print(duration);
      digitalWrite(A1,HIGH);   
      //wait for switch to go back HIGH, then reset
      if (val_s1 == HIGH) {state_s1 = 8;}
    break;
    case 8:
      state_s1 = 0;
      break;
  }
}
void timeTrack(){
  prevstate_t1=state_t1;
  switch(state_t1){
    case 0: //Idle state, waiting for confirmed button press from debounce function
      if(state_s1==5){state_t1=1;}
      break;
    case 1:
      t0_t1=millis();
      state_t1=2;
      break;
    case 2: //counting time until button is released...
      t_t1=millis();
      lcd.setCursor(0,0);
      duration=(double(t_t1-t0_t1))/1000;
      lcd.print(duration);
      lcd.setCursor(0,1);
      lcd.print(duration+elapsed);
    if(state_s1==6){state_t1=3;} //which we can tell by waiting for the debounce function to reset
    break;
    case 3: //add the time we counted to the total duration, then reset
    elapsed += duration;
    Serial.println(elapsed);
    duration=0;
    state_t1=0;
    break;
  }
}
void loop() {
  TimeLog();
  timeTrack();
  SM_eep();
  SM_s1();
  SM_s2();
  if(debugging){
    if(prevstate_t1 != state_t1){
    //  lcd.clear();
      //lcd.print("Switch state: ");
     //lcd.print(state_s1);
     Serial.print("Tracker state: ");
     Serial.println(state_t1);
   }
    if(prevstate_s1 != state_s1){
    //  lcd.clear();
      //lcd.print("Switch state: ");
     //lcd.print(state_s1);
     //Serial.print("Switch state: ");
     //Serial.println(state_s1);
   }

  /*if(Serial.available()){
    Serial.println("Printing...");
    int addr1=Serial.read();
    Serial.println(addr1);
    Serial.println(EEPROM.read(addr1));
  }*/
  /*if(val_s1 != prevval_s1){
    Serial.print("Button state: ");
    Serial.println(val_s1);
    prevval_s1=val_s1;
  }*/
  
}
if(preveepState != eepState){
    Serial.print("eepState state: ");
    Serial.println(eepState);
  }
}
