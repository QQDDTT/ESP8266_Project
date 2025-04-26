#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DisplayManager.h"

// 屏幕参数定义
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// 建立全局 OLED 对象
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

namespace DisplayManager {

// 初始化屏幕
bool begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 默认 I2C 地址 0x3C
        Serial.println(F("SSD1306 OLED init failed"));
        return false;
    }
    display.clearDisplay();
    display.display();
    return true;
}

// 显示两行文字
void showMessage(const String& line1, const String& line2) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    String l1 = line1;
    String l2 = line2;
    if (l1.length() > 16) {
        display.println(l1.substring(0, 16));
        display.println(l1.substring(16));
    } else {
        display.println(l1);
        display.println();
    }
    if (l2.length() > 16) {
        display.println(l2.substring(0, 16));
        l2 = l2.substring(16);
        if (l2.length() > 16) {
            display.println(l2.substring(0, 16));
            l2 = l2.substring(16);
            if (l2.length() > 16) {
                display.println(l2.substring(0, 16));
                l2 = l2.substring(16);
                if (l2.length() > 16) {
                    display.println(l2.substring(0, 16));
                    display.println("...");
                } else {
                    display.println(l2);
                }
            } else {
                display.println(l2);
            }
        } else {
            display.println(l2);
        }
    } else {
        display.println(l2);
    }
    display.display();
}

} // namespace DisplayManager
