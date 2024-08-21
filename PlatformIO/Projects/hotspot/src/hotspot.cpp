#include "hotspot.h"
#include "oled_display.h"
#include <WiFi.h>
#include "esp_wifi.h"

HOTSPOT::HOTSPOT() {}

void HOTSPOT::start_hotspot(HOTSPOT_SETTINGS config) {
    WiFi.mode(WIFI_AP); // Set ESP32 to AP mode
    WiFi.softAP(config.Apssid, config.Appassword); // Start Access Point

    delay(3000);
    get_host_IP();
    Serial.println("==============================Hotspot running==============================");

    // Initialize OLED display
    oled.init();
}

void HOTSPOT::get_host_IP() {
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("Access Point IP address: ");
    Serial.println(myIP);
}

void HOTSPOT::printConnectedClients() {
    wifi_sta_list_t wifi_sta_list;
    tcpip_adapter_sta_list_t adapter_sta_list;

    // Get the list of connected stations
    esp_wifi_ap_get_sta_list(&wifi_sta_list);
    tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

    int numClients = adapter_sta_list.num;
    IPAddress clientIPs[numClients];

    Serial.print("Number of connected clients: ");
    Serial.println(numClients);

    for (uint8_t i = 0; i < numClients; i++) {
        tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
        clientIPs[i] = IPAddress(station.ip.addr); // Store the IP address

        Serial.print("Client ");
        Serial.print(i + 1);
        Serial.print(" IP Address: ");
        Serial.println(clientIPs[i]);
    }

    // Display IP addresses on OLED
    oled.displayClientIPs(clientIPs, numClients);
}
