const int sensorPin = A0;
const int voltagetwo = A2;
const float baselineTemp=22;
void setup() {
  Serial.begin(9600);
  for(int pinNumber = 2; pinNumber<5; pinNumber++){
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }  
    // put your setup code here, to run once:
}

void loop() {
  int sensorVal = analogRead(sensorPin);
  //Serial.print("Sensor value: ");
  //Serial.print(sensorVal);
  int voltagereading = analogRead(voltagetwo);
  float battvolt = (voltagereading/1034.0) * 5.0;
  Serial.print("batt voltage: ");
  Serial.print(battvolt);
  // convert the ADC reading to voltage
  float voltage = (sensorVal/1034.0) * 5.0;
  Serial.print(", temp sensor volts: ");
  Serial.print(voltage);
  Serial.print(", degrees C: ");
  //convert the voltage into temperature in degrees
  float temperature = (voltage - .5) * 100;
  Serial.println(temperature);
  //Serial.println(battvolt);
  // put your main code here, to run repeatedly:
delay(100);
}
