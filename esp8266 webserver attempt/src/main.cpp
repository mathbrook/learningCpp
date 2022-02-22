#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <string.h>
File myFile;

#define CS_PIN D8;
#define LED 2
AsyncWebServer server(80);

const char* ssid = "******";          // Your WiFi SSID
const char* password = "*******";  // Your WiFi Password
SoftwareSerial unoSerial(D6, D5);
void recvMsg(unsigned char *data, size_t len){
  WebSerial.println("Received Data...");
  //myFile = SD.open("test.txt", FILE_WRITE);
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  if (d.length() > 64){
    //unoSerial.println(d.substring(0,63));
    //WebSerial.println(d.substring(0,63));
    //Serial.println(d.substring(0,63));
    for (int i=0; i < (d.length()); i +=64){
      int j = i * 64;
      //unoSerial.println(j);
      WebSerial.println(d.substring(i,64));
      unoSerial.println(d.substring(i,64));
      Serial.println(d.substring(i,i+64));
      delay(5000);
      unoSerial.println(d.substring(i+64,128));
  }
  }
  else{
      WebSerial.println(d);
      unoSerial.println(d);
      Serial.println(d);
    }
  
  //myFile.println(d);
  if (d == "ON"){
    digitalWrite(LED, LOW);
  }
  if (d=="OFF"){
    digitalWrite(LED, HIGH);
  }
  //myFile.close();
}

void setup() {
  unoSerial.begin(9600);
  Serial.begin(115200);
  //WebSerial.println("Initializing SD card...");
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
  //Serial.print("Initializing SD card...");
//if (!SD.begin(10)) {
  //Serial.println("initialization failed!");
  //WebSerial.println("SD init failed!");
  //while (1);
//}
  //WebSerial.println("SD init success");
  //Serial.println("initialization done.");
  /*if(SD.exists("test.txt")){
    WebSerial.println("test files exists. delete?");

  }*/
  //myFile = SD.open("test.txt", FILE_WRITE);
}

void loop() {
  //Serial.print("Hello!");
  //unoSerial.write("Hello!");
}
