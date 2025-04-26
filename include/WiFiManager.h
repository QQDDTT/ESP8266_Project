#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>

namespace WiFiManager {
    String connectWiFi(const char* ssid, const char* password);
}

#endif // WIFI_MANAGER_H
