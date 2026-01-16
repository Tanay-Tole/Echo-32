# Echo-32
ESP32 ESP-NOW Chat Device with OLED &amp; Virtual Keyboard
Features

Wireless chat between ESP32 boards using ESP-NOW
OLED display for chat messages and virtual keyboard
Simple button interface:
Next button → cycles characters
Select button → adds character
Send button → sends message
Stores last 4 messages in chat buffer
Hardware

2 × ESP32 boards (Echo-32 works with any ESP32)
1 × OLED display (128x64)
3 × push buttons
Jumper wires
OLED Pin	ESP32 Pin
VCC	3.3V
GND	GND
SCL	GPIO 22
SDA	GPIO 21
Button Function	ESP32 Pin
----------------	---------
Next character	GPIO 25
Select character	GPIO 26
Send message	GPIO 27
Each board must have peer MAC set in firmware: uint8_t peerMAC[6] = {0xAA,0xBB,0xCC,0xDD,0xEE,0xFF}; To find a board’s MAC, use this temporary sketch: 


```
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println("ESP32 MAC Address:");
  Serial.println(WiFi.macAddress());
}

void loop() {}

```
