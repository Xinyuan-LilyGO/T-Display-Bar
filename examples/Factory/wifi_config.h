#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <WebServer.h>
#include <Preferences.h>
#include "ui.h"

extern WebServer server;
extern Preferences preferences;

/*****define******/
#define WIFI_SSID "xinyuandianzi"
#define WIFI_PASSWORD "AA15994823428"

extern char *SSID_KEY;
extern char *PASS_KEY;

// AP模式配置
extern bool AP_MODE;
extern char *AP_SSID;
extern char *AP_PASSWORD;

void perference_init();
void setupAPMode();
void handleRoot();
void handleConfigure();

#endif