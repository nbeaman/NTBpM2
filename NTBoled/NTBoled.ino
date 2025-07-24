#include <NTBoled.h>

NTBoled oled;

void setup() {
  Serial.begin(115200);

  if (!oled.begin()) {
    Serial.println(F("Failed to start OLED"));
    while (true);
  }

  // 1. Clear the screen buffer
  oled.clear();

  oled.show();
}

void loop() {
  // The screen will continue to show the text from setup()
  // until it is cleared and updated again.
  // Animate the bar filling up
  for (int i = 0; i <= 100; i++) {
    oled.clear(); // Clear previous content
        oled.drawWifiSymbol(true);
    oled.drawChargeBar(i); // Draw the bar with the current percentage
 
    // Print the percentage value on screen
    oled.print(String(i) + "%", 2, 35, 20);
    
    oled.show(); // Update the display
    delay(25);
  }

  delay(1000);

  // Animate the bar draining
  for (int i = 100; i >= 0; i--) {
    oled.clear();
    oled.drawChargeBar(i);
    oled.drawWifiSymbol(true); 
    oled.print(String(i) + "%", 2, 35, 20);
    oled.show();
    delay(25);
  }

  delay(1000);  
}