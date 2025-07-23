#pragma once

#include <Arduino.h>  // ✅ Provides String and basic types

class NTBwifi {
public:
  static void begin(const char* ssid, const char* password, const char* tz, const char* hostname);
  static void handle();
  static bool isConnected();
  static bool isTimeSynced();
  static String localIP();

private:
  static unsigned long wifiStart;
  static unsigned long ntpStart;
  static bool wifiDone;
  static bool timeDone;
  static const char* timezone;
};
