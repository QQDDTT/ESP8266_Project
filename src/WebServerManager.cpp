#include <ESP8266WebServer.h>
#include "WebServerManager.h"
#include "SystemMonitor.h"

// 建立服务器实例
ESP8266WebServer server(80);

namespace WebServerManager {

    void beginServer() {
        server.on("/", []() {
            // 每次访问 / 都实时更新一次系统状态
            SystemMonitor::update();

            // 获取监控信息JSON
            String json = SystemMonitor::getStatusJson();

            // 设置CORS头，允许所有来源的请求
            server.sendHeader("Access-Control-Allow-Origin", "*");
            server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            
            // 发送HTTP响应
            server.send(200, "application/json", json);
        });

        server.onNotFound([]() {
            server.send(404, "text/plain", "404: Not Found");
        });

        server.begin();
    }

    void handleClient() {
        server.handleClient();
    }

} // namespace WebServerManager
