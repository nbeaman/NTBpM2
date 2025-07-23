#pragma once

#include <Arduino.h>

class NTBwifi {
public:
  static void begin(const char* tz, const char* hostname);
  static void handle();
  static bool isConnected();
  static bool isTimeSynced();
  static String localIP();

private:
  static void connectWiFi();
  static void syncTime();

  static unsigned long wifiStart;
  static unsigned long ntpStart;
  static bool wifiDone;
  static bool timeDone;

  static const char* ssid;
  static const char* password;
  static const char* timezone;
};
