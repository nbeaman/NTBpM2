#include <NTBwifi.h>
#include <NTBoled.h>
NTBoled oled;

const char* timezone = "EST5EDT,M3.2.0,M11.1.0";
const char* hostname = "MeshMaster";

void setup() {
  Serial.begin(115200);

  if (!oled.begin()) {
    Serial.println(F("Failed to start OLED"));
    //while (true);
  }
  // 1. Clear the screen buffer
  oled.clear();
  oled.show();

  NTBwifi::begin(timezone, hostname);
}

void loop() {
  // what handle does----------------------------------------------------------------
  // keeps your device’s network logic alive and responsive.
  //    Handles OTA updates with ArduinoOTA.handle().
  //    Checks if the device connects to Wi-Fi (for up to 2 minutes).
  //    If Wi-Fi connects, it tries to sync the time via NTP (for up to 3 minutes).
  //    Runs all of this non-blocking so your sensor code isn't interrupted.
  NTBwifi::handle();
  //---------------------------------------------------------------------------------

  Serial.println("pretending to sense data.");
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

  oled.clear();
  if (NTBwifi::isConnected()){
    oled.drawWifiSymbol(true);     // Show full Wi-Fi strength
    oled.print(String(NTBwifi::localIP()),1,13,8);
  } else {
    oled.drawNoWifiSymbol();       // Show crossed-out Wi-Fi icon
  }
  oled.show();


  delay(4000);        //setting this to 10000 makes it so that OTA will not work
}
