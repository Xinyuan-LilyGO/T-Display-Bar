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

extern lv_ui ui;
extern BleMouse bleMouse;
extern uint8_t src_load_page;
extern Web_data web_data;
extern WiFiClass WiFi;

extern bool no_Back;
extern bool mouse_wheel;

void events_init(lv_ui *ui);
void events_init_screen_mian(lv_ui *ui);

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
        if(mouse_wheel)
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

void events_init_screen_mian(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_btn_weather, screen_btn_weather_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui->screen_btn_mouse, screen_btn_mouse_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui->screen_btn_set, screen_btn_set_event_handler, LV_EVENT_ALL, NULL);
}

void events_init_screen_blehid(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_btn_right, screen_btn_right_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui->screen_btn_left, screen_btn_left_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui->screen_btn_touch, screen_btn_touch_event_handler, LV_EVENT_ALL, NULL);
}
