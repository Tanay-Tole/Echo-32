#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*
  =====================================================
  Echo-32 v1.1
  ESP32 ESP-NOW Chat Device
  -----------------------------------------------------
  Author: Tanay Tole
  =====================================================
*/

/* ---------- PIN CONFIGURATION ---------- */
#define BTN_NEXT    25   // Scroll / Next option
#define BTN_SELECT  26   // Select / Confirm
#define BTN_SEND    27   // Send / Back (tap & hold)
#define BUZZER_PIN  33   // Active-low buzzer

#define BUZZER_ON   LOW
#define BUZZER_OFF  HIGH

/* ---------- OLED CONFIG ---------- */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* ---------- ESP-NOW PACKET STRUCTURE ---------- */
typedef struct {
  uint8_t type;     // 0 = HELLO (discovery), 1 = CHAT message
  char msg[32];     // Message payload
} packet_t;

packet_t outgoing, incoming;

/* ---------- PEER MANAGEMENT ---------- */
uint8_t broadcastMAC[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t peerMAC[6];
bool peerLocked = false;

/* ---------- DISCOVERY MODE ---------- */
#define MAX_PEERS 4
uint8_t discoveredPeers[MAX_PEERS][6];
int numDiscovered = 0;
int selectedPeerIndex = 0;
bool scanning = false;

/* ---------- UI STATE ---------- */
int menuIndex = 0;   // 0 = Broadcast, 1 = Discover
bool inChat = false;

/* ---------- CHAT BUFFER ---------- */
String chat[4] = {"", "", "", ""};

void addChat(String line) {
  for (int i = 0; i < 3; i++) chat[i] = chat[i + 1];
  chat[3] = line;
}

/* ---------- TYPING SYSTEM ---------- */
const char keys[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?<";
int keyIndex = 0;

char input[32];
int inputLen = 0;

/* ---------- SEND BUTTON LOGIC ---------- */
bool sendHeld = false;
unsigned long pressStart = 0;
unsigned long lastTap = 0;
bool waitSecondTap = false;

/* ---------- BUZZER CONTROL ---------- */
unsigned long buzzerEndTime = 0;
bool buzzerActive = false;

void beep(int duration = 120) {
  buzzerActive = true;
  buzzerEndTime = millis() + duration;
  digitalWrite(BUZZER_PIN, BUZZER_ON);
}

void updateBuzzer() {
  if (buzzerActive && millis() >= buzzerEndTime) {
    digitalWrite(BUZZER_PIN, BUZZER_OFF);
    buzzerActive = false;
  }
}

/* ---------- ESP-NOW RECEIVE CALLBACK ---------- */
void onReceive(const esp_now_recv_info *info,
               const uint8_t *data,
               int len) {

  memcpy(&incoming, data, sizeof(incoming));

  // Handle discovery packets
  if (incoming.type == 0) {
    bool alreadyFound = false;

    for (int i = 0; i < numDiscovered; i++) {
      bool match = true;
      for (int j = 0; j < 6; j++)
        if (discoveredPeers[i][j] != info->src_addr[j]) match = false;
      if (match) alreadyFound = true;
    }

    if (!alreadyFound && numDiscovered < MAX_PEERS) {
      memcpy(discoveredPeers[numDiscovered++], info->src_addr, 6);
      addChat("SYS:PEER FOUND");
    }
  }

  // Handle incoming chat messages
  if (incoming.type == 1) {
    addChat("RX:" + String(incoming.msg));
    beep(300); // Beep only on receive
  }
}

/* ---------- LONG RANGE MODE ---------- */
void enableLongRange() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  esp_wifi_set_max_tx_power(82);
  esp_wifi_set_protocol(
    WIFI_IF_STA,
    WIFI_PROTOCOL_11B |
    WIFI_PROTOCOL_11G |
    WIFI_PROTOCOL_11N |
    WIFI_PROTOCOL_LR
  );

  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
}
