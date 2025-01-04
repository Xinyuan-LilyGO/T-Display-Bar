#include "SensorBHI260AP.hpp"
#include "SensorPCF8563.hpp"
#include "SD.h"

#define BHY2_USE_I2C 1

#ifdef BHY2_USE_I2C
#define I2C_SDA 3
#define I2C_SCL 2
#define BHI260AP_IRQ 18
#define BHI260AP_RST 17
#else
#define BHI260AP_MOSI 33
#define BHI260AP_MISO 34
#define BHI260AP_SCK 35
#define BHI260AP_CS 36
#define BHI260AP_IRQ 37
#define BHI260AP_RST 47
#endif

#define SPI_MOSI 12
#define SPI_MISO 13
#define SPI_SCK 14

#define SD_CS 11
#define BUZZER_PIN 9

SensorBHI260AP bhy;
SensorPCF8563 rtc;
