#include <ESP8266WebServer.h>
#include "WebServerManager.h"
#include "SystemMonitor.h"
#include "DisplayManager.h"

// 建立服务器实例
ESP8266WebServer server(80);

// 16x16 超小 favicon.ico （黑底白点） 示例
const uint8_t FAVICON_DATA[] PROGMEM = {
    0x00,0x00,0x01,0x00,0x01,0x00,0x10,0x10,0x00,0x00,0x01,0x00,0x04,0x00,
    0x28,0x01,0x00,0x00,0x16,0x00,0x00,0x00,0x28,0x00,0x00,0x00,0x10,0x00,
    0x00,0x00,0x20,0x00,0x00,0x00,0x01,0x00,0x04,0x00,0x00,0x00,0x00,0x00,
    0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,0x00,
    0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
const size_t FAVICON_SIZE = sizeof(FAVICON_DATA);

namespace WebServerManager {

    void beginServer() {
        server.on("/status", []() {
            // 每次访问 / 都实时更新一次系统状态
            SystemMonitor::update();

            // 获取监控信息JSON
            String json = SystemMonitor::getStatusJson();

            // 设置CORS头，允许所有来源的请求
            server.sendHeader("Access-Control-Allow-Origin", "*");
            server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

            
            // 发送HTTP响应
            server.send(200, "application/json", json);
        });

        // 处理 favicon.ico
        server.on("/favicon.ico", []() {
            server.sendHeader("Content-Type", "image/x-icon");
            server.send(200, "image/x-icon", reinterpret_cast<const char*>(FAVICON_DATA), FAVICON_SIZE);
        });

        // 处理指令接口
        server.on("/command", []() {
            if (server.hasArg("name")) {
                String command = server.arg("name");
                DisplayManager::showMessage("Command received", command);
                Serial.println("收到指令：" + command);

                // 根据指令名称处理
                if (command == "led_on") {
                    digitalWrite(LED_BUILTIN, LOW); // LED亮（注意ESP8266是反着的）
                    server.send(200, "text/plain", "LED turned ON");
                } else if (command == "led_off") {
                    digitalWrite(LED_BUILTIN, HIGH); // LED灭
                    server.send(200, "text/plain", "LED turned OFF");
                } else if (command == "restart") {
                    server.send(200, "text/plain", "Restarting...");
                    ESP.restart();
                } else {
                    server.send(400, "text/plain", "Unknown command");
                }
            } else {
                server.send(400, "text/plain", "Missing 'name' parameter");
            }
        });

        // 处理文件上传
        server.on("/update", HTTP_POST, []() {
            if (server.hasArg("firmware")) {
                HTTPUpload& upload = server.upload();
                if (upload.status == UPLOAD_FILE_START) {
                    Serial.printf("Upload Start: %s\n", upload.filename.c_str());
                    // Start the firmware update
                    Update.begin(UPDATE_SIZE_UNKNOWN);
                } else if (upload.status == UPLOAD_FILE_WRITE) {
                    // Write the uploaded file to ESP
                    Update.write(upload.buf, upload.currentSize);
                } else if (upload.status == UPLOAD_FILE_END) {
                    if (Update.end()) {
                        if (Update.isFinished()) {
                            Serial.println("Update Success!");
                            server.send(200, "text/plain", "Firmware Update Successful");
                            ESP.restart();
                        } else {
                            Serial.println("Update Failed!");
                            server.send(500, "text/plain", "Update Failed");
                        }
                    } else {
                        Serial.println("Update Failed!");
                        server.send(500, "text/plain", "Update Failed");
                    }
                }
            }
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
