#include <Arduino.h>
#include <WebServer.h>
#include <Preferences.h>

WebServer server(80);
Preferences preferences;
// 存储WiFi凭证的键名
const char *SSID_KEY = "ssid";
const char *PASS_KEY = "password";

// AP模式配置
const char *AP_SSID = "Display-Bar Config";
const char *AP_PASSWORD = "";

/*****define******/
#define WIFI_SSID "YOUR-WIFI-NAME"
#define WIFI_PASSWORD "YOUR-WIFI-PASSWORD"

void perference_init()
{
  // 初始化存储系统
  preferences.begin("wifi_config", false);

  // 尝试读取已保存的WiFi配置
  String savedSSID = preferences.getString(SSID_KEY, "");
  String savedPASS = preferences.getString(PASS_KEY, "");

  Serial.printf("Saved SSID:%s, PASS:%s\n", savedSSID.c_str(), savedPASS.c_str());

  if (savedSSID.isEmpty())
  {
    savedSSID = WIFI_SSID;
    savedPASS = WIFI_PASSWORD;
  }

  if (!savedSSID.isEmpty())
  {
    //   尝试连接保存的WiFi
    WiFi.begin(savedSSID.c_str(), savedPASS.c_str());

    Serial.print("Connecting to ");
    Serial.println(savedSSID);

    // 等待连接结果
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 8000)
    {
      delay(500);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      // WiFi.onEvent(WiFiEvent);
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
  WiFi.disconnect(true);  // 彻底断开之前的连接
  WiFi.mode(WIFI_AP_STA); // 双模式兼容
  delay(500);
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

void handleConfigure()
{
  String newSSID = server.arg("ssid");
  String newPASS = server.arg("password");
  Serial.printf("Received SSID: %s\n", newSSID.c_str());
  Serial.printf("Received Password: %s\n", newPASS.c_str());

  if (newSSID.length() == 0)
  {
    server.send(400, "text/plain", "SSID cannot be empty");
    return;
  }

  WiFi.disconnect(true); // 彻底断开之前的连接
  // 尝试连接新网络
  WiFi.begin(newSSID.c_str(), newPASS.c_str());

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 8000)
  {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    // 保存新凭证
    preferences.putString(SSID_KEY, newSSID);
    preferences.putString(PASS_KEY, newPASS);

    server.send(200, "text/plain",
                "Connected successfully!\n"
                "Device will restart in STA mode.\n"
                "New IP: " +
                    WiFi.localIP().toString());
    delay(2000);
    ESP.restart();
  }
  else
  {
    // 清除错误凭证
    preferences.remove(SSID_KEY);
    preferences.remove(PASS_KEY);
    delay(500);
    server.send(500, "text/plain",
                "Connection failed! Please check credentials.\n"
                "AP mode will remain active.");
  }
  preferences.end(); // 必须关闭存储
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting...");
  Serial.printf("Used space: %d/%d bytes\n",
                preferences.freeEntries(), preferences.freeEntries());

  perference_init();
}

void loop()
{
  server.handleClient(); // 处理Web请求
  delay(10);
}