#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128   // OLED display width, in pixels
#define SCREEN_HEIGHT 64   // OLED display height, in pixels
#define OLED_RESET    -1   // Reset pin # (or -1 if sharing Arduino reset pin)

class OLED_DISPLAY {
public:
    OLED_DISPLAY();
    void init();
    void clearDisplay();
    void displayText(const String &text, int x, int y);
    void displayClientIPs(const IPAddress clientIPs[], int numClients);
private:
    Adafruit_SSD1306 display;
};
