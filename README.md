# Echo-32 🔊📡

**Echo-32** is a standalone ESP32-based wireless chat device built using **ESP-NOW** and an **OLED display**.  
It allows two ESP32 boards to discover each other and exchange messages **without Wi-Fi or internet**.

---
![IMG_3763](https://github.com/user-attachments/assets/27785f7f-7414-402f-8737-ab0ebedb294a)
![IMG_3762](https://github.com/user-attachments/assets/3316e18c-8126-493c-b0ed-3c004e373a04)
![IMG_3761](https://github.com/user-attachments/assets/7ca4304b-1a90-41cb-b93b-f24832c2848f)

## 🚀 Features

- ESP-NOW based communication (no router required)
- Broadcast mode & peer-to-peer chat
- Device discovery system
- On-device typing using 3 buttons
- OLED UI (menu, chat, peer list)
- Audio notification using buzzer
- Long-range mode enabled

---

## 🧠 How It Works

### Modes
- **Broadcast** – Messages are sent to all nearby Echo-32 devices
- **Discover** – Scans for nearby devices and locks onto a selected peer

### Controls
| Button | Function |
|------|---------|
| NEXT | Scroll menu / characters |
| SELECT | Select menu / type character |
| SEND | Double tap = send message<br>Hold = exit chat |

---

## 🛠 Hardware Used

- ESP32 Dev Board
- 128×64 OLED Display (SSD1306, I2C)
- 3 Push Buttons
- Active-Low Buzzer
- Jumper wires & breadboard

---

## 📦 Libraries Required

- WiFi.h  
- esp_now.h  
- esp_wifi.h  
- Adafruit_GFX  
- Adafruit_SSD1306  

---

## 📌 Version History

- **v1.0** – Initial ESP-NOW chat prototype
- **v1.1** – UI improvements, discovery system, buzzer alerts, Peer to Peer, improved range

---

## 📷 Project Status

🧪 Actively developed  
📈 More features planned (message history, better UI, encryption)

---

## 👤 Author

**Tanay Tole**  
Electronics | ESP32 | Embedded Systems
