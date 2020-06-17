#include "ESP8266WiFi.h"

void setup() {
  Serial.begin(115200);
}
 
void loop() {
  int networksListSize = WiFi.scanNetworks();

  for(int i = 0; i < networksListSize; i++){
    Serial.println(WiFi.SSID(i) + " " + WiFi.RSSI(i));
  }
  
  Serial.println("");
  delay(5000);
}
