#include <Arduino.h>
#include <Wire.h>
#include "board.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include "esp_task_wdt.h"
#include "T-display_Bar.h"
#include "ui.h"
#include <WebServer.h>
#include <Preferences.h>

/*****funtion******/
void perference_init();
void setupAPMode();
void handleRoot();
void handleConfigure();
void sycn_init();
void model_init();
void lvgl_init();
void time_sync_notification_handler(timeval *t);
void get_weather(const String &city, const String &apiKey);
static void WiFiEvent(WiFiEvent_t event);
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static void accel_process_callback(uint8_t sensor_id, uint8_t *data_ptr, uint32_t len);
void step_counter_process_callback(uint8_t sensor_id, uint8_t *data_ptr, uint32_t len);
void bhi260_config();
void Button1_Callback(ButtonState event);
void Button2_Callback(ButtonState event);
void Button3_Callback(ButtonState event);

/*****var******/
#define BUFFER_SIZE (TFT_HEIGHT * 5)
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[BUFFER_SIZE];
static lv_color_t buf2[BUFFER_SIZE];
TFT_eSPI tft = TFT_eSPI();
TouchDrvCSTXXX touch;
int16_t x[2], y[2];
int16_t move_X = 0;

XPowersPPM PPM;
Power_info power_info;
BQ27220 bq27220(I2C_SDA, I2C_SCL, 400000); // 1000000 400000
BQ27220::BQ27220_TypeDef data;
SensorPCF85063 rtc;
SensorBHI260AP bhy;
LilyGo_Button btn1;
LilyGo_Button btn2;
LilyGo_Button btn3;
lv_ui ui;
uint8_t src_load_page = 0;

/***********AP Mode************/
WebServer server(80);
Preferences preferences;

// AP模式配置
const char *AP_SSID = "Display-Bar Config";
const char *AP_PASSWORD = "";

// 存储WiFi凭证的键名
const char *SSID_KEY = "";
const char *PASS_KEY = "";

/***********Weather Server************/
const char *apiKey = "9f2e62fde46bd3b34d30f1b7db930df9"; // your api key
const char *cityID = "1795565";
const char *cityname = "Shenzhen"; // your city
String url;

extern Web_data web_data;
uint8_t home_count = 0;
struct tm timeinfo;

Madgwick filter;
float roll, pitch, yaw;
float gyro_scaling_factor;
float accel_scaling_factor;
cbuf accel_data_buffer(1);
cbuf gyro_data_buffer(1);
struct bhy2_data_xyz acc;
struct bhy2_data_xyz gyr;

HTTPClient http;
bool wifi_connect_status = false;

TaskHandle_t wifiTaskHandle = NULL;
SemaphoreHandle_t xMutex;
BleMouse bleMouse("Air-Mouse");

int dx = 0, dy = 0;
bool wifi_on = false;
bool move_mouse = false;
bool mouse_wheel = false;
bool tft_backlight = true;
bool no_Back = false;

void setup()
{
  Serial.begin(115200);
  model_init();
  enter_ui();
  sycn_init();
}

void loop()
{
  static uint32_t power_check_time = 0;
  btn1.update();
  btn2.update();
  btn3.update();
  if (src_load_page == 2)
  {
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

    // Serial.printf("%f,%f,%f\n", roll, pitch, yaw);

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

    // // 转换为光标位移（限制最大速度）
    // dx = constrain((int)(delta_yaw * -sensitivity), -127, 127);
    // dy = constrain((int)(delta_pitch * -sensitivity), -127, 127);

    // 转换为光标位移（限制最大速度）
    dx = constrain((int)filtered_dx, -127, 127);
    dy = constrain((int)filtered_dy, -127, 127);

    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
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
    }
    xSemaphoreGive(xMutex);
    // Serial.printf("%d,%d\n", dx, dy);
    // Serial.printf("%d,%d,%f,%f,%f\n", dx, dy,roll, pitch, yaw);

    // 调试时可启用
    // Serial.printf("Raw:%.2f,%.2f | Filtered:%.2f,%.2f\n",
    //               delta_yaw * -sensitivity,
    //               delta_pitch * -sensitivity,
    //               filtered_dx, filtered_dy);
  }

  if (millis() - power_check_time > 3000)
  {
    bq27220.read_registers(data);
    power_info.VBattVoltage = data.voltReg; // 电池电压
    power_info.Percentage = constrain((uint8_t)(((power_info.VBattVoltage - 3364.0) / (4208.0 - 3364.0)) * 100), 0, 100);
    // Serial.printf("VBattVoltage:%d \n", power_info.VBattVoltage);
    // Serial.printf("Percentage:%.2f \n", power_info.Percentage);
    if (PPM.getVbusVoltage() > 2800)
    {
      if (!PPM.isEnableCharge())
      {
        PPM.enableCharge();
        power_info.ChargeStatus = PPM.getChargeStatusString(); // 充电状态
        Serial.println("USB connected, charging enabled.");
      }
    }
    else
    {
      if (PPM.isEnableCharge())
      {
        PPM.disableCharge();
        power_info.ChargeStatus = PPM.getChargeStatusString(); // 充电状态
        Serial.println("USB not connected, charging disabled.");
      }
    }

    if (src_load_page == 3)
    {
      power_info.ChargeTargetVoltage = PPM.getChargeTargetVoltage(); // 充电目标电压
      power_info.VBusVoltage = PPM.getVbusVoltage();                 // USB总线电压
      power_info.VSysVoltage = PPM.getSystemVoltage();               // 系统电压

      if (wifi_connect_status)
      {
        power_info.Rssi = WiFi.RSSI();
      }
    }
    power_check_time = millis();
  }

  lv_task_handler();
  vTaskDelay(5 / portTICK_PERIOD_MS);
}

void perference_init()
{
  // 初始化存储系统
  preferences.begin("wifi-config", false);

  // 尝试读取已保存的WiFi配置
  String savedSSID = preferences.getString(SSID_KEY, "");
  String savedPASS = preferences.getString(PASS_KEY, "");

  if(savedSSID == "")
  {
    savedSSID = WIFI_SSID;
    savedPASS = WIFI_PASSWORD;
  }

  Serial.printf("Saved SSID:%s, PASS:%s\n", savedSSID.c_str(), savedPASS.c_str());

  if (savedSSID != "")
  {
    // 尝试连接保存的WiFi
    WiFi.begin(savedSSID.c_str(), savedPASS.c_str());

    Serial.print("Connecting to ");
    Serial.println(savedSSID);

    // 等待连接结果
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000)
    {
      delay(500);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\nConnected successfully!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      return; // 连接成功，跳过AP模式
    }
  }  
  setupAPMode();
}

void setupAPMode()
{
  // 设置AP模式
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.print("AP Mode SSID: ");
  Serial.println(AP_SSID);
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  // 设置Web服务器路由
  server.on("/", HTTP_GET, handleRoot);
  server.on("/configure", HTTP_POST, handleConfigure);
  server.begin();
}

void handleRoot()
{
  String html = R"(
    <!DOCTYPE html>
    <html>
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <style>
        * {
          box-sizing: border-box;
          font-family: Arial, sans-serif;
        }
        
        body {
          margin: 0;
          padding: 20px;
          background: #f0f2f5;
          min-height: 100vh;
          display: flex;
          flex-direction: column;
        }
        
        .container {
          max-width: 400px;
          margin: auto;
          background: white;
          padding: 30px;
          border-radius: 15px;
          box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        }
        
        h1 {
          color: #1a73e8;
          text-align: center;
          margin: 0 0 25px 0;
          font-size: 24px;
        }
        
        .form-group {
          margin-bottom: 20px;
        }
        
        label {
          display: block;
          margin-bottom: 8px;
          color: #5f6368;
          font-size: 14px;
          font-weight: bold;
        }
        
        input {
          width: 100%;
          padding: 12px;
          border: 1px solid #dadce0;
          border-radius: 8px;
          font-size: 16px;
          margin-bottom: 5px;
        }
        
        input:focus {
          border-color: #1a73e8;
          outline: none;
        }
        
        button {
          width: 100%;
          background: #1a73e8;
          color: white;
          padding: 14px;
          border: none;
          border-radius: 8px;
          font-size: 16px;
          cursor: pointer;
          transition: background 0.3s;
        }
        
        button:hover {
          background: #1557b0;
        }
        
        @media (max-width: 480px) {
          .container {
            width: 100%;
            padding: 20px;
          }
          
          body {
            padding: 10px;
          }
        }
      </style>
    </head>
    <body>
      <div class="container">
        <h1>WiFi config</h1>
        <form action='/configure' method='post'>
          <div class="form-group">
            <label for="ssid">WiFi Name</label>
            <input type="text" id="ssid" name="ssid" required 
                   placeholder="Enter WiFi name">
          </div>
          <div class="form-group">
            <label for="password">WiFi Password</label>
            <input type="password" id="password" name="password" 
                   placeholder="Enter WiFi password">
          </div>
          <button type="submit">Connect to network</button>
        </form>
      </div>
    </body>
    </html>
  )";

  server.send(200, "text/html", html);
}

// 处理配置请求
void handleConfigure()
{
  String newSSID = server.arg("ssid");
  String newPASS = server.arg("password");

  if (newSSID.length() == 0)
  {
    server.send(400, "text/plain", "SSID cannot be empty");
    return;
  }

  // 保存新凭证
  preferences.putString(SSID_KEY, newSSID);
  preferences.putString(PASS_KEY, newPASS);

  // 尝试连接新网络
  WiFi.begin(newSSID.c_str(), newPASS.c_str());

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 15000)
  {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    server.send(200, "text/plain",
                "Connected successfully!\n"
                "Device will restart in STA mode.\n"
                "New IP: " +
                    WiFi.localIP().toString());
    delay(3000);
    ESP.restart();
  }
  else
  {
    server.send(500, "text/plain",
                "Connection failed! Please check credentials.\n"
                "AP mode will remain active.");
  }
}

void blehidTask(void *pvParameters)
{
  while (1)
  {
    if (bleMouse.isConnected() && move_mouse && src_load_page == 2)
    {
      if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
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
        xSemaphoreGive(xMutex);
      }
    }
    vTaskDelay(12 / portTICK_PERIOD_MS);
  }
}

void wifiTask(void *pvParameters)
{
  perference_init();

  while (1)
  {
    server.handleClient(); // 处理Web请求
    if (WiFi.status() == WL_CONNECTED)
    {
      wifi_on = true;
      wifi_connect_status = true;
      get_weather(cityname, apiKey);
      vTaskDelay(60000 / portTICK_PERIOD_MS);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void sycn_init()
{
  xMutex = xSemaphoreCreateMutex();
  xTaskCreatePinnedToCore(
      blehidTask,
      "blehid",
      4096,
      NULL,
      2,
      NULL,
      0);

  xTaskCreatePinnedToCore(
      wifiTask,
      "wifi",
      4096,
      NULL,
      1,
      &wifiTaskHandle,
      0);
}

void adjustOrientation(float &x, float &y)
{
  // 示例：设备竖屏握持时交换XY轴
  float temp = x;
  x = -y;
  y = -temp;
}

void model_init()
{
  tft.init();
  tft.setRotation(SCREEN_ROTATION);
  tft.fillScreen(TFT_WHITE);

  lvgl_init();

  pinMode(GPIO_NUM_0, INPUT_PULLUP);             // 配置按键引脚
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, LOW); // 设置按键为唤醒源（低电平唤醒）

  /*********BHI260AP**********/
#ifdef BHY2_USE_I2C
  // Using I2C interface
  bhy.setPins(BHI260AP_RST, BHI260AP_IRQ);
  if (!bhy.init(Wire, I2C_SDA, I2C_SCL, BHI260AP_SLAVE_ADDRESS_L))
  {
    Serial.println(bhy.getError());
    Serial.print("Failed to init BHI260AP - ");
    while (1)
    {
      delay(1000);
    }
  }
  // bhy.printSensors(Serial);
#endif
  Serial.println("Init BHI260AP Sensor success!");
  delay(10);
  bhi260_config();

  /*********TOUCH**********/
  pinMode(TOUCH_RST, OUTPUT);
  digitalWrite(TOUCH_RST, LOW);
  delay(30);
  digitalWrite(TOUCH_RST, HIGH);
  delay(50);
  touch.setPins(TOUCH_RST, TOUCH_IRQ);
  touch.begin(Wire, CST816_SLAVE_ADDRESS, I2C_SDA, I2C_SCL);
  Serial.print("Model :");
  Serial.println(touch.getModelName());
  touch.setMaxCoordinates(TFT_HEIGHT, TFT_WIDTH + 1);
  touch.setSwapXY(SWAP_XY);
  touch.setMirrorXY(MIRROR_X, MIRROR_Y);
  touch.setCenterButtonCoordinate(TFT_HOME_BTN_X, TFT_HOME_BTN_Y);
  touch.setHomeButtonCallback([](void *user_data)
                              { 
                                home_count++;
                                // Serial.printf("home_count:%d" + home_count); 
                                if (home_count > 15)
                                {
                                  home_count = 0;
                                  src_load_page = 0;
                                  lv_scr_load_anim(ui.screen_main, LV_SCR_LOAD_ANIM_FADE_ON, 150, 0, false);
                                } }, NULL);

  /*********PCF8563**********/
  if (!rtc.begin(Wire, PCF85063_SLAVE_ADDRESS, I2C_SDA, I2C_SCL))
  {
    Serial.println("Failed to find PCF8563 - check your wiring!");
    while (1)
    {
      delay(1000);
    }
  }
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

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  if (!SD.begin(SD_CS, SPI, 40000000))
  {
    Serial.println("SD card initialization failed!");
  }
  else
    Serial.println("Init SD card success!");

  /*********PPM**********/
  bool result = PPM.init(Wire, I2C_SDA, I2C_SCL, BQ25896_SLAVE_ADDRESS);
  if (result == false)
  {
    while (1)
    {
      Serial.println("PPM is not online...");
      delay(50);
    }
  }
  else
    Serial.println("Init PPM success!");

  PPM.setSysPowerDownVoltage(3300);
  PPM.setChargeTargetVoltage(4208); // 3364mv
  PPM.setPrechargeCurr(64);
  PPM.setChargerConstantCurr(1024);

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

  /**********BQ27220************/
  bq27220.unseal(data);
  delay(5);
  uint32_t devID = bq27220.get_dev_id(data);
  if (devID != 0)
  {
    Serial.printf("BQ27220 Device ID: 0x%08X\n", devID);
  }
  else
  {
    Serial.println("Not find BQ27220 Device!");
  }

  /*********Button**********/
  btn1.init(Button1, 50, nullptr);
  btn1.setEventCallback(Button1_Callback);
  btn2.init(Button2, 50, nullptr);
  btn2.setEventCallback(Button2_Callback);
  pinMode(Button3, INPUT_PULLUP);
  btn3.init(Button3, 50, nullptr);
  btn3.setEventCallback(Button3_Callback);

  // /*********BUZZER**********/
  pinMode(BUZZER_PIN, OUTPUT);
  tone(BUZZER_PIN, 1000); // 发出1000Hz的音调
  delay(300);
  noTone(BUZZER_PIN); // 停止发声

  // power_info.VBattVoltage = PPM.getBattVoltage(); // 电池电压
  bq27220.read_registers(data);
  power_info.VBattVoltage = data.voltReg; // 电池电压
  power_info.ChargeCurrent = data.currentReg;
  power_info.Percentage = constrain((uint8_t)(((power_info.VBattVoltage - 3364.0) / (4208.0 - 3364.0)) * 100), 0, 100);
  Serial.printf("VBattVoltage:%d \n", power_info.VBattVoltage);
  Serial.printf("Percentage:%.2f \n", power_info.Percentage);
  Serial.printf("ChargeCurrent:%.2f \n", power_info.ChargeCurrent);
}

void lvgl_init()
{
  lv_init();

  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, BUFFER_SIZE);
  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = LV_SCREEN_WIDTH;
  disp_drv.ver_res = LV_SCREEN_HEIGHT + LV_SCREEN_COMPENSATION; // TFT only Support:RGB888   LVGL only Support:RGB565
  disp_drv.offset_x = 0;
  disp_drv.offset_y = -LV_SCREEN_COMPENSATION;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
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
  gyro_scaling_factor = get_sensor_default_scaling(BHY2_SENSOR_ID_GYRO_PASS);
  accel_scaling_factor = get_sensor_default_scaling(BHY2_SENSOR_ID_ACC_PASS);
}

void Button1_Callback(ButtonState event) // TFT_Backlight
{
  switch (event)
  {
  case BTN_CLICK_EVENT:
    Serial.println("BTN_38_CLICK_EVENT");
    tft_backlight = !tft_backlight;
    if (tft_backlight)
    {
      String savedSSID = preferences.getString(SSID_KEY, "");
      String savedPASS = preferences.getString(PASS_KEY, "");
      WiFi.begin(savedSSID, savedPASS);
      pinMode(TFT_BL, OUTPUT);
      digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);
    }
    else
    {
      WiFi.disconnect();
      move_mouse = false;
      pinMode(TFT_BL, OUTPUT);
      digitalWrite(TFT_BL, !TFT_BACKLIGHT_ON);
    }
    break;
  case BTN_LONG_PRESSED_EVENT:
    Serial.println("deep sleep...");
    PPM.shutdown();
  default:
    break;
  }
}

void Button2_Callback(ButtonState event) // air mouse move  BTN39
{
  switch (event)
  {
  case BTN_CLICK_EVENT:
    Serial.println("BTN_39_CLICK_EVENT");
    if (bleMouse.isConnected())
    {
      move_mouse = !move_mouse;
      Serial.printf("move_mouse: %d\n", move_mouse);
    }

    break;
  case BTN_DOUBLE_CLICK_EVENT:
    Serial.println("BTN_39_DOUBLE_CLICK_EVENT");
    move_mouse = 1;
    if (!bleMouse.isConnected())
    {
      bleMouse.begin();
    }
    break;

    break;
  default:
    break;
  }
}

void Button3_Callback(ButtonState event) // boot wifi on/off
{
  switch (event)
  {
  case BTN_CLICK_EVENT:
    Serial.println("BTN_0_CLICK_EVENT");
    if (src_load_page == 0)
      wifi_on = !wifi_on;

    if (wifi_on)
    {
      Serial.println("WiFi on");
      String savedSSID = preferences.getString(SSID_KEY, "");
      String savedPASS = preferences.getString(PASS_KEY, "");
      WiFi.begin(savedSSID, savedSSID);
    }
    else
    {
      Serial.println("WiFi off");
      WiFi.disconnect();
    }
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
  // Serial.printf("timeinfo %d-%d-%d %d:%d:%d\n", timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
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
    JsonObject main = doc["main"];                          // 获取main对象
    web_data.weather.temperature = (float)main["temp"];     // 从main对象中获取温度
    web_data.weather.humidity = (uint8_t)main["humidity"];  // 湿度
    web_data.weather.pressure = (uint16_t)main["pressure"]; // 气压
    JsonObject wind = doc["wind"];                          // 获取wind对象
    web_data.weather.wind_speed = (uint8_t)wind["speed"];   // 风速
    // web_data.weather.city = doc["name"];                    // 城市
    // web_data.weather.country = doc["sys"]["country"];       // 国家
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

static void WiFiEvent(WiFiEvent_t event)
{
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch (event)
  {
  case ARDUINO_EVENT_WIFI_READY:
    Serial.println("WiFi interface ready");
    break;
  case ARDUINO_EVENT_WIFI_SCAN_DONE:
    Serial.println("Completed scan for access points");
    break;
  case ARDUINO_EVENT_WIFI_STA_START:
    Serial.println("WiFi client started");
    break;
  case ARDUINO_EVENT_WIFI_STA_STOP:
    Serial.println("WiFi clients stopped");
    break;
  case ARDUINO_EVENT_WIFI_STA_CONNECTED:
    sntp_set_time_sync_notification_cb(time_sync_notification_handler); // 注册时间同步通知回调函数
    configTzTime(CFG_TIME_ZONE, NTP_SERVER1, NTP_SERVER2);              // 设置时区和NTP服务器
    break;
  case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
    wifi_connect_status = false;
    Serial.println("Disconnected from WiFi access point");
    break;
  case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
    Serial.println("Authentication mode of access point has changed");
    break;
  case ARDUINO_EVENT_WIFI_STA_GOT_IP:
    Serial.print("Obtained IP address: ");
    Serial.println(WiFi.localIP());
    break;
  case ARDUINO_EVENT_WIFI_STA_LOST_IP:
    Serial.println("Lost IP address and IP address is reset to 0");
    break;
  default:
    break;
  }
}

void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
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
    data->point.y = y[0] + LV_SCREEN_COMPENSATION - 1;

    // Serial.print("Data x ");
    // Serial.println(x[0]);
    move_X = x[1] - x[0];
    // Serial.print("Data x ");
    // Serial.println(move_X);

    // Serial.print("Data y ");
    // Serial.println(y[0] + LV_SCREEN_COMPENSATION - 1);
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
