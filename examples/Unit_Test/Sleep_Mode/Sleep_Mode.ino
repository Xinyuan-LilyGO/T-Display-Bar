#include <Arduino.h>
#include "LilyGo_Button.h"

#define BUTTON 38
LilyGo_Button btn;

void Button1_Callback(ButtonState event) //TFT_Backlight
{
  switch (event)
  {
  case BTN_LONG_PRESSED_EVENT:
    Serial.println("long pressed...");
    // into deep sleep mode
    Serial.println("deep sleep...");
    delay(1000);
    esp_deep_sleep_start();
    break;
  default:
    break;
  }
}

void setup() {
    Serial.begin(115200);
    pinMode(GPIO_NUM_0, INPUT_PULLUP); // 配置按键引脚
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, LOW); // 设置按键为唤醒源（低电平唤醒）

    btn.init(BUTTON, 50, nullptr);
    btn.setEventCallback(Button1_Callback);

    delay(1000);
    Serial.println("start...");
}

void loop() {
    btn.update();
    delay(100);
}