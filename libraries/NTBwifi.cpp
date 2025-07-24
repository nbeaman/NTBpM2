#include "NTBwifi.h"

#include <WiFi.h>         // ESP32 WiFi library
#include <ArduinoOTA.h>   // OTA update support
#include <time.h>         // Time functions for NTP sync

// Wi-Fi credentials (configured here for modular design)
const char* NTBwifi::ssid     = "henryb";
const char* NTBwifi::password = "Excell55!@";
const char* NTBwifi::timezone = nullptr;

// Internal state tracking
unsigned long NTBwifi::wifiStart = 0;     // Timestamp when Wi-Fi attempt starts
unsigned long NTBwifi::ntpStart = 0;      // Timestamp when NTP attempt starts
bool NTBwifi::wifiDone = false;           // Has Wi-Fi succeeded
bool NTBwifi::timeDone = false;           // Has time successfully synced

// Initializes Wi-Fi and OTA logic
void NTBwifi::begin(const char* tz, const char* hostname) {
  timezone = tz;               // Save timezone reference for NTP sync
  wifiStart = millis();        // Start Wi-Fi timer

  connectWiFi();               // Begin non-blocking Wi-Fi connection attempt

  // OTA setup: sets hostname and hooks into update lifecycle events
  ArduinoOTA.setHostname(hostname);

  ArduinoOTA.onStart([]() {
    Serial.println("🔄 OTA update started...");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("✅ OTA update finished!");
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.print("❌ OTA error: ");
    switch (error) {
      case OTA_AUTH_ERROR:    Serial.println("Auth Failed"); break;
      case OTA_BEGIN_ERROR:   Serial.println("Begin Failed"); break;
      case OTA_CONNECT_ERROR: Serial.println("Connect Failed"); break;
      case OTA_RECEIVE_ERROR: Serial.println("Receive Failed"); break;
      case OTA_END_ERROR:     Serial.println("End Failed"); break;
    }
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("📦 OTA Progress: %u%%\r", (progress * 100) / total);
  });

  ArduinoOTA.begin();  // Starts the OTA service
}

// Starts Wi-Fi connection attempt (non-blocking)
void NTBwifi::connectWiFi() {
  WiFi.mode(WIFI_STA);     // Station mode — connect to existing network
  WiFi.begin(ssid, password);
  Serial.print("📡 Connecting to Wi-Fi");
}

// Starts NTP time sync via given timezone config
void NTBwifi::syncTime() {
  configTzTime(timezone, "pool.ntp.org");  // Uses pool.ntp.org for sync
  ntpStart = millis();                     // Start NTP timer
}

// Main handler called from loop()
// Drives Wi-Fi connection, time sync, and OTA servicing
void NTBwifi::handle() {
  ArduinoOTA.handle();  // Keep OTA service alive

  // Wi-Fi: allow up to 2 minutes for connection
  if (!wifiDone && millis() - wifiStart < 120000) {
    if (WiFi.status() == WL_CONNECTED) {
      wifiDone = true;
      Serial.println("\n✅ Wi-Fi connected!");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      syncTime();  // Begin time sync after Wi-Fi connects
    } else {
      Serial.print(".");  // Connection feedback
    }
  }

  // NTP: allow up to 3 minutes for time sync
  if (wifiDone && !timeDone && millis() - ntpStart < 180000) {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      char timeStr[30];
      strftime(timeStr, sizeof(timeStr), "%m/%d/%Y %I:%M:%S%p", &timeinfo);
      Serial.print("✅ Time synced: ");
      Serial.println(timeStr);
      timeDone = true;
    }
  }
}

// Returns true if Wi-Fi has connected successfully
bool NTBwifi::isConnected() {
  return wifiDone && WiFi.status() == WL_CONNECTED;
}

// Returns true if time has been successfully synced via NTP
bool NTBwifi::isTimeSynced() {
  return timeDone;
}

// Returns current IP as a String
String NTBwifi::localIP() {
  return WiFi.localIP().toString();
}
