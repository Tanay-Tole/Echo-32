#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <esp_now.h>

/*
  =====================================================
  Echo-32 v1.0
  ESP32 ESP-NOW Chat Device
  =====================================================
*/

// ===== OLED CONFIG =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== BUTTON PINS =====
#define BTN_NEXT    25
#define BTN_SELECT  26
#define BTN_SEND    27

// ===== PEER MAC ADDRESS =====
// CHANGE THIS to the MAC address of the other ESP32
// Example format: {0xAA,0xBB,0xCC,0xDD,0xEE,0xFF}
uint8_t peerMAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// ===== MESSAGE STRUCT (ESP-NOW) =====
typedef struct {
  char msg[64];
} Message;

Message outgoing;
Message incoming;

// ===== VIRTUAL KEYBOARD =====
const char keys[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?";
int keyIndex = 0;

String input = "";
String chat[4];

// ===== CHAT BUFFER =====
void addChat(String msg) {
  for (int i = 0; i < 3; i++) {
    chat[i] = chat[i + 1];
  }
  chat[3] = msg;
}

// ===== DRAW USER INTERFACE =====
void drawUI() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Header
  display.setCursor(0, 0);
  display.println("ECHO-32 ESP32");
  display.println("--------------");

  // Chat messages
  for (int i = 0; i < 4; i++) {
    display.println(chat[i]);
  }

  // Input area
  display.println("--------------");
  display.print("> ");
  display.print(input);

  // Selected character
  display.setCursor(110, 56);
  display.print(keys[keyIndex]);

  display.display();
}

// ===== ESP-NOW RECEIVE CALLBACK =====
void onReceive(const esp_now_recv_info *info,
               const uint8_t *data,
               int len) {
  memcpy(&incoming, data, sizeof(incoming));
  addChat("RX:" + String(incoming.msg));
  drawUI();
}

// ===== SETUP =====
void setup() {
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_SEND, INPUT_PULLUP);

  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // ESP-NOW setup
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    addChat("ESP-NOW ERROR");
    return;
  }

  esp_now_register_recv_cb(onReceive);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peerMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);

  addChat("ESP32 Online");
  drawUI();
}

// ===== LOOP =====
void loop() {

  // NEXT: cycle characters
  if (digitalRead(BTN_NEXT) == LOW) {
    keyIndex = (keyIndex + 1) % strlen(keys);
    delay(200);
    drawUI();
  }

  // SELECT: add character
  if (digitalRead(BTN_SELECT) == LOW) {
    if (input.length() < 20) {
      input += keys[keyIndex];
      delay(200);
      drawUI();
    }
  }

  // SEND message
  if (digitalRead(BTN_SEND) == LOW) {
    input.toCharArray(outgoing.msg, sizeof(outgoing.msg));
    esp_now_send(peerMAC,
                 (uint8_t*)&outgoing,
                 sizeof(outgoing));

    addChat("TX:" + input);
    input = "";
    delay(300);
    drawUI();
  }
}
