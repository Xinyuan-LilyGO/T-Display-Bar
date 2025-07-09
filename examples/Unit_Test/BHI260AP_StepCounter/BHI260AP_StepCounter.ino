#include "Arduino.h"
#include "Wire.h"
#include "SensorBHI260AP.hpp"

#define BHY2_USE_I2C 1

#define BHI260AP_SDA 3
#define BHI260AP_SCL 2

#ifdef BHY2_USE_I2C
#define BHI260AP_IRQ 18
#define BHI260AP_RST 17
#endif
SensorBHI260AP bhy;

void stc_process_callback(uint8_t sensor_id, uint8_t *data_ptr, uint32_t len)
{
    Serial.print(bhy.getSensorName(sensor_id));
    Serial.print(":");
    Serial.println(bhy2_parse_step_counter(data_ptr));
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
    // Set the reset pin and interrupt pin, if any
    bhy.setPins(BHI260AP_RST, BHI260AP_IRQ);
    if (!bhy.init(Wire, BHI260AP_SDA, BHI260AP_SCL, BHI260AP_SLAVE_ADDRESS_L))
    {
        Serial.print("Failed to init BHI260AP - ");
        Serial.println(bhy.getError());
        while (1)
        {
            delay(1000);
        }
    }
    Serial.println("Init BHI260AP Sensor success!");
    bhy.printSensors(Serial);

    float sample_rate = 100.0;      /* Read out hintr_ctrl measured at 100Hz */
    uint32_t report_latency_ms = 0; /* Report immediately */

    bhy.configure(SENSOR_ID_STC, sample_rate, report_latency_ms);
    bhy.onResultEvent(SENSOR_ID_STC, stc_process_callback);
}

void loop()
{
    bhy.update();
    delay(50);
}
