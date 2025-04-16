<h1 align = "center">🌟T-Display-Bar🌟</h1> 

## 1️⃣Product
| Product       | Combination | Resolution  | Size          |
| ------------- | ----------- | ----------- | ------------- |
| T-Display-Bar | ESP32S3-r8  | 76*284(RGB) | IPS 2.25 Inch |

## 2️⃣Examples
| Example                  | Description                                                        |
| ------------------------ | ------------------------------------------------------------------ |
| Factory                  | Factory test example, Includes desktop weather and Bluetooth mouse |
| BQ27220                  | Battery level monitor meter                                        |
| BQ25896_Example          | Power control chip routines                                        |
| Colour_Test              | TFT display colour test                                            |
| TFT_ESPI_Example         | TFT_espi library adaptation                                        |
| LVGL_Example             | lvgl library adaptation                                            |
| TouchDrv_CSTxxx_GetPoint | display touch example                                              |
| BHI260AP                 | 6 axis IMU sensor example                                          |

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
9. If it cannot be written, or the USB device keeps flashing, please check the **FAQ** below

* You can also choose `LilyGo T-Display-Bar` or `ESP32S3 Dev Module` as the board, but the partition table is fixed to **8M with spiffs(3M APP/1.5MB SPIFFS)**
* [T-Display-Bar Arduino IDE Record](https://www.youtube.com/watch?v=PgtxisFvMcc) 

## 5️⃣ FAQ

## 6️⃣ PinMap
| ESP32S3       | IO      |
| ------------- | ------- |
| I2C_SDA       | 3       |
| I2C_SCL       | 2       |
| SPI_MOSI      | 12      |
| SPI_MISO      | 13      |
| SPI_SCK       | 14      |
| BHI260AP_MOSI | 33      |
| BHI260AP_MISO | 34      |
| BHI260AP_SCK  | 35      |
| BHI260AP_CS   | 36      |
| BHI260AP_IRQ  | 18      |
| BHI260AP_RST  | 47      |
| SD_CS         | 11      |
| BUZZER_PIN    | 9       |
| TOUCH_IRQ     | 21      |
| TOUCH_RST     | 1       |
| Button        | 38,39,0 |
 
## 7️⃣ Depends on required libraries
* [ESP32-BLE-Mouse](https://github.com/T-vK/ESP32-BLE-Mouse)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [lvgl](https://github.com/lvgl/lvgl)
* [MadgwickAHRS]https://github.com/arduino-libraries/MadgwickAHRS
* [SensorsLib](https://github.com/lewisxhe/SensorsLib)
* [XPowersLib](https://github.com/lewisxhe/XPowersLib)
* [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
