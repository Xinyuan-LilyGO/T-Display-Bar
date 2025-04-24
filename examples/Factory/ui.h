#ifndef UI_H
#define UI_H

#include "lvgl.h"
#include "Arduino.h"
#include "TFT_eSPI.h"
#include "SensorPCF85063.hpp"
#include <XPowersLib.h>
#include <BleMouse.h>

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>

// SET SCREEN_ROTATION
#define SCREEN_ROTATION 1 // 0: 0°, 1: 90°, 2: 180°, 3: 270°
#define TFT_WIDTH 76
#define TFT_HEIGHT 284

#define LV_SCREEN_COMPENSATION 0

#if (SCREEN_ROTATION == 1 || SCREEN_ROTATION == 3)
#define LV_SCREEN_WIDTH TFT_HEIGHT
#define LV_SCREEN_HEIGHT TFT_WIDTH
#elif (SCREEN_ROTATION == 0 || SCREEN_ROTATION == 2)
#define LV_SCREEN_WIDTH TFT_HEIGHT
#define LV_SCREEN_HEIGHT TFT_WIDTH
#endif

/*If you rotate the screen, modify the touch coordinate system to make sure it fits the rotated screen
Here is the touch configuration with rotation 1*/
// Flip the XY axis, set to 1
#define SWAP_XY 1
// Define a constant MIRROR_X with a value of 0 to mirror the X axis
#define MIRROR_X 0
// Define a constant MIRROR_Y with a value of 1 to mirror the Y axis
#define MIRROR_Y 1

// HOME BTN
#define TFT_HOME_BTN_X 30
#define TFT_HOME_BTN_Y 400

#define WEATHER_CLEAR "Clear"
#define WEATHER_CLOUDS "Clouds"
#define WEATHER_RAIN "Rain"
#define WEATHER_SNOW "Snow"

	typedef struct
	{
		lv_obj_t *screen_main;	   // 主界面
		lv_obj_t *screen_label_bg; // 背景
		bool screen_mian_del;
		lv_obj_t *screen_cont_info;
		lv_obj_t *screen_label_time;
		lv_obj_t *screen_cont_gradline;
		lv_obj_t *screen_cont_icon;
		lv_obj_t *screen_img_wifi_icon;
		lv_obj_t *screen_img_ble_icon;
		lv_obj_t *screen_cont_app;
		lv_obj_t *screen_label_app_bg;
		lv_obj_t *screen_label_app_bg2;
		lv_obj_t *screen_label_app_bg3;
		lv_obj_t *screen_label_app_bg4;
		lv_obj_t *screen_btn_weather;
		lv_obj_t *screen_btn_weather_label;
		lv_obj_t *screen_btn_mouse;
		lv_obj_t *screen_btn_mouse_label;
		lv_obj_t *screen_btn_set;
		lv_obj_t *screen_btn_set_label;
		lv_obj_t *screen_img_weather;
		lv_obj_t *screen_img_mouse;
		lv_obj_t *screen_img_set;

		lv_obj_t *screen_cont_bettery0;
		lv_obj_t *screen_cont_bettery1;
		lv_obj_t *screen_cont_bettery2;
		lv_obj_t *screen_label_bettery_num;

		lv_obj_t *screen_btn_shotdown;
		lv_obj_t *screen_btn_shotdown_label;
		lv_obj_t *screen_img_shotdown;

		lv_obj_t *screen_main_blehid; // 主界面
		lv_obj_t *screen_blehid_bg;	  // 背景
		bool screen_main_del;
		lv_obj_t *screen_btn_right;
		lv_obj_t *screen_btn_right_label;
		lv_obj_t *screen_btn_left;
		lv_obj_t *screen_btn_left_label;
		lv_obj_t *screen_btn_touch;
		lv_obj_t *screen_btn_touch_label;

		lv_obj_t *screen_main_weather;
		bool screen_weather_del;
		lv_obj_t *screen_weather_location;
		lv_obj_t *screen_weather_temp;
		lv_obj_t *screen_weather_datatime;
		lv_obj_t *screen_weather_img_location;
		lv_obj_t *screen_weather_img_weather[4];
		lv_obj_t *screen_weather_cont_pressure;
		lv_obj_t *screen_weather_pressure_num;
		lv_obj_t *screen_weather_img_pressure;
		lv_obj_t *screen_weather_cont_wind;
		lv_obj_t *screen_weather_wind_num;
		lv_obj_t *screen_weather_img_wind;
		lv_obj_t *screen_weather_cont_humidity;
		lv_obj_t *screen_weather_humidity_num;
		lv_obj_t *screen_weather_img_humidity;

		lv_obj_t *screen_main_about;
		bool screen_about_del;
		lv_obj_t *screen_about_cont_about;
		lv_obj_t *screen_about_label_mcu;
		lv_obj_t *screen_about_label_wifirssi;
		lv_obj_t *screen_about_label_Battery_level;
		lv_obj_t *screen_about_label_vbus;
		lv_obj_t *screen_about_label_vbatt;
		lv_obj_t *screen_about_label_vsys;
		lv_obj_t *screen_about_label_chargestatus;
		lv_obj_t *screen_about_label_current;
		lv_obj_t *screen_about_label_batterytemp;
		lv_obj_t *screen_about_label_vtarget;
		lv_obj_t *screen_about_cont_bg;

		lv_obj_t *screen_about_list;
		lv_obj_t *screen_about_item[5];
		lv_obj_t *screen_about_item_label[5];

	} lv_ui;

	typedef struct
	{
		const char *weather = "Clear";
		const char *city = "Shenzhen";
		const char *country = "CN";
		float temperature = 0;
		uint8_t humidity = 0;
		uint16_t pressure = 0;
		uint8_t wind_speed = 0;
	} Weather;

	typedef struct
	{
		int year = 2025;
		int month = 1;
		int day = 1;
		int hour = 12;
		int minute = 0;
		int second = 0;
		String week[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	} Time;

	typedef struct
	{
		Weather weather;
		Time time;
	} Web_data;

	typedef struct
	{
		uint16_t x;
		uint16_t y;
		uint8_t size_x;
		uint8_t size_y;
		const lv_img_dsc_t *weather_icon;
	} Info_weather;

	typedef struct
	{
		int8_t Rssi = 0;
		uint8_t Percentage = 0;
		uint16_t VBusVoltage = 0;
		uint16_t VBattVoltage = 0;
		uint16_t VSysVoltage = 0;
		uint16_t ChargeTargetVoltage = 0;
		uint16_t ChargeCurrent = 0;
		const char *ChargeStatus = "NULL";
		int8_t BatteryTemp = 0;
	} Power_info;

	LV_IMG_DECLARE(_wifi_alpha_22x22);
	LV_IMG_DECLARE(_ble2_alpha_22x22);
	LV_IMG_DECLARE(_weather_alpha_30x30);
	LV_IMG_DECLARE(_shezhi_alpha_30x30);
	LV_IMG_DECLARE(_info_circle_alpha_30x30);
	LV_IMG_DECLARE(_mouse_alpha_30x30);
	LV_IMG_DECLARE(_shutdown_alpha_30x30);

	LV_IMG_DECLARE(_humidity_alpha_20x20);
	LV_IMG_DECLARE(_location_fill_alpha_12x12);
	LV_IMG_DECLARE(_pressure_alpha_20x20);
	LV_IMG_DECLARE(_sun_50x50);
	LV_IMG_DECLARE(_snow_50x50);
	LV_IMG_DECLARE(_rain_50x50);
	LV_IMG_DECLARE(_suncloud_alpha_50x50);
	LV_IMG_DECLARE(_werther_bg2_284x76);
	LV_IMG_DECLARE(_Wind_alpha_20x20);

	LV_IMG_DECLARE(_LOGO1_58x66);

	LV_FONT_DECLARE(lv_font_Abel_regular_10);
	LV_FONT_DECLARE(lv_font_Adventpro_regular_10);
	LV_FONT_DECLARE(lv_font_Adventpro_regular_12);
	LV_FONT_DECLARE(lv_font_Alatsi_Regular_12);
	LV_FONT_DECLARE(lv_font_Orbitron_VariableFont_wght_14);
	LV_FONT_DECLARE(lv_font_OrbitronSemiBold_16);
	LV_FONT_DECLARE(lv_font_OrbitronBlack_12);
	LV_FONT_DECLARE(lv_font_OrbitronBlack_16);
	LV_FONT_DECLARE(lv_font_OrbitronBlack_20);
	LV_FONT_DECLARE(lv_font_OrbitronBlack_22);
	LV_FONT_DECLARE(lv_font_OrbitronBlack_25);
	LV_FONT_DECLARE(lv_font_OrbitronBlack_28);
	LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_12);
	LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_16);
	LV_FONT_DECLARE(lv_font_TrainOneRegular_40);
	LV_FONT_DECLARE(lv_font_OswaldVariableFont_wght_22);
	LV_FONT_DECLARE(lv_font_Acme_Regular_8);
	LV_FONT_DECLARE(lv_font_Acme_Regular_10);
	LV_FONT_DECLARE(lv_font_Acme_Regular_12);
	LV_FONT_DECLARE(lv_font_Acme_Regular_14);
	LV_FONT_DECLARE(lv_font_Acme_Regular_22);

	void enter_ui(void);
	void wifi_ap_init();
	// void cont_weather_info(lv_obj_t *param,Info_weather *info_buf,lv_ui *ui);
	void about_info_label(lv_obj_t *param, String info, int x, int y);

#ifdef __cplusplus
}
#endif

#endif