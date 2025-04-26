#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>

namespace DisplayManager {
    bool begin();                      // 初始化屏幕
    void showMessage(const String& line1, const String& line2); // 显示两行文字
}

#endif // DISPLAY_MANAGER_H
