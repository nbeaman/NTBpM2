#ifndef NTBoled_h
#define NTBoled_h

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class NTBoled {
  public:
    NTBoled(); 
    bool begin();
    void clear();
    void print(String message, int size, int x, int y);
    
    void drawWifiSymbol(bool connected);
    void drawNoWifiSymbol();
    
    bool isI2CFunctioning();
    bool isI2CDevicePresent(uint8_t address);
    	
    /**
     * @brief Draws a battery-style charge bar in the top-right corner.
     * * @param percentage The charge level to display, from 0 to 100.
     */
    void drawChargeBar(int percentage);

    void show();

  private:
    Adafruit_SSD1306 _display;
		bool _initialized; 							// Track whether OLED is available	
};

#endif