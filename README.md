# ESP32 Wi-Fi Device Sniffer

An **ESP32-WROOM-based Wi-Fi Device Sniffer** that passively detects nearby devices by capturing their MAC addresses and signal strength (RSSI) using **promiscuous mode**.

---

## 🔧 Features

* Detects devices broadcasting on 2.4GHz Wi-Fi
* Displays MAC address, RSSI, and last seen time
* Uses ESP32's hardware promiscuous mode
* Optional button to display all detected devices
* Works offline without connecting to any Wi-Fi network

---

## 📦 Requirements

* ESP32-WROOM board
* Arduino IDE with ESP32 board support
* Micro USB cable

---

## 🛠 Wiring

| ESP32 Pin | Purpose               |
| --------- | --------------------- |
| GPIO0     | Display button (BOOT) |
| GND       | Button GND side       |

*Optional: You can connect a push button between GPIO0 and GND to print the list of detected devices.*

---

## 📲 Installation

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Add ESP32 board support via Board Manager:

   * Board URL: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
3. Install the following libraries (should be built-in):

   * `WiFi.h`
   * `esp_wifi.h`
4. Upload the sketch to your ESP32 board

---

## ▶️ How It Works

* ESP32 is set to `WIFI_MODE_NULL` to disable connections
* Promiscuous mode is enabled to capture all Wi-Fi packets
* MAC addresses from management frames are extracted
* Detected devices are stored in a list with their RSSI
* Pressing GPIO0 (BOOT) prints the current device list

---

## 🧪 Example Output

```
🚀 Starting WiFi sniffer...
[NEW] 92:14:36:FA:B2:77 RSSI: -45
[NEW] D8:BC:38:49:A1:CC RSSI: -70
📋 Known devices:
  92:14:36:FA:B2:77 RSSI: -45 Last seen: 5 sec ago
  D8:BC:38:49:A1:CC RSSI: -70 Last seen: 10 sec ago
```

---

## ⚠️ Limitations

* Only captures devices using 2.4GHz Wi-Fi
* MAC randomization by modern phones may affect accuracy
* Does not decode payloads or establish connections
* Locked to a specific channel (default: 6)

---

## 💡 Future Ideas

* Channel hopping to capture on all Wi-Fi channels
* Web server to view devices on a browser
* Logging to SD card or serial monitor

---

## 📄 License

MIT License. Free to use and modify.

---

Contributions welcome!
