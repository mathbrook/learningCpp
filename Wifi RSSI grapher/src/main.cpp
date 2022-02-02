/* WiFi Signal Strength
 * Shows RSSI of a predefined network with a graph
 *
 * RSSI is a percentage in the range -120db to 0db.
 * The closer to 0 the better.
 *
 * Connections:
 * WeMos D1 Mini   YWROBOT    Description
 * (ESP8266)       I2C LCD
 *
 * GND             GND
 * 5V              Vcc
 * D2              SDA
 * D1              SDL
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> // marcoschwartz/LiquidCrystal_I2C
//#include <LcdBarGraphX.h>

// Graph the RSSI of this WiFi
const char* myssid = "Home399";
const char* mypass = "399angier";

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//LcdBarGraphX lbg(&lcd, 20, 0, 2);

void setup(void) {
  Wire.begin(D2,D3);
  Serial.begin(115200);
  Serial.println("\nESP8266 WiFi Signal Strength Graph");
  Serial.println("WeMos D1 Mini + YWROBOT I2C LCD\n");

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Turn LCD backlight on
  // Configure LCD
  lcd.init();
//  lcd.begin(20, 4);
  lcd.backlight();

  WiFi.begin(myssid, mypass);
  Serial.print("Connecting");
  lcd.print("Connecting");

  // Wait for successful connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.print(".");
  }

  Serial.print("\nConnected to: ");
  Serial.println(myssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  lcd.clear();
  lcd.print("Connected");
  delay(1000);
  lcd.clear();
}


long rssi;
void loop(void) {
  rssi = WiFi.RSSI();  // eg. -63

  // Convert to scale -48 to 0 eg. map(rssi, -100, 0, 0, -48);
  // I used -100 instead of -120 because <= -95 is unusable
  // Negative number so we can draw n pixels from the bottom in black

  // Draw the RSSI eg. -63db
  lcd.setCursor(3,0);
  lcd.printf("%ddB  ",rssi);
  if(rssi < -99)
  {
    rssi = -99;
  }
//  lbg.drawValue(100+rssi, 100);
  Serial.printf("%ddb\n",rssi);

  delay(100);  // Adjust this to change graph speed
}