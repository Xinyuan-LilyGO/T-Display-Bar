/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */
// #include "ui.h"
#include <Arduino.h>
#include <stdio.h>
#include "lvgl.h"
#include "WiFi.h"
#include "T-display_Bar.h"
#include <XPowersLib.h>

extern lv_ui ui;
extern BleMouse bleMouse;
extern uint8_t src_load_page;
extern Web_data web_data;
extern WiFiClass WiFi;
extern XPowersPPM PPM;

extern bool no_Back;
extern bool mouse_wheel;

void events_init(lv_ui *ui);
// void events_init_screen_mian(APP_ICON_INFO app_info);

void screen_btn_weather_event_handler(lv_event_t *e)
{
    // Serial.println("weather");
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        src_load_page = 1;
        Serial.printf("src_load_page:%d", src_load_page);
        lv_scr_load_anim(ui.screen_main_weather, LV_SCR_LOAD_ANIM_FADE_IN, 100, 100, false);
        break;
    }
    default:
        break;
    }
}

void screen_btn_mouse_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        src_load_page = 2;
        Serial.printf("src_load_page:%d", src_load_page);
        lv_scr_load_anim(ui.screen_main_blehid, LV_SCR_LOAD_ANIM_FADE_IN, 100, 100, false);
        break;
    }
    default:
        break;
    }
}

void screen_btn_set_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        src_load_page = 3;
        Serial.printf("src_load_page:%d", src_load_page);
        lv_scr_load_anim(ui.screen_main_about, LV_SCR_LOAD_ANIM_FADE_IN, 100, 100, false);
        break;
    }
    default:
        break;
    }
}

static void shutdown_anim_end(lv_anim_t *anim)
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, !TFT_BACKLIGHT_ON);
    Serial.printf("Shutdown...\n");
    PPM.shutdown();
}

void start_shutdown_animation()
{
    lv_obj_t *upper_mask = lv_obj_create(lv_scr_act());

    // 样式配置
    lv_obj_set_style_bg_color(upper_mask, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_width(upper_mask, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(upper_mask, 0, LV_PART_MAIN);

    // 初始位置（完全隐藏）
    lv_obj_set_size(upper_mask, LV_SCREEN_WIDTH, 0);
    lv_obj_set_pos(upper_mask, 0, 0);

    // 上半屏动画：向下扩展高度
    lv_anim_t anim_upper;
    lv_anim_init(&anim_upper);
    lv_anim_set_var(&anim_upper, upper_mask);
    lv_anim_set_values(&anim_upper, 0, LV_SCREEN_HEIGHT);
    lv_anim_set_time(&anim_upper, 800);
    lv_anim_set_exec_cb(&anim_upper, (lv_anim_exec_xcb_t)lv_obj_set_height);
    lv_anim_set_path_cb(&anim_upper, lv_anim_path_ease_out); // 缓出效果
    lv_anim_set_ready_cb(&anim_upper, shutdown_anim_end);    // 动画完成回调
    lv_anim_start(&anim_upper);
}

void screen_btn_shutdown_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_LONG_PRESSED:
    {
        start_shutdown_animation();
        break;
    }
    default:
        break;
    }
}

void screen_btn_right_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        Serial.println("mouse right");
        bleMouse.click(MOUSE_RIGHT);
        break;
    }
    default:
        break;
    }
}

void screen_btn_left_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        Serial.println("mouse left");
        bleMouse.click(MOUSE_LEFT);
        break;
    }
    default:
        break;
    }
}

void screen_btn_touch_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        Serial.println("mouse LV_EVENT_CLICKED");
        if (mouse_wheel)
        {
            no_Back = true;
        }
        else
        {
            no_Back = false;
        }

        if (!no_Back)
        {
            Serial.println("Back");
            bleMouse.click(MOUSE_BACK);
        }
        mouse_wheel = false;
        break;
    }

    case LV_EVENT_LONG_PRESSED:
    {
        Serial.println("mouse LONG_PRESSED");
        mouse_wheel = true;
        break;
    }

    default:
        break;
    }
}

void screen_sw_wifi_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_VALUE_CHANGED:
    {
        Serial.println("sw wheel");
        lv_obj_t *status_obj = lv_event_get_target(e);
        int status = lv_obj_has_state(status_obj, LV_STATE_CHECKED) ? 1 : 0;
        if (status == 1)
        {
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        }
        else
        {
            WiFi.disconnect();
        }
        break;
    }
    default:
        break;
    }
}

void events_init_screen_blehid(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_btn_right, screen_btn_right_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui->screen_btn_left, screen_btn_left_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui->screen_btn_touch, screen_btn_touch_event_handler, LV_EVENT_ALL, NULL);
}
