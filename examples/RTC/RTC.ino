#include <Arduino.h>
#include <Wire.h>
#include "SensorPCF85063.hpp"
#include <esp_sntp.h>
#include <WiFi.h>

#define I2C_SDA 3
#define I2C_SCL 2

#define WIFI_SSID "xinyuandianzi"
#define WIFI_PASSWORD "AA15994823428"
#define CFG_TIME_ZONE "CST-8" // TZ_Asia_Shanghai
#define NTP_SERVER1 "pool.ntp.org"
#define NTP_SERVER2 "time.nist.gov"

struct tm timeinfo;
SensorPCF85063 rtc;

void time_sync_notification_handler(timeval *t)
{
    Serial.println("Time synchronized");
    rtc.hwClockWrite();
    rtc.getDateTime(&timeinfo);
}

void setup()
{
    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL);
    if (!rtc.begin(Wire, PCF85063_SLAVE_ADDRESS, I2C_SDA, I2C_SCL))
    {
        Serial.println("Failed to find PCF8563 - check your wiring!");
        while (1)
        {
            delay(1000);
        }
    }
    Serial.println("Init RTC PCF8563 success!");

    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
    Serial.println("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }

    sntp_set_time_sync_notification_cb(time_sync_notification_handler); // 注册时间同步通知回调函数
    configTzTime(CFG_TIME_ZONE, NTP_SERVER1, NTP_SERVER2);              // 设置时区和NTP服务器

    // rtc.getDateTime(&timeinfo);
    // rtc.hwClockWrite();
}

void loop()
{
    rtc.getDateTime(&timeinfo);
    Serial.printf("%d-%d-%d %d:%d:%d %d\n", timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_wday);
    delay(1000);
}