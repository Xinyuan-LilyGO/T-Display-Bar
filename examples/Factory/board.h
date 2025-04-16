#define BHY2_USE_I2C 1

#define I2C_SDA 3
#define I2C_SCL 2

#ifdef BHY2_USE_I2C
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

#define XPOWERS_CHIP_BQ25896
#define BQ27220_ADDR 0x55  // 7位地址

#define SPI_MOSI 12
#define SPI_MISO 13
#define SPI_SCK 14

#define LDO_ENABLE 16

#define SD_CS 11
#define BUZZER_PIN 9

#define TOUCH_IRQ  21
#define TOUCH_RST  1

#define Button1 38
#define Button2 39
#define Button3 0


