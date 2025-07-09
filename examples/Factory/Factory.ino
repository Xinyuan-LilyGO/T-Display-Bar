#include <Arduino.h>
#include <Wire.h>
#include "board.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include "esp_task_wdt.h"
#include "T-display_Bar.h"
#include "wifi_config.h"
#include "ui.h"

/*****funtion******/
void sycn_init();
void model_init();
void lvgl_drv_init();
void Openinganimation();
void Check_Error_ui();
void time_sync_notification_handler(timeval *t);
void get_weather(const String &city, const String &apiKey);
uint8_t voltage_percentage_calculation(uint16_t VBattVoltage);
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static void accel_process_callback(uint8_t sensor_id, uint8_t *data_ptr, uint32_t len);
void step_counter_process_callback(uint8_t sensor_id, uint8_t *data_ptr, uint32_t len);
void bhi260_config();
void Button1_Callback(ButtonState event);
void Button2_Callback(ButtonState event);
void Button3_Callback(ButtonState event);

/***********DISPALY************/
#define BUFFER_SIZE (TFT_WIDTH * TFT_HEIGHT)
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[BUFFER_SIZE];
static lv_color_t buf2[BUFFER_SIZE];
TFT_eSPI tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);
TouchDrvCSTXXX touch;
int16_t x[2], y[2];
int16_t move_X = 0;

/***********External equipment************/
SPIClass SDSPI(HSPI);
SD_Size sd;
XPowersPPM PPM;
About_info about_info;
GaugeBQ27220 gauge;
SensorPCF85063 rtc;
SensorBHI260AP bhy(Wire, I2C_SDA, I2C_SCL, BHI260AP_SLAVE_ADDRESS_L);
LilyGo_Button btn_38;
LilyGo_Button btn_boot;
Madgwick filter;
float roll, pitch, yaw;
float gyro_scaling_factor;
float accel_scaling_factor;
cbuf accel_data_buffer(1);
cbuf gyro_data_buffer(1);
struct bhy2_data_xyz acc;
struct bhy2_data_xyz gyr;
BleMouse bleMouse("Air-Mouse");
uint8_t pwm_channel = 0;
bool last_move_mouse = false;
bool last_wifi_connected = false;

/***********AP Mode************/
WebServer server(80);
Preferences preferences;
char *AP_SSID = "Display-Bar Config";
char *AP_PASSWORD = "";
char *SSID_KEY = "ssid";
char *PASS_KEY = "password";
HTTPClient http;
bool AP_MODE = false;

/***********Weather Server************/
const char *apiKey = "9f2e62fde46bd3b34d30f1b7db930df9"; // your api key
const char *cityID = "1795565";
const char *cityname = "Shenzhen"; // your city
String url;

/***********UI variables************/
lv_ui ui;
uint8_t src_load_page = HOME_PAGE;
uint8_t home_count = 0;
uint32_t walk_count;
struct tm timeinfo;
TaskHandle_t wifiTaskHandle = NULL;
TaskHandle_t bleTaskHandle = NULL;
TaskHandle_t imuTaskHandle = NULL;
TaskHandle_t loop_TaskHandle = NULL;
SemaphoreHandle_t xSemaphore;
int dx = 0, dy = 0;
bool move_mouse = false;
bool mouse_wheel = false;
bool tft_backlight = true;
bool no_Back = false;
Peripheral peripheral;

void setup()
{
  Serial.begin(115200);
  Serial.println("setup");

  lvgl_drv_init();
  Openinganimation();
  model_init();
  Check_Error_ui();
  enter_ui();
  sycn_init();
}

void loop()
{
  // Serial.println("Loop start");
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}

void setBacklight(uint8_t brightness)
{
  ledcWrite(pwm_channel, brightness);
}

void lvlg_task(void *pvParameters)
{
  while (1)
  {
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
    {
      lv_task_handler();
      xSemaphoreGive(xSemaphore);
      vTaskDelay(16 / portTICK_PERIOD_MS);
    }
  }
}

void imuTask(void *pvParameters)
{
  while (1)
  {
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
    {
      if (src_load_page == AIR_MOUSE_PAGE || src_load_page == BHI260_PAGE)
      {
        Wire.setClock(1000000); // 设置I2C时钟频率为400kHz
        bhy.update();
        gyro_data_buffer.read((char *)&gyr, sizeof(gyr));
        accel_data_buffer.read((char *)&acc, sizeof(acc));
        filter.updateIMU(gyr.x * gyro_scaling_factor,
                         gyr.y * gyro_scaling_factor,
                         gyr.z * gyro_scaling_factor,
                         acc.x * accel_scaling_factor,
                         acc.y * accel_scaling_factor,
                         acc.z * accel_scaling_factor);
        roll = filter.getRoll();
        pitch = filter.getPitch() + 90; // 控制上下 0 <--> 180
        yaw = filter.getYaw();          // 控制左右 0 <--> 360
      }
      // uint32_t static time = 0;
      // if (millis() - time > 3000)
      // {
      //   Serial.println("IMU task");
      //   time = millis();
      //   Serial.printf("%f,%f,%f\n", roll, pitch, yaw);
      //   Serial.printf("bleMouse: %d\n", bleMouse.isConnected());
      // }
      xSemaphoreGive(xSemaphore);
    }

    if (src_load_page == AIR_MOUSE_PAGE && bleMouse.isConnected())
    {
      static float last_pitch = 0, last_yaw = 0;
      float delta_yaw = yaw - last_yaw;
      float delta_pitch = pitch - last_pitch;

      if (delta_yaw >= 180 || delta_yaw <= -180)
      {
        if (delta_yaw > 0)
          delta_yaw = delta_yaw - 360;
        else if (delta_yaw < 0)
          delta_yaw = delta_yaw + 360;
      }

      // Serial.printf("%f,%f\n", yaw, delta_yaw);

      last_pitch = pitch;
      last_yaw = yaw;

      // 动态灵敏度调整
      float speed_factor = sqrt(delta_pitch * delta_pitch + delta_yaw * delta_yaw);
      // float sensitivity = map(speed_factor, 0, 5.0, 30.0, 80.0); // 慢速10px/deg, 快速50px/deg

      float sensitivity = 30.0 + 50.0 * (1.0 - exp(-speed_factor * 0.4));
      // 新增：加入IIR低通滤波（系数0.2-0.5之间调节）
      static float filtered_dx = 0, filtered_dy = 0;
      filtered_dx = 0.3 * filtered_dx + 0.7 * (delta_yaw * -sensitivity);
      filtered_dy = 0.3 * filtered_dy + 0.7 * (delta_pitch * -sensitivity);

      // 转换为光标位移（限制最大速度）
      dx = constrain((int)filtered_dx, -127, 127);
      dy = constrain((int)filtered_dy, -127, 127);

      // 优化死区处理（动态阈值 + 滞环控制）
      static float last_dx = 0, last_dy = 0;
      static int stable_counter = 0;

      // 1. 动态噪声阈值（根据历史波动自动调整）
      static float noise_threshold = 1.5;
      noise_threshold = 0.9 * noise_threshold + 0.1 * (fabs(dx - last_dx) + fabs(dy - last_dy)) / 2;

      // 2. 滞环控制（双阈值机制）
      if (fabs(dx) < fmax(4.0, noise_threshold * 2) &&
          fabs(dy) < fmax(4.0, noise_threshold * 2))
      {
        stable_counter++;
        if (stable_counter < 5)
        { // 持续5帧静止才归零
          dx = 0;
          dy = 0;
        }
      }
      else
      {
        stable_counter = 0;
      }

      // 3. 运动状态检测（防止误触发）
      if (stable_counter >= 5)
      {
        dx = 0;
        dy = 0;
        last_dx = 0; // 重置历史位置
        last_dy = 0;
      }
      else
      {
        last_dx = dx;
        last_dy = dy;
      }

      // 4. 预测系数动态调整（静止时不预测）
      float predict_factor = (stable_counter < 5) ? 1.2 : 1.0;
      dx *= predict_factor;
      dy *= predict_factor;
      // Serial.printf("%d,%d\n", dx, dy);

      if (bleMouse.isConnected())
      {
        if (mouse_wheel)
        {
          bleMouse.move(dx, dy, move_X / 2, 0);
          vTaskDelay(30 / portTICK_PERIOD_MS);
        }
        else
        {
          bleMouse.move(dx, dy);
        }
      }
      // }
      // Serial.printf("%d,%d,%f,%f,%f\n", dx, dy,roll, pitch, yaw);
      vTaskDelay(8 / portTICK_PERIOD_MS);
    }
    else
    {
      vTaskDelay(30 / portTICK_PERIOD_MS);
    }
  }
}

void wifiTask(void *pvParameters)
{
  vTaskDelay(4000 / portTICK_PERIOD_MS);
  perference_init();

  while (1)
  {
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
    {
      if (AP_MODE)
        server.handleClient(); // 处理Web请求

      if (WiFi.status() == WL_CONNECTED)
      {
        sntp_set_time_sync_notification_cb(time_sync_notification_handler); // 注册时间同步通知回调函数
        configTzTime(CFG_TIME_ZONE, NTP_SERVER1, NTP_SERVER2);              // 设置时区和NTP服务器
        get_weather(cityname, apiKey);
        xSemaphoreGive(xSemaphore);
        vTaskDelay(60000 / portTICK_PERIOD_MS);
      }
      xSemaphoreGive(xSemaphore);
      vTaskDelay(200 / portTICK_PERIOD_MS);
    }
  }
}

void loop_Task(void *pvParameters)
{
  while (1)
  {
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
    {
      static uint32_t power_check_time = 0;
      static uint8_t sd_check_count = 0;

      btn_38.update();
      btn_boot.update();
      if (millis() - power_check_time > 3000)
      {
        // Serial.println("loop");
        if (src_load_page == HOME_PAGE)
        {
          Wire.setClock(400000); // 设置I2C时钟频率为400kHz
          if (gauge.refresh())
          {
            about_info.VBattVoltage = gauge.getVoltage(); // 电池电压
            about_info.Percentage = voltage_percentage_calculation(about_info.VBattVoltage);
            // Serial.print("\t- BatteryVoltage:"); Serial.print(about_info.VBattVoltage); Serial.println(" mV");
            // Serial.print("\t- StateOfCharge:"); Serial.print(about_info.Percentage); Serial.println(" %");
          }
        }

        if (PPM.getVbusVoltage() > 2800)
        {
          if (!PPM.isEnableCharge())
          {
            PPM.enableCharge();
            about_info.ChargeStatus = PPM.getChargeStatusString(); // 充电状态
            Serial.println("USB connected, charging enabled.");
          }
        }
        else
        {
          if (PPM.isEnableCharge())
          {
            PPM.disableCharge();
            about_info.ChargeStatus = PPM.getChargeStatusString(); // 充电状态
            Serial.println("USB not connected, charging disabled.");
          }
        }

        if (src_load_page == SD_PAGE && sd_check_count > 10)
        {
          sd_check_count = 0;
          sd.sd_size = SD.cardSize() / 1024 / 1024.0;
          sd.sd_used = SD.usedBytes() / 1024 / 1024.0;
          sd_check_count++;
        }

        if (src_load_page == ABOUT_PAGE)
        {
          Wire.setClock(400000); // 设置I2C时钟频率为400kHz
          if (gauge.refresh())
          {
            about_info.VBattVoltage = gauge.getVoltage(); // 电池电压
            about_info.Percentage = voltage_percentage_calculation(about_info.VBattVoltage);
            about_info.BatteryTemp = gauge.getTemperature();
            about_info.ChargeCurrent = gauge.getCurrent();
          }
          about_info.ChargeTargetVoltage = PPM.getChargeTargetVoltage(); // 充电目标电压
          about_info.VBusVoltage = PPM.getVbusVoltage();                 // USB总线电压
          about_info.VSysVoltage = PPM.getSystemVoltage();               // 系统电压

          // Serial.print("\t- InstantaneousCurrent:"); Serial.print(about_info.ChargeCurrent); Serial.println(" mAh");
          // Serial.print("\t- Temperature:"); Serial.print(about_info.BatteryTemp); Serial.println(" ℃");
          // Serial.printf("getVbusVoltage:%d \n", about_info.VBusVoltage);
          // Serial.printf("VSysVoltage:%d \n", about_info.VSysVoltage);

          if (WiFi.isConnected())
          {
            about_info.Rssi = WiFi.RSSI();
          }
        }
        power_check_time = millis();
      }
      xSemaphoreGive(xSemaphore);
      vTaskDelay(20 / portTICK_PERIOD_MS);
    }
  }
}

void sycn_init()
{
  xTaskCreatePinnedToCore(
      imuTask,
      "imu",
      4096,
      NULL,
      2,
      &imuTaskHandle,
      0);

  xTaskCreatePinnedToCore(
      wifiTask,
      "wifi",
      4096,
      NULL,
      1,
      &wifiTaskHandle,
      0);

  xTaskCreatePinnedToCore(
      loop_Task,
      "loop",
      4096,
      NULL,
      1,
      &loop_TaskHandle,
      0);
}

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

void model_init()
{
  // Wire.begin(I2C_SDA, I2C_SCL);
  /**********Battery detection************/
  if (!gauge.begin(Wire, I2C_SDA, I2C_SCL))
  {
    Serial.println("Failed to BQ27220 - check your wiring!");
    peripheral.BQ27220 = false;
  }
  else
  {
    Serial.println("Init gauge success!");
    peripheral.BQ27220 = true;
    uint16_t newDesignCapacity = 400;
    uint16_t newFullChargeCapacity = 400;
    gauge.setNewCapacity(newDesignCapacity, newFullChargeCapacity);
  }

  /*********Power Management**********/
  bool result = PPM.init(Wire, I2C_SDA, I2C_SCL, BQ25896_SLAVE_ADDRESS);
  if (result == false)
  {
    Serial.println("PPM is not online...");
    peripheral.PPM = false;
  }
  else
  {
    peripheral.PPM = true;
    Serial.println("Init PPM success!");
    PPM.setSysPowerDownVoltage(3200);
    PPM.setChargeTargetVoltage(4208); // 3364mv
    PPM.setPrechargeCurr(64);
    PPM.setChargerConstantCurr(1024);

    about_info.ChargeTargetVoltage = PPM.getChargeTargetVoltage(); // 充电目标电压

    if (PPM.getVbusVoltage() > 2800)
    {
      PPM.enableCharge();
      Serial.println("USB connected, charging enabled.");
    }
    else
    {
      PPM.disableCharge();
      Serial.println("USB not connected, charging disabled.");
    }
    about_info.ChargeStatus = PPM.getChargeStatusString(); // 充电状态
  }

  /*********RTC**********/
  if (!rtc.begin(Wire, PCF85063_SLAVE_ADDRESS, I2C_SDA, I2C_SCL))
  {
    peripheral.RTC = false;
    Serial.println("Failed to find PCF8563 - check your wiring!");
  }
  else
  {
    peripheral.RTC = true;
    Serial.println("Init RTC PCF8563 success!");
    rtc.getDateTime(&timeinfo);
    web_data.time.year = timeinfo.tm_year + 1900;
    web_data.time.month = timeinfo.tm_mon + 1;
    web_data.time.day = timeinfo.tm_mday;
    web_data.time.hour = timeinfo.tm_hour;
    web_data.time.minute = timeinfo.tm_min;
    web_data.time.second = timeinfo.tm_sec;
    web_data.time.week[timeinfo.tm_wday];
    Serial.printf("%d-%d-%d %d:%d:%d %s\n", web_data.time.year, web_data.time.month, web_data.time.day,
                  web_data.time.hour, web_data.time.minute, web_data.time.second, web_data.time.week[timeinfo.tm_wday]);
  }

  /*********BHI260AP**********/
  Wire.setClock(10000000);
  pinMode(BHI260_LDO_ENABLE, OUTPUT);
  digitalWrite(BHI260_LDO_ENABLE, HIGH);
  bhy.setPins(BHI260AP_RST, BHI260AP_IRQ);
  if (!bhy.init(Wire, I2C_SDA, I2C_SCL, BHI260AP_SLAVE_ADDRESS_L))
  {
    peripheral.BHI260AP = false;
    Serial.println(bhy.getError());
    Serial.print("Failed to init BHI260AP - ");
  }
  else
  {
    peripheral.BHI260AP = true;
    Serial.println("Init BHI260AP Sensor success!");
    delay(5);
    bhi260_config();
  }

  /*********TOUCH**********/
  pinMode(TOUCH_RST, OUTPUT);
  digitalWrite(TOUCH_RST, LOW);
  delay(30);
  digitalWrite(TOUCH_RST, HIGH);
  delay(50);
  touch.setPins(TOUCH_RST, TOUCH_IRQ);
  if (touch.begin(Wire, CST816_SLAVE_ADDRESS, I2C_SDA, I2C_SCL))
  {
    peripheral.TOUCH = true;
    Serial.print("Model :");
    Serial.println(touch.getModelName());
    touch.setMaxCoordinates(TFT_HEIGHT, TFT_WIDTH + 1);
    touch.setSwapXY(SWAP_XY);
    touch.setMirrorXY(MIRROR_X, MIRROR_Y);
    touch.setCenterButtonCoordinate(TFT_HOME_BTN_X, TFT_HOME_BTN_Y);
    touch.setHomeButtonCallback([](void *user_data)
                                {
                                home_count++;
                                if (home_count > 15)
                                {
                                  home_count = 0;
                                  src_load_page = HOME_PAGE;
                                  lv_scr_load_anim(ui.screen_main, LV_SCR_LOAD_ANIM_FADE_ON, 150, 0, false);
                                } }, NULL);
  }
  else
  {
    peripheral.TOUCH = false;
    Serial.println("Failed to init CST816 - check your wiring!");
  }

  /*********SD CARD**********/
  SDSPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  if (!SD.begin(SD_CS, SDSPI, 40000000))
  {
    peripheral.SD = false;
    Serial.println("SD card initialization failed!");
  }
  else
  {
    Serial.print("SD initialization done.");
    peripheral.SD = true;
    sd.sd_size = SD.cardSize() / 1024 / 1024.0;
    sd.sd_used = SD.usedBytes() / 1024 / 1024.0;
    Serial.print(sd.sd_size);
    Serial.println("MB");
    Serial.print(sd.sd_used);
    Serial.println("MB");
  }

  /*********Button**********/
  Serial.println("Button");
  btn_38.init(Button1, 50, nullptr);
  btn_38.setEventCallback(Button_38_Callback);
  btn_boot.init(Button2, 50, nullptr);
  btn_boot.setEventCallback(Button_Boot_Callback);

  // // /*********BUZZER**********/
  if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
  {
    Serial.println("Buzzer");
    tone(BUZZER_PIN, 800, 500); // 发出1000Hz的音调
    xSemaphoreGive(xSemaphore);
  }
}

void lvgl_drv_init()
{
  /*********TFT**********/
  tft.init();
  tft.setRotation(SCREEN_ROTATION);
  tft.fillScreen(TFT_BLACK);

  uint8_t brightness = 255;
  while (brightness--)
  {
    analogWrite(TFT_BL, brightness);
    delay(10);
  }

  lv_init();

  lv_disp_draw_buf_init(&draw_buf, buf1, NULL, BUFFER_SIZE);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = LV_SCREEN_WIDTH;
  disp_drv.ver_res = LV_SCREEN_HEIGHT; // TFT only Support:RGB888   LVGL only Support:RGB565
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  xSemaphore = xSemaphoreCreateBinary();
  xTaskCreatePinnedToCore(
      lvlg_task,
      "lvlg",
      6144,
      NULL,
      3,
      NULL,
      1);
  xSemaphoreGive(xSemaphore);
}

void bhi260_config()
{
  accel_data_buffer.resize(sizeof(struct bhy2_data_xyz) * 2);
  gyro_data_buffer.resize(sizeof(struct bhy2_data_xyz) * 2);

  filter.begin(100);
  float sample_rate = 100.0;      /* Read out hintr_ctrl measured at 100Hz */
  uint32_t report_latency_ms = 0; /* Report immediately */

  bhy.configure(SENSOR_ID_ACC_PASS, sample_rate, report_latency_ms);
  bhy.onResultEvent(SENSOR_ID_ACC_PASS, accel_process_callback);

  bhy.configure(SENSOR_ID_GYRO_PASS, sample_rate, report_latency_ms);
  bhy.onResultEvent(SENSOR_ID_GYRO_PASS, gyro_process_callback);

  bhy.configure(SENSOR_ID_STC, sample_rate, report_latency_ms);
  bhy.onResultEvent(SENSOR_ID_STC, stc_process_callback);

  gyro_scaling_factor = get_sensor_default_scaling(BHY2_SENSOR_ID_GYRO_PASS);
  accel_scaling_factor = get_sensor_default_scaling(BHY2_SENSOR_ID_ACC_PASS);
}

void Button_38_Callback(ButtonState event) // TFT_Backlight
{
  uint8_t brightness = 0;
  switch (event)
  {
  case BTN_CLICK_EVENT:
    Serial.println("BTN_38_CLICK_EVENT");
    tft_backlight = !tft_backlight;
    if (tft_backlight)
    {
      if (last_wifi_connected)
      {
        String savedSSID = preferences.getString(SSID_KEY, "");
        String savedPASS = preferences.getString(PASS_KEY, "");
        if (savedSSID.isEmpty())
        {
          savedSSID = WIFI_SSID;
          savedPASS = WIFI_PASSWORD;
          about_info.wifissid = WIFI_SSID;
        }
        WiFi.begin(savedSSID, savedPASS);
      }

      if (last_move_mouse)
      {
        move_mouse = true;
      }

      uint8_t brightness = 200;
      while (brightness--)
      {
        analogWrite(TFT_BL, brightness);
        delay(10);
      }
      pinMode(TFT_BL, OUTPUT);
      digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);
    }
    else
    {
      uint8_t brightness = 0;
      while (brightness++)
      {
        analogWrite(TFT_BL, brightness);
        delay(10);
      }
      if (WiFi.isConnected())
      {
        last_wifi_connected = true;
        WiFi.disconnect();
      }
      else
      {
        last_wifi_connected = false;
      }
      if (move_mouse)
      {
        last_move_mouse = true;
        move_mouse = false;
      }
      else
      {
        last_move_mouse = false;
      }
      pinMode(TFT_BL, OUTPUT);
      digitalWrite(TFT_BL, !TFT_BACKLIGHT_ON);
    }
    break;
  default:
    break;
  }
}

void Button_Boot_Callback(ButtonState event) // boot wifi on/off
{
  switch (event)
  {
  case BTN_CLICK_EVENT:
    Serial.println("BTN_0_CLICK_EVENT");
    break;

  default:
    break;
  }
}

void time_sync_notification_handler(timeval *t)
{
  Serial.println("Time synchronized");
  rtc.hwClockWrite();
  struct tm timeinfo;
  rtc.getDateTime(&timeinfo);
  web_data.time.year = timeinfo.tm_year + 1900;
  web_data.time.month = timeinfo.tm_mon + 1;
  web_data.time.day = timeinfo.tm_mday;
  web_data.time.hour = timeinfo.tm_hour;
  web_data.time.minute = timeinfo.tm_min;
  web_data.time.second = timeinfo.tm_sec;
  web_data.time.week[timeinfo.tm_wday];
  Serial.printf("%d-%d-%d %d:%d:%d %s\n", web_data.time.year, web_data.time.month, web_data.time.day, web_data.time.hour, web_data.time.minute, web_data.time.second, web_data.time.week[timeinfo.tm_wday]);
}

void get_weather(const String &city, const String &apiKey)
{
  // url = "http://api.openweathermap.org/data/2.5/weather?id=" + String(cityID) + "&appid=" + String(apiKey) + "&units=metric";
  url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";
  if (http.begin(url))
  {
    // Serial.println(url);
  }

  int httpResponseCode = http.GET();
  if (httpResponseCode > 0)
  {
    String payload = http.getString();
    http.getStreamPtr();
    // Serial.println(httpResponseCode);
    // Serial.println(payload);

    // 解析JSON
    StaticJsonDocument<1024> doc;
    deserializeJson(doc, payload);
    JsonObject main = doc["main"];                                      // 获取main对象
    web_data.weather.temperature = (float)main["temp"];                 // 从main对象中获取温度
    web_data.weather.humidity = (uint8_t)main["humidity"];              // 湿度
    web_data.weather.pressure = (uint16_t)main["pressure"];             // 气压
    JsonObject wind = doc["wind"];                                      // 获取wind对象
    web_data.weather.wind_speed = (uint8_t)wind["speed"];               // 风速
    web_data.weather.weather = (const char *)doc["weather"][0]["main"]; // 天气状况

    // Serial.printf("Temperature: %.1f\n", web_data.weather.temperature);
    // Serial.printf("Humidity: %d\n", web_data.weather.humidity);
    // Serial.printf("Pressure: %d\n", web_data.weather.pressure);
    // Serial.printf("Wind Speed: %d\n", web_data.weather.wind_speed);
    // Serial.printf("City: %s\n", web_data.weather.city);
    // Serial.printf("Country: %s\n", web_data.weather.country);
    // Serial.printf("Weather: %s\n", web_data.weather.weather);
    http.end();
    Serial.print("HTTP success\n");
  }
  else
  {
    Serial.print("Error on HTTP request \n");
    Serial.println(httpResponseCode);
  }
}

uint8_t voltage_percentage_calculation(uint16_t VBattVoltage)
{
  /* | 电压区间 (V) | 对应电量 (%) | 区间特性 |
  |--------------|--------------|------------------------|
  | 4.2 ~ 3.9 | 100% ~ 90% | 电压快速下降（高电量区）|
  | 3.9 ~ 3.7 | 90% ~ 40%  | 电压平缓（中电量区） |
  | 3.7 ~ 3.4 | 40% ~ 10%  | 电压缓慢下降 |
  | 3.4 ~ 3.0 | 10% ~ 0%   | 电压快速下降（低电量区）|*/
  uint8_t Percentage = 0;
  if (VBattVoltage <= 4208 && VBattVoltage > 3900)
  {
    Percentage = 100 - (4200 - VBattVoltage) / 300.0 * 10;
  }
  else if (VBattVoltage <= 3900 && VBattVoltage > 3700)
  {
    Percentage = 100 - (3900 - VBattVoltage) / 200.0 * 40;
  }
  else if (VBattVoltage <= 3700 && VBattVoltage > 3400)
  {
    Percentage = 40 - (3700 - VBattVoltage) / 300.0 * 40;
  }
  else if (VBattVoltage <= 3400 && VBattVoltage > 3000)
  {
    Percentage = 10 - (3400 - VBattVoltage) / 400.0 * 10;
  }
  return Percentage;
}

void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  // Serial.printf("my_disp_flush %d %d %d %d\n", w, h, area->x1, area->y1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  // tft.pushPixelsDMA((uint16_t *)&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp_drv);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
  x[1] = x[0];
  bool touched = touch.getPoint(x, y, touch.getSupportTouchPoint());
  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = x[0];
    data->point.y = y[0] - 1;

    move_X = x[1] - x[0];
  }
}

static void accel_process_callback(uint8_t sensor_id, uint8_t *data_ptr, uint32_t len)
{
  struct bhy2_data_xyz data;
  bhy2_parse_xyz(data_ptr, &data);
  // float scaling_factor = get_sensor_default_scaling(sensor_id);
  // Serial.print(bhy.getSensorName(sensor_id));
  // Serial.print(":");
  // Serial.printf("%f,%f, %f\n",
  //               data.x * scaling_factor,
  //               data.y * scaling_factor,
  //               data.z * scaling_factor);
  accel_data_buffer.write((const char *)&data, sizeof(data));
}

static void gyro_process_callback(uint8_t sensor_id, uint8_t *data_ptr, uint32_t len)
{
  struct bhy2_data_xyz data;
  bhy2_parse_xyz(data_ptr, &data);
  // float scaling_factor = get_sensor_default_scaling(sensor_id);
  // Serial.print(bhy.getSensorName(sensor_id));
  // Serial.print(":");
  // Serial.printf("%f,%f,%f\r\n",
  //               data.x * scaling_factor,
  //               data.y * scaling_factor,
  //               data.z * scaling_factor
  //              );
  gyro_data_buffer.write((const char *)&data, sizeof(data));
}

static void stc_process_callback(uint8_t sensor_id, uint8_t *data_ptr, uint32_t len)
{
  walk_count = bhy2_parse_step_counter(data_ptr);
  // Serial.print(bhy.getSensorName(sensor_id));
  // Serial.print(":");
  // Serial.println(walk_count);
}
