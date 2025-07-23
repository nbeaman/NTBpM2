#include "NTBwifi.h"

#include <WiFi.h>         // ✅ ESP32 WiFi functions
#include <ArduinoOTA.h>
#include <time.h>

unsigned long NTBwifi::wifiStart = 0;
unsigned long NTBwifi::ntpStart = 0;
bool NTBwifi::wifiDone = false;
bool NTBwifi::timeDone = false;
const char* NTBwifi::timezone = nullptr;

void NTBwifi::begin(const char* ssid, const char* password, const char* tz, const char* hostname) {
  timezone = tz;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  wifiStart = millis();

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
      case OTA_AUTH_ERROR: Serial.println("Auth Failed"); break;
      case OTA_BEGIN_ERROR: Serial.println("Begin Failed"); break;
      case OTA_CONNECT_ERROR: Serial.println("Connect Failed"); break;
      case OTA_RECEIVE_ERROR: Serial.println("Receive Failed"); break;
      case OTA_END_ERROR: Serial.println("End Failed"); break;
    }
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("📦 OTA Progress: %u%%\r", (progress * 100) / total);
  });

  ArduinoOTA.begin();
}

void NTBwifi::handle() {
  ArduinoOTA.handle();

  // Wi-Fi connect logic (non-blocking, 2 minutes max)
  if (!wifiDone && millis() - wifiStart < 120000) {
    if (WiFi.status() == WL_CONNECTED) {
      wifiDone = true;
      Serial.println("\n✅ Wi-Fi connected!");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());

      configTzTime(timezone, "pool.ntp.org");
      ntpStart = millis();
    }
  }

  // NTP sync logic (non-blocking, 3 minutes max)
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

bool NTBwifi::isConnected() {
  return wifiDone && WiFi.status() == WL_CONNECTED;
}

bool NTBwifi::isTimeSynced() {
  return timeDone;
}

String NTBwifi::localIP() {
  return WiFi.localIP().toString();
}
