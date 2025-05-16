<h1 align = "center">🌟Factory Example🌟</h1> 

# 1. Introduction
This example is used to demonstrate the use of the Factory pattern.
Factory routines use all peripherals on the version such as BHI260AP 6-axis sensor, RTC clock, buzzer, power management, TFT screen. 
And it comes with two apps, desktop weather and Bluetooth air mouse.
Below is the factory routine button instructions:
<img src = picture/btn.png>

    RESET: 
    Reset the device

    BOOT: 
    Hold boot and press reset to release boot and enter the firmware burning mode
    
    BLK: 
    CLick to ON/OFF the screen backlight

# 2. Usage
## 2.1. Desktop Weather
Use desktop weather in program first modify your `wifi` and `wifi password` and login [openweather](https://openweathermap.org/api) registered apikey , replace your `apikey`, `cityID`, `cityname`.
```C
//Factory.ino
/***********Weather Server************/
const char *apiKey = "9f2e62fde46bd3b34d30f1b7db930df9"; // your api key
const char *cityID = "1795565";
const char *cityname = "Shenzhen"; // your city

//T-Display_Bar.h
#define WIFI_SSID "xinyuandianzi"
#define WIFI_PASSWORD "AA15994823428"
```

## 2.2. Bluetooth Air Mouse（PC or android）
### 2.2.1. Connect to the device
Open the Bluetooth of the device, click the air_mouse button twice, search the Bluetooth device on the mobile phone or pc, and AIR_MOUSE appears to connect.

### 2.2.2. Use the mouse
Enter the AIR_MOUSE interface, there are left mouse button, right mouse button, back button and slide up and down button.

    LEFT MOUSE: Click the left mouse button
    RIGHT MOUSE: Click the right mouse button
    BACK: Click the Back button
    SLIDE UP/DOWN: Long Press TOUCH button Slide up and down




