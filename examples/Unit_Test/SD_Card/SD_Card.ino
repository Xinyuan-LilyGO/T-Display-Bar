#include "Arduino.h"
#include "SD.h"

SPIClass SDSPI(HSPI);
#define SPI_MOSI 12
#define SPI_MISO 13
#define SPI_SCK 14
#define SD_CS 11

void setup()
{
    Serial.begin(115200);
    SDSPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    if (!SD.begin(SD_CS, SDSPI))
    {
        Serial.println("SD card initialization failed!");
    }
    else
    {
        Serial.println("SD card initialization done.");
    }
}

void loop()
{
    Serial.print("SD_Size: ");
    Serial.print(SD.cardSize() / 1024 / 1024.0);
    Serial.println("MB");

    Serial.print("SD_Used: ");
    Serial.print(SD.usedBytes() / 1024 / 1024.0);
    Serial.println("MB");
    delay(3000);
}