#include <NTBwifi.h>

const char* timezone = "EST5EDT,M3.2.0,M11.1.0";
const char* hostname = "MeshMaster";

void setup() {
  Serial.begin(115200);
  NTBwifi::begin(timezone, hostname);
}

void loop() {
  NTBwifi::handle();
  Serial.println("do'n somthing");
  // ✅ Sensor logic always runs
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

  delay(5000);
}
