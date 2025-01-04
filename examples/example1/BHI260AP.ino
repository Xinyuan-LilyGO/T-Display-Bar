#include <Arduino.h>
#include <Wire.h>
#include "SPI.h"
#include "board.h"

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

#ifdef BHY2_USE_I2C
  // Using I2C interface
  // BHI260AP_SLAVE_ADDRESS_L = 0x28
  // BHI260AP_SLAVE_ADDRESS_H = 0x29
  if (!bhy.init(Wire, I2C_SDA, I2C_SCL, BHI260AP_SLAVE_ADDRESS_L))
  {
    Serial.println(bhy.getError());
    Serial.print("Failed to init BHI260AP - ");
    while (1)
    {
      delay(1000);
    }
  }
#else
  // Using SPI interface
  if (!bhy.init(SPI, BHI260AP_CS, BHI260AP_MOSI, BHI260AP_MISO, BHI260AP_SCK))
  {
    Serial.print("Failed to init BHI260AP - ");
    Serial.println(bhy.getError());
    while (1)
    {
      delay(1000);
    }
  }
#endif
  else
    Serial.println("Init BHI260AP Sensor success!");

  if (!rtc.begin(Wire, PCF8563_SLAVE_ADDRESS, I2C_SDA, I2C_SCL))
  {
    Serial.println("Failed to find PCF8563 - check your wiring!");
    while (1)
    {
      delay(1000);
    }
  }
  else
    Serial.println("Init RTC PCF8563 success!");

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  if (!SD.begin(SD_CS, SPI, 40000000))
  {
    Serial.println("SD card initialization failed!");
  }
  else
    Serial.println("Init SD card success!");

  pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
  Serial.println("T-Display-Bar");
  // tone(BUZZER_PIN, 1000);  // 发出1000Hz的音调
  // delay(1000);                  // 延时1秒
  // noTone(BUZZER_PIN);      // 停止发声
  delay(1000);                  // 延时1秒
}
