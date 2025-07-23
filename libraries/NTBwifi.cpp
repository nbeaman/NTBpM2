#include "NTBwifi.h"

#include <WiFi.h>
#include <ArduinoOTA.h>
#include <time.h>

// Wi-Fi Credentials (configured here)
const char* NTBwifi::ssid     = "henryb";
const char* NTBwifi::password = "Excell55!@";
const char* NTBwifi::timezone = nullptr;

unsigned long NTBwifi::wifiStart = 0;
unsigned long NTBwifi::ntpStart = 0;
bool NTBwifi::wifiDone = false;
bool NTBwifi::timeDone = false;

void NTBwifi::begin(const char* tz, const char* hostname) {
  timezone = tz;
  wifiStart = millis();

  connectWiFi();

  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.onStart([]() { Serial.println("🔄 OTA update started..."); });
  ArduinoOTA.onEnd([]()   { Serial.println("✅ OTA update finished!"); });
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

void NTBwifi::connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("📡 Connecting to Wi-Fi");
}

void NTBwifi::syncTime() {
  configTzTime(timezone, "pool.ntp.org");
  ntpStart = millis();
}

void NTBwifi::handle() {
  ArduinoOTA.handle();

  if (!wifiDone && millis() - wifiStart < 120000) {
    if (WiFi.status() == WL_CONNECTED) {
      wifiDone = true;
      Serial.println("\n✅ Wi-Fi connected!");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      syncTime();
    } else {
      Serial.print(".");
    }
  }

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
