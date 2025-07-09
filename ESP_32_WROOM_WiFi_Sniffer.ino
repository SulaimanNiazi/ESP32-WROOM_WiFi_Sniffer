#include <WiFi.h>
#include <esp_wifi.h>

#define DISPLAY_BUTTON 0 // Boot Button WROOM

// Store known devices
struct Device {
  uint8_t mac[6];
  int8_t rssi;
  unsigned long lastSeen;
};

#define MAX_DEVICES 100
Device devices[MAX_DEVICES];
int deviceCount = 0;

// Check if MAC already stored
int findDevice(const uint8_t* mac) {
  for (int i = 0; i < deviceCount; i++) {
    if (memcmp(devices[i].mac, mac, 6) == 0) return i;
  }
  return -1;
}

String macToString(const uint8_t* mac) {
  char buf[18];
  snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

void snifferCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
  wifi_promiscuous_pkt_t* pkt = (wifi_promiscuous_pkt_t*)buf;
  const uint8_t* srcMac = pkt->payload + 10;  // Source MAC address

  int index = findDevice(srcMac);
  if (index == -1 && deviceCount < MAX_DEVICES) {
    memcpy(devices[deviceCount].mac, srcMac, 6);
    devices[deviceCount].rssi = pkt->rx_ctrl.rssi;
    devices[deviceCount].lastSeen = millis();
    Serial.print("[NEW] ");
    Serial.print(macToString(srcMac));
    Serial.print(" RSSI: ");
    Serial.println(pkt->rx_ctrl.rssi);
    deviceCount++;
  } else if (index != -1) {
    devices[index].lastSeen = millis(); // update last seen
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("🚀 Starting WiFi sniffer...");

  pinMode(DISPLAY_BUTTON, INPUT_PULLUP);  // Enable internal pull-up

  // Disconnect from any Wi-Fi network and disable STA/AP mode. Required before using promiscuous mode.
  WiFi.disconnect(true);
  delay(100);
  WiFi.mode(WIFI_MODE_NULL);
  delay(100);

  // Low-level Wi-Fi initialization sequence. Puts ESP32 in raw listening mode.
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_mode(WIFI_MODE_NULL);
  esp_wifi_start();

  // Optional: lock to a specific channel (e.g., 6). Most used channels are 1, 6 and 11. Supported up to 14.
  esp_wifi_set_channel(6, WIFI_SECOND_CHAN_NONE);

  // Register callback and enable promiscuous packet capture.
  esp_wifi_set_promiscuous_rx_cb(&snifferCallback);
  esp_wifi_set_promiscuous(true);
}

void loop() {
  if (digitalRead(DISPLAY_BUTTON) == LOW) {
    delay(200);
    if(digitalRead(DISPLAY_BUTTON) == LOW){
      Serial.println("📋 Known devices:");
      for (int i = 0; i < deviceCount; i++) {
        Serial.print("  ");
        Serial.print(macToString(devices[i].mac));
        Serial.print(" RSSI: ");
        Serial.print(devices[i].rssi);
        Serial.print(" Last seen: ");
        Serial.print((millis() - devices[i].lastSeen) / 1000);
        Serial.println(" sec ago");
      }
      Serial.println("-------------------------------");
    }
  }
}