<h1 align = "center">🌟T-Display-Bar🌟</h1> 

## 1️⃣Product

| Product       | Combination | Resolution  | Size          | TFT-Driver |
| ------------- | ----------- | ----------- | ------------- | ---------- |
| T-Display-Bar | ESP32S3-r8  | 76*284(RGB) | IPS 2.25 Inch | ST7789     |

## 2️⃣Examples

| Example                  | Description                                                        |
| ------------------------ | ------------------------------------------------------------------ |
| Factory                  | Factory test example, Includes desktop weather and Bluetooth mouse |
| TFT_ESPI_Example         | TFT_espi library adaptation                                        |
| LVGL_Example             | lvgl library adaptation                                            |
| WIFI_Config              | wifi configuration example                                         |
| BHI260AP                 | 6 axis IMU sensor example                                          |
| BHI260AP_StepCounter     | walking step counter example                                       |
| BQ27220                  | Battery level monitor meter                                        |
| BQ25896                  | Power control chip routines                                        |
| RTC                      | PCF8563 Real-Time Clock example                                    |
| SD Card                  | SD card example                                                    |
| Colour_Test              | TFT display colour test                                            |
| Colour_Test2             | TFT display colour test                                            |
| TouchDrv_CSTxxx_GetPoint | display touch example                                              |
| Sleep_Mode               | ESP32S3 sleep mode example                                         |

## 3️⃣ PlatformIO Quick Start (Recommended)

1. Install [Visual Studio Code](https://code.visualstudio.com/) and [Python](https://www.python.org/)
2. Search for the `PlatformIO` plugin in the `VisualStudioCode` extension and install it.
3. After the installation is complete, you need to restart `VisualStudioCode`
4. After restarting `VisualStudioCode`, select `File` in the upper left corner of `VisualStudioCode` -> `Open Folder` -> select the `T-Display-Bar` directory
5. Wait for the installation of third-party dependent libraries to complete
6. Click on the `platformio.ini` file, and in the `platformio` column
7. Uncomment one of the lines `default_envs = xxxx` to make sure only one line works
8. Uncomment `src_dir = xxxx` from the routine you want to execute to make sure only one line is correct
9. Click the (✔) symbol in the lower left corner to compile
10. Connect the board to the computer USB
11. Click (→) to upload firmware
12. Click (plug symbol) to monitor serial output

## 4️⃣ Arduino IDE Quick Start

1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. In Arduino Preferences, on the Settings tab, enter the `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json` URL in the `Additional boards manager URLs` input box. **Please pay attention to the version. The test phase is using 2.0.14. It is not certain that versions above 2.0.14 can run. When the operation is abnormal, please downgrade to a version below 2.0.14.** , As of 2024/08/02, TFT_eSPI does not work on versions higher than 2.0.14, see [TFT_eSPI/issue3329](https://github.com/Bodmer/TFT_eSPI/issues/3329)
3. Download `T-Display-Bar` , move to Arduino library folder (e.g. C:\Users\YourName\Documents\Arduino\libraries)
4. Copy all folders in [lib folder](./lib/) to Arduino library folder (e.g. C:\Users\YourName\Documents\Arduino\libraries)
5. Enter the downloaded `T-Display-Bar/examples` directory
6. Select any example and double-click the `any_example.ino` to open it
7. Open ArduinoIDE ,`Tools` ,Make your selection according to the table below

    | Arduino IDE Setting                  | Value                                   |
    | ------------------------------------ | --------------------------------------- |
    | Board                                | **ESP32S3 Dev Module**                  |
    | Port                                 | Your port                               |
    | USB CDC On Boot                      | Enable                                  |
    | CPU Frequency                        | 240MHZ(WiFi)                            |
    | Core Debug Level                     | None                                    |
    | USB DFU On Boot                      | Disable                                 |
    | Erase All Flash Before Sketch Upload | Disable                                 |
    | Events Run On                        | Core1                                   |
    | Flash Mode                           | QIO 80MHZ                               |
    | Flash Size                           | **16MB(128Mb)**                         |
    | Arduino Runs On                      | Core1                                   |
    | USB Firmware MSC On Boot             | Disable                                 |
    | Partition Scheme                     | **8M with spiffs(3M APP/1.5MB SPIFFS)** |
    | PSRAM                                | **OPI PSRAM**                           |
    | Upload Mode                          | **UART0/Hardware CDC**                  |
    | Upload Speed                         | 921600                                  |
    | USB Mode                             | **CDC and JTAG**                        |

    * The options in bold are required, others are selected according to actual conditions.
8. Click `upload` , Wait for compilation and writing to complete
9.  If it cannot be written, or the USB device keeps flashing, please check the **FAQ** below

* You can also choose `LilyGo T-Display-Bar` or `ESP32S3 Dev Module` as the board, but the partition table is fixed to **8M with spiffs(3M APP/1.5MB SPIFFS)**

## 5️⃣ FAQ

## 6️⃣ PinMap

| GPIO | SPI      | I2C     | TFT(SPI) | TOUCH(I2C) | BUZZER     | BHI260AP(I2C) | BUTTON  | SD(SPI) | USB   | BQ27220 | FLASH128M | UART  |
| ---- | -------- | ------- | -------- | ---------- | ---------- | ------------- | ------- | ------- | ----- | ------- | --------- | ----- |
| IO2  |          | I2C_SDA |          |            |            |               |         |         |       |         |           |       |
| IO3  |          | I2C_SDA |          |            |            |               |         |         |       |         |           |       |
| IO12 | SPI_MOSI |         |          |            |            |               |         |         |       |         |           |       |
| IO13 | SPI_MISO |         |          |            |            |               |         |         |       |         |           |       |
| IO14 | SPI_SCK  |         |          |            |            |               |         |         |       |         |           |       |
| IO6  |          |         | TFT_MOSI |            |            |               |         |         |       |         |           |       |
| IO7  |          |         | TFT_SCLK |            |            |               |         |         |       |         |           |       |
| IO8  |          |         | TFT_CS   |            |            |               |         |         |       |         |           |       |
| IO5  |          |         | TFT_DC   |            |            |               |         |         |       |         |           |       |
| IO40 |          |         | TFT_RST  |            |            |               |         |         |       |         |           |       |
| IO15 |          |         | TFT_BL   |            |            |               |         |         |       |         |           |       |
| IO18 |          |         |          |            |            | BHI260AP_IRQ  |         |         |       |         |           |       |
| IO17 |          |         |          |            |            | BHI260AP_RST  |         |         |       |         |           |       |
| IO16 |          |         |          |            |            | BHI260AP_EN   |         |         |       |         |           |       |
| IO11 |          |         |          |            |            |               |         | SD_CS   |       |         |           |       |
| IO9  |          |         |          |            | BUZZER_PIN |               |         |         |       |         |           |       |
| IO21 |          |         |          | TOUCH_IRQ  |            |               |         |         |       |         |           |       |
| IO1  |          |         |          | TOUCH_RST  |            |               |         |         |       |         |           |       |
| IO38 |          |         |          |            |            |               | Button1 |         |       |         |           |       |
| IO0  |          |         |          |            |            |               | Button2 |         |       |         |           |       |
| IO19 |          |         |          |            |            |               |         |         | USB_N |         |           |       |
| IO20 |          |         |          |            |            |               |         |         | USB_P |         |           |       |
| IO10 |          |         |          |            |            |               |         |         |       | GPOUT   |           |       |
| IO27 |          |         |          |            |            |               |         |         |       |         | SPIHD     |       |
| IO28 |          |         |          |            |            |               |         |         |       |         | SPIWP     |       |
| IO29 |          |         |          |            |            |               |         |         |       |         | SPICS0    |       |
| IO30 |          |         |          |            |            |               |         |         |       |         | SPICLK    |       |
| IO31 |          |         |          |            |            |               |         |         |       |         | SPIQ      |       |
| IO32 |          |         |          |            |            |               |         |         |       |         | SPID      |       |
| IO44 |          |         |          |            |            |               |         |         |       |         |           | U0RXD |
| IO43 |          |         |          |            |            |               |         |         |       |         |           | U0TXD |


 
## 7️⃣ Depends on required libraries
* [ESP32-BLE-Mouse](https://github.com/T-vK/ESP32-BLE-Mouse)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [lvgl](https://github.com/lvgl/lvgl)
* [MadgwickAHRS]https://github.com/arduino-libraries/MadgwickAHRS
* [SensorsLib](https://github.com/lewisxhe/SensorsLib)
* [XPowersLib](https://github.com/lewisxhe/XPowersLib)
* [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
