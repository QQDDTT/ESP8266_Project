#include <Arduino.h>
#include "WiFiManager.h"
#include "WebServerManager.h"
#include "DisplayManager.h"

const char* ssid = "Nick";
const char* password = "hu900414";

void setup() {
    Serial.begin(115200);

    if (DisplayManager::begin()) {
        DisplayManager::showMessage("Booting...", "");
    } else {
        Serial.println("Display failed, halt.");
        while (true); // 停止执行
    }
    DisplayManager::showMessage("Booting...", "Connecting to WiFi...");
    const String ip = WiFiManager::connectWiFi(ssid, password);
    DisplayManager::showMessage("Booting...", "Starting server...");
    WebServerManager::beginServer();
    DisplayManager::showMessage("Server ready", ip);
}

void loop() {
    WebServerManager::handleClient();
}
