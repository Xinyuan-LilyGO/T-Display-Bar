#ifndef T_DISPLAY_BAR_H
#define T_DISPLAY_BAR_H

#include "SPI.h"
#include "WiFi.h"
#include "TouchDrvCSTXXX.hpp"
#include "SensorBHI260AP.hpp"
#include "LilyGo_Button.h"
#include "TFT_eSPI.h"
#include "SD.h"
#include <esp_sntp.h>
#include "ArduinoJson.h"
#include <MadgwickAHRS.h> //MadgwickAHRS from https://github.com/arduino-libraries/MadgwickAHRS
#include <HTTPClient.h>
#include "lvgl.h"
// #include "bq27220.h"
#include <GaugeBQ27220.hpp>

/***********Weather Server************/
#define WEATHER_UPDATE_TIME 1

/***********Time Server************/
#define CFG_TIME_ZONE "CST-8" // TZ_Asia_Shanghai
#define NTP_SERVER1 "pool.ntp.org"
#define NTP_SERVER2 "time.nist.gov"
#define WIFI_MSG_ID 0x1001

/**********Air_Mouse config***********/
#define CALIBRATION_SAMPLES 100
#define SENSITIVITY_X 700
#define SENSITIVITY_Y 700
#define ACCEL_LPF 0.25
#define GYRO_LPF 0.35
#define VELOCITY_DECAY 0.8
#define SAMPLE_RATE 100

#endif
