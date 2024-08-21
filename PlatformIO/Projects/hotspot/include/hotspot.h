#pragma once
#include <WiFi.h>
#include "oled_display.h"

class HOTSPOT {
public:
    HOTSPOT();

    struct HOTSPOT_SETTINGS {
        char *Apssid = "**********";   // Access Point SSID
        char *Appassword = "**********"; // Access Point Password
    } config;

    void start_hotspot(HOTSPOT_SETTINGS config);
    void get_host_IP();
    void printConnectedClients();

private:
    OLED_DISPLAY oled;
};
