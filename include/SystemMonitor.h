#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

class SystemMonitor {
public:
    static void begin();             // 初始化监控器
    static void update();            // 定时更新状态（每秒调用一次）
    static String getStatusJson();   // 获取所有状态的 JSON 字符串
private:
    static void updateWiFiStatus();  // 更新WiFi信息
    static void updateGPIOStatus();  // 更新GPIO引脚信息
    static void updateMemoryInfo();  // 更新内存信息
};

#endif
