// Example sketch to demonstrate the drawing of X BitMap (XBM)
// format image onto the display.

// Information on the X BitMap (XBM) format can be found here:
// https://en.wikipedia.org/wiki/X_BitMap

// This example is part of the TFT_eSPI library:
// https://github.com/Bodmer/TFT_eSPI

// Created by Bodmer 23/04/18

#include <TFT_eSPI.h> // Hardware-specific library
#include "LilyGo_Button.h"

TFT_eSPI tft = TFT_eSPI(); // Invoke library

#define BUTTON_38 38
#define BUTTON_39 39
LilyGo_Button btn_38;
LilyGo_Button btn_39;

void Button_38_Callback(ButtonState event) // TFT_Backlight
{
  uint8_t static brightness_on = 0;
  uint8_t static brightness = 0;
  switch (event)
  {
  case BTN_CLICK_EVENT:
    Serial.println("BTN_CLICK_EVENT");
    brightness_on ^= 1;
    if (brightness_on)
    {
      for (int i = 255; i >= 0; i--)
      {
        brightness = i;
        setBacklight(brightness);
        Serial.printf("brightness:%d \n", brightness);
        delay(5);
      }
    }
    else
    {
      for (int i = 0; i <= 255; i++)
      {
        brightness = i;
        setBacklight(brightness);
        Serial.printf("brightness:%d \n", brightness);
        delay(5);
      }
    }
    break;

    break;
  }
}

void Button_39_Callback(ButtonState event) // TFT_Backlight
{
  switch (event)
  {
  case BTN_CLICK_EVENT:
    Serial.println("BTN_CLICK_EVENT");
    uint8_t static rotation = tft.getRotation();
    rotation = (rotation + 1) % 4;
    tft.setRotation(rotation);
    Serial.printf("rotation:%d \n", rotation);

    tft.fillScreen(TFT_BLACK); // Black screen fill
    tft.setCursor(10, 0);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.println("BTN38 TFT backlight (ON/OFF)");
    tft.println("BTN39 rotation display");
    break;
  }
}

void setBacklight(uint8_t brightness)
{
  ledcWrite(0, brightness);
}

void setup()
{
  Serial.begin(115200);

  tft.begin(); // Initialise the display

  pinMode(TFT_BL, OUTPUT); // 强制设置为数字输出模式
  ledcSetup(0, 5000, 8);   // 5kHz, 8-bit
  ledcAttachPin(TFT_BL, 0);
  setBacklight(0);

  tft.fillScreen(TFT_BLACK); // Black screen fill
  tft.setCursor(10, 0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println("BTN38 TFT backlight (ON/OFF)");
  tft.println("BTN39 rotation display");

  btn_38.init(BUTTON_38, 50, nullptr);
  btn_38.setEventCallback(Button_38_Callback);
  btn_39.init(BUTTON_39, 50, nullptr);
  btn_39.setEventCallback(Button_39_Callback);
  delay(1000);
  Serial.println("start...");
}

void loop()
{
  btn_38.update();
  btn_39.update();
  delay(20);
}
