#include "Arduino.h"
#include "bq27220.h"

BQ27220 bq27220;
BQ27220BatteryStatus batt;

void setup()
{
    Serial.begin(115200);
    Wire.begin(3, 2);

    delay(200);
    Serial.println("BQ27220 Test");
    bool ret = bq27220.init();
    if (ret)
        Serial.println("BQ27220 init success");
    else
        Serial.println("BQ27220 init fail");
}

void loop()
{
    Serial.println("BQ27220 Test:");
    Serial.println(bq27220.getChargeVoltageMax());
    Serial.printf("Current: %d\n", bq27220.getCurrent());
    Serial.printf("Voltage: %d\n", bq27220.getVoltage());
    Serial.printf("Temperature: %d\n", bq27220.getTemperature());
    Serial.printf("Capacity: %d\n", bq27220.getRemainingCapacity());
    Serial.printf("Charge: %d\n", bq27220.getIsCharging());
    Serial.printf("StateOfCharge: %d\n", bq27220.getStateOfCharge());

    delay(3000);
}