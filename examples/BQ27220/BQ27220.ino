#include <Arduino.h>
#include <Wire.h>
#include "bq27220.h"

#define I2C_SDA 3
#define I2C_SCL 2

BQ27220 fuelGauge(I2C_SDA, I2C_SCL);
BQ27220::BQ27220_TypeDef data;

void scanI2C()
{
    Serial.println("Scanning I2C devices...");
    for (uint8_t addr = 0x01; addr <= 0x7F; addr++)
    {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0)
        {
            Serial.printf("Device found at 0x%02X\n", addr);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    delay(2000);
    fuelGauge.unseal(data);
    uint32_t devID = fuelGauge.get_dev_id(data);
    Serial.printf("Device ID: 0x%08X\n", devID);
}

void loop()
{
    fuelGauge.read_registers(data);
    Serial.printf("----------------------------------------\n");
    Serial.printf("Temperature: %.2f °C\n", (data.tempReg * 0.1) - 273.15);
    Serial.printf("Voltage: %dmV\n", data.voltReg);
    Serial.printf("BatteryStatus: %d\n", data.flagsReg);
    Serial.printf("RelativeStateOfCharge: %d\n", data.currentReg);
    Serial.printf("Remaining Capacity: %dmAh\n", data.rmReg);
    Serial.printf("SOC: %d%%\n", data.socReg/100);
    Serial.printf("FullChargeCapacity: %dmAh\n", data.fccReg);
    Serial.println();

    delay(3000);
}
