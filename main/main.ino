#include <WiFi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <WiFiClientSecure.h>
#include "config.h"
#include <FS.h>
#include <SPIFFS.h>
#include "cert.h"  

#define LED_BUILTIN 2 // Define onboard LED pin for ESP32

String FirmwareVer = "2.2"; // Firmware version

#define URL_fw_Version "https://github.com/adevmini/ESP32_OTA_Firmware/blob/main/bin_version.txt"
#define URL_fw_Bin "https://github.com/adevmini/ESP32_OTA_Firmware/blob/main/ESP32_OTA_Firmware.ino.bin"

// Function prototypes
void connect_wifi();
void firmwareUpdate();
int FirmwareVersionCheck();
void saveWiFiCredentials(const String &ssid, const String &password);
bool loadWiFiCredentials(String &ssid, String &password);

unsigned long previousMillis = 0; // Timer variables for periodic checks
unsigned long previousMillis_2 = 0;
const long interval = 60000;
const long mini_interval = 1000;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialize SPIFFS for storing Wi-Fi credentials
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialization failed.");
    return;
  }

  // Load Wi-Fi credentials from SPIFFS
  String ssid, password;
  if (!loadWiFiCredentials(ssid, password)) {
    Serial.println("No Wi-Fi credentials found, using defaults.");
    ssid = WIFI_SSID;
    password = WIFI_PASSWORD;
  }

  // Connect to Wi-Fi
  connect_wifi(ssid, password);
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
      connect_wifi(WIFI_SSID, WIFI_PASSWORD);
    }
  }
}

// Function to connect to Wi-Fi
void connect_wifi(const String &ssid, const String &password) {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid.c_str(), password.c_str());
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
  client.setCACert(rootCACertificate);  // Set the root certificate
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
}

// Function to check firmware version
int FirmwareVersionCheck() {
  String payload;
  int httpCode;
  WiFiClientSecure client;
  client.setCACert(rootCACertificate);  // Set the root certificate
  HTTPClient https;
  
  if (https.begin(client, URL_fw_Version)) {
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

// Function to save Wi-Fi credentials to SPIFFS
void saveWiFiCredentials(const String &ssid, const String &password) {
  File file = SPIFFS.open("/wifi_credentials.txt", "w");
  if (file) {
    file.println(ssid);
    file.println(password);
    file.close();
  }
}

// Function to load Wi-Fi credentials from SPIFFS
bool loadWiFiCredentials(String &ssid, String &password) {
  File file = SPIFFS.open("/wifi_credentials.txt", "r");
  if (file) {
    ssid = file.readStringUntil('\n');
    password = file.readStringUntil('\n');
    file.close();
    return true;
  }
  return false;
}
