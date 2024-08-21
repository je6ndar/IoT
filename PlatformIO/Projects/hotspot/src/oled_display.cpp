#include "oled_display.h"

OLED_DISPLAY::OLED_DISPLAY() 
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

void OLED_DISPLAY::init() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }
    display.clearDisplay();
    display.display();
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
}

void OLED_DISPLAY::clearDisplay() {
    display.clearDisplay();
}

void OLED_DISPLAY::displayText(const String &text, int x, int y) {
    display.setCursor(x, y);
    display.print(text);
    display.display();
}

void OLED_DISPLAY::displayClientIPs(const IPAddress clientIPs[], int numClients) {
    clearDisplay();
    for (int i = 0; i < numClients; i++) {
        display.setCursor(0, i * 10); // Assuming each line height is 10 pixels
        display.print("Client ");
        display.print(i + 1);
        display.print(": ");
        display.println(clientIPs[i].toString());
    }
    display.display();
}
