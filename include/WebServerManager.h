#ifndef WEB_SERVER_MANAGER_H
#define WEB_SERVER_MANAGER_H

// 图标的二进制内容
extern const uint8_t FAVICON_DATA[];
extern const size_t FAVICON_SIZE;

namespace WebServerManager {
    void beginServer();
    void handleClient();
}

#endif // WEB_SERVER_MANAGER_H
