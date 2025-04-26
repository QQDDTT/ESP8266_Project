#include "SystemMonitor.h"
#include <ArduinoJson.h>
#define ARDUINOJSON_ENABLE_DEPRECATED 0

static String wifiStatus;
static String gpioStatus;
static String memoryInfo;

void SystemMonitor::begin() {
    wifiStatus = "Unknown";
    gpioStatus = "Unset";
    memoryInfo = "Unset";
}

void SystemMonitor::update() {
    updateWiFiStatus();
    updateGPIOStatus();
    updateMemoryInfo();
}

void SystemMonitor::updateWiFiStatus() {
    if (WiFi.status() == WL_CONNECTED) {
        wifiStatus = WiFi.localIP().toString();
    } else {
        wifiStatus = "Disconnected";
    }
}

void SystemMonitor::updateGPIOStatus() {
    gpioStatus = "";

    // 监控普通GPIO
    for (int pin = 0; pin <= 16; ++pin) {
        if (pin == 6 || pin == 7 || pin == 8) {
            // GPIO6,7,8是SPI内部用的，不读
            continue;
        }
        gpioStatus += "D" + String(pin) + ": " + String(digitalRead(pin)) + " ";
    }

    // 监控特殊引脚
    int a0Value = analogRead(A0);    // A0引脚，0~1023
    int sd2Value = digitalRead(9);   // SD2
    int sd3Value = digitalRead(10);  // SD3

    gpioStatus += "A0: " + String(a0Value) + " ";
    gpioStatus += "SD2(GPIO9): " + String(sd2Value) + " ";
    gpioStatus += "SD3(GPIO10): " + String(sd3Value) + " ";

    gpioStatus += "RSV: reserved ";
    gpioStatus += "CMD: reserved ";
}

void SystemMonitor::updateMemoryInfo() {
    memoryInfo = "FreeHeap: " + String(ESP.getFreeHeap()) + " bytes";
}

String SystemMonitor::getStatusJson() {
    StaticJsonDocument<256> doc;
    doc["wifi"] = wifiStatus;
    doc["gpio"] = gpioStatus;
    doc["memory"] = memoryInfo;
    doc["uptime_sec"] = millis() / 1000;

    String output;
    serializeJson(doc, output);
    return output;
}
