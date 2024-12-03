#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#include "config.h"    
#include "cert.h"       

#define LED_BUILTIN 2 // Define onboard LED pin for ESP32

String FirmwareVer = "2.2"; // Firmware version

// GitHub token for private repo authentication
#define GITHUB_TOKEN "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"

// API Endpoints for private GitHub repo
#define URL_fw_Version "https://api.github.com/repos/adevmini/ESP32_OTA_Firmware/contents/bin_version.txt"
#define URL_fw_Bin "https://api.github.com/repos/adevmini/ESP32_OTA_Firmware/contents/ESP32_OTA_Firmware.ino.bin"

// Function prototypes
void connect_wifi();
void firmwareUpdate();
int FirmwareVersionCheck();

unsigned long previousMillis = 0; // Timer variables for periodic checks
unsigned long previousMillis_2 = 0;
const long interval = 60000;
const long mini_interval = 1000;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Connect to Wi-Fi
  connect_wifi();
}

void loop() {
  // Repeated check for firmware update
  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis) >= interval) {
    previousMillis = currentMillis;
    if (FirmwareVersionCheck()) {
      firmwareUpdate();
    }
  }

  // Log Wi-Fi status and firmware version
  if ((currentMillis - previousMillis_2) >= mini_interval) {
    previousMillis_2 = currentMillis;
    Serial.print("idle loop...");
    Serial.print(" Active fw version: ");
    Serial.println(FirmwareVer);
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi connected");
    } else {
      connect_wifi();
    }
  }
}

// Function to connect to Wi-Fi
void connect_wifi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

// Function to handle OTA firmware update
void firmwareUpdate() {
  WiFiClientSecure client;
  client.setCACert(rootCACertificate); // Set the root certificate from cert.h

  HTTPClient https;
  if (https.begin(client, URL_fw_Bin)) {
    https.addHeader("Authorization", "Bearer " + String(GITHUB_TOKEN));
    https.addHeader("Accept", "application/vnd.github.v3.raw");

    int httpCode = https.GET();
    if (httpCode == HTTP_CODE_OK) {
      t_httpUpdate_return ret = httpUpdate.update(client, URL_fw_Bin);
      switch (ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
          break;
        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("HTTP_UPDATE_NO_UPDATES");
          break;
        case HTTP_UPDATE_OK:
          Serial.println("HTTP_UPDATE_OK");
          break;
      }
    } else {
      Serial.printf("Failed to download firmware: %d\n", httpCode);
    }
  }

  https.end();
}

// Function to check firmware version
int FirmwareVersionCheck() {
  String payload;
  int httpCode;
  WiFiClientSecure client;
  client.setCACert(rootCACertificate); // Set the root certificate from cert.h

  HTTPClient https;
  if (https.begin(client, URL_fw_Version)) {
    https.addHeader("Authorization", "Bearer " + String(GITHUB_TOKEN));
    https.addHeader("Accept", "application/vnd.github.v3.raw");

    httpCode = https.GET();
    if (httpCode == HTTP_CODE_OK) {
      payload = https.getString();
      https.end();
    }
  }

  if (httpCode == HTTP_CODE_OK) {
    payload.trim();
    if (payload.equals(FirmwareVer)) {
      Serial.println("Device is already on the latest firmware.");
      return 0;
    } else {
      Serial.println("New firmware detected.");
      return 1;
    }
  }
  return 0;
}
