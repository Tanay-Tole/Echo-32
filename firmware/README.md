# Echo-32 Firmware

This folder contains the firmware for **Echo-32**, an ESP32-based wireless chat device using **ESP-NOW** and an **OLED display**.

The firmware handles:
- Device discovery
- Peer-to-peer and broadcast messaging
- OLED UI
- Button-based typing
- Buzzer alerts

Current firmware version: **v1.1**

---

## 🔧 Hardware Wiring

### OLED Display (SSD1306 – I2C)

| OLED Pin | ESP32 Pin |
|---------|-----------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

- OLED resolution: 128×64  
- I2C address used: `0x3C`

---

### Buttons

All buttons use `INPUT_PULLUP`.  
Connect one side of the button to the GPIO pin and the other side to **GND**.

| Button | GPIO | Purpose |
|------|------|--------|
| NEXT | GPIO 25 | Scroll menu / change character |
| SELECT | GPIO 26 | Select option / type character |
| SEND | GPIO 27 | Send message / exit chat |

---

### Buzzer (Active-Low)

| Buzzer Pin | ESP32 |
|-----------|-------|
| + | GPIO 33 |
| – | GND |

- The buzzer is **active-low**
- It beeps **only when a message is received**

---

## ▶️ How to Use

### Power On
1. Power the ESP32 using USB or 5V
2. The OLED shows the **ECHO-32** splash screen
3. The main menu appears

---

### Main Menu
Use:
- **NEXT** → change option
- **SELECT** → confirm

Menu options:
- **BROADCAST**
- **DISCOVER**

---

### Broadcast Mode
- Messages are sent to all nearby Echo-32 devices
- No pairing required

Steps:
1. Select **BROADCAST**
2. Type a message
3. Double-tap **SEND** to transmit

---

### Discover Mode
This is the recommended mode.

Steps:
1. Select **DISCOVER**
2. Device scans for nearby Echo-32 units
3. Detected peers appear as MAC addresses
4. Use **NEXT** to scroll
5. Press **SELECT** to lock onto a peer

Once selected, the device enters chat mode.

✔ No manual MAC address entry  
✔ Automatic peer connection

---

### Chat Mode

#### Typing
- **NEXT** → change character
- **SELECT** → add character
- `<` → backspace

#### Sending
- **Double-tap SEND** → send message
- **Hold SEND (~1 second)** → exit chat and return to menu

---

### Buzzer Behavior
- Beeps only on **incoming messages**
- No beep on sending

---

## ⚠️ Notes

- Both devices must be running the **same firmware**
- ESP-NOW channel is fixed to channel 1
- Incorrect OLED wiring will result in a blank display

---

## 📌 Version History

- **v1.0**
  - Initial ESP-NOW chat firmware
  - Broadcast messaging
  - OLED UI and button input

- **v1.1**
  - Automatic peer discovery
  - Removed manual MAC address configuration
  - Added buzzer alert on receive
  - Improved UI flow

---

## 👤 Author

Tanay Tole  
Echo-32 Project
