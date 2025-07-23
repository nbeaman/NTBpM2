#include <NTBwifi.h>

const char* ssid = "henryb";
const char* password = "Excell55!@";
const char* timezone = "EST5EDT,M3.2.0,M11.1.0";
const char* hostname = "MeshMaster";

void setup() {
  Serial.begin(115200);
  NTBwifi::begin(ssid, password, timezone, hostname);
}

void loop() {
  NTBwifi::handle();

  // ✅ Always run sensor logic regardless of Wi-Fi/NTP
  // read sensor data here

  if (NTBwifi::isConnected() && NTBwifi::isTimeSynced()) {
    time_t now = time(nullptr);
    struct tm* current = localtime(&now);
    if (current != nullptr) {
      char timeStr[30];
      strftime(timeStr, sizeof(timeStr), "%m/%d/%Y %I:%M:%S%p", current);
      Serial.print("🕒 Current Time: ");
      Serial.println(timeStr);
    }
  }

  delay(20000);
}
