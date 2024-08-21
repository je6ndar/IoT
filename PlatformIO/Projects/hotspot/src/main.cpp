#include <Arduino.h>
#include "hotspot.h"

HOTSPOT hotspot;

void setup() {
    Serial.begin(115200);
    HOTSPOT::HOTSPOT_SETTINGS settings;
    hotspot.start_hotspot(settings);
}

void loop() {
    hotspot.printConnectedClients();
    delay(10000);
}
