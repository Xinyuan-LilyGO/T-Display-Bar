/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <Arduino.h>
#include <stdio.h>
#include "lvgl.h"
#include "WiFi.h"
#include "T-display_Bar.h"
#include "wifi_config.h"
#include <XPowersLib.h>
#include "ui.h"
#include "board.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>

extern bool no_Back;
extern bool mouse_wheel;

void screen_btn_weather_event_handler(lv_event_t *e)
{
    // Serial.println("weather");
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        src_load_page = WEATHER_PAGE;
        Serial.printf("src_load_page:%d\n", src_load_page);
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
        src_load_page = AIR_MOUSE_PAGE;
        Serial.printf("src_load_page:%d\n", src_load_page);
        lv_scr_load_anim(ui.screen_main_blehid, LV_SCR_LOAD_ANIM_FADE_IN, 100, 100, false);
        break;
    }
    default:
        break;
    }
}

void screen_btn_sd_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        src_load_page = SD_PAGE;
        Serial.printf("src_load_page:%d\n", src_load_page);
        lv_scr_load_anim(ui.screen_main_sd, LV_SCR_LOAD_ANIM_FADE_IN, 100, 100, false);
        break;
    }
    default:
        break;
    }
}

void screen_btn_bhi260_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        src_load_page = BHI260_PAGE;
        Serial.printf("src_load_page:%d\n", src_load_page);
        lv_scr_load_anim(ui.screen_main_bhi260, LV_SCR_LOAD_ANIM_FADE_IN, 100, 100, false);
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
        src_load_page = SET_PAGE;
        Serial.printf("src_load_page:%d\n", src_load_page);
        lv_scr_load_anim(ui.screen_main_set, LV_SCR_LOAD_ANIM_FADE_IN, 100, 100, false);
        break;
    }
    default:
        break;
    }
}

void screen_btn_info_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_CLICKED:
    {
        src_load_page = ABOUT_PAGE;
        Serial.printf("src_load_page:%d\n", src_load_page);
        lv_scr_load_anim(ui.screen_main_about, LV_SCR_LOAD_ANIM_FADE_IN, 100, 100, false);
        break;
    }
    default:
        break;
    }
}

static void shutdown_anim_end(lv_anim_t *anim)
{
    pinMode(BHI260_LDO_ENABLE, OUTPUT);
    digitalWrite(BHI260_LDO_ENABLE, LOW);
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

void screen_btn_wifi_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_PRESSED:
    {
        uint8_t static wifi_status = 1;
        wifi_status ^= 1;
        Serial.printf("wifi_status:%d\n", wifi_status);
        if (wifi_status == 1)
        {
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
            lv_obj_set_style_bg_img_src(ui.screen_set_btn_wifi, &_wifi_60x60, LV_STATE_DEFAULT);
        }
        else
        {
            WiFi.disconnect(true);
            lv_obj_set_style_bg_img_src(ui.screen_set_btn_wifi, &_wifi1_60x60, LV_STATE_DEFAULT);
        }
        break;
    }
    }
}

void screen_btn_ble_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_PRESSED:
    {
        static int ble_status = 0;
        ble_status ^= 1;
        Serial.printf("ble_status:%d\n", ble_status);

        if (ble_status == 1)
        {
            Serial.println("ble open");
            move_mouse = true;
            if (!bleMouse.isConnected())
            {
                bleMouse.begin();
            }
            lv_obj_set_style_bg_img_src(ui.screen_set_btn_ble, &_bleicon_60x60, LV_STATE_DEFAULT);
        }
        else
        {
            bleMouse.end();
            Serial.println("ble colse");
            move_mouse = false;
            lv_obj_set_style_bg_img_src(ui.screen_set_btn_ble, &_ble1_60x60, LV_STATE_DEFAULT);
        }
    }
    }
}

void screen_btn_brightness_event_handler(lv_event_t *e)
{
    static uint8_t brightness = 205;
    static uint8_t brightness_status = 0;
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
    case LV_EVENT_PRESSED:
    {
        brightness_status ^= 1;
        if (brightness_status == 1)
        {
            lv_obj_set_style_bg_img_src(ui.screen_set_btn_brightness, &_brightness_60x60, LV_STATE_DEFAULT);
            if (ui.screen_set_cont_brightness == NULL)
            {
                ui.screen_set_cont_brightness = lv_obj_create(ui.screen_main_set);
                lv_obj_set_pos(ui.screen_set_cont_brightness, 0, 0);
                lv_obj_set_size(ui.screen_set_cont_brightness, 205, 60);
                lv_obj_set_scrollbar_mode(ui.screen_set_cont_brightness, LV_SCROLLBAR_MODE_OFF);
                lv_obj_set_style_pad_all(ui.screen_set_cont_brightness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

                // Write style for screen_walk_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
                lv_obj_set_style_border_width(ui.screen_set_cont_brightness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_radius(ui.screen_set_cont_brightness, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_opa(ui.screen_set_cont_brightness, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(ui.screen_set_cont_brightness, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

                ui.screen_set_slider_brightness = lv_slider_create(ui.screen_set_cont_brightness);
                lv_slider_set_range(ui.screen_set_slider_brightness, 0, 255);
                lv_slider_set_value(ui.screen_set_slider_brightness, brightness, LV_ANIM_OFF);
                lv_obj_set_style_align(ui.screen_set_slider_brightness, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_size(ui.screen_set_slider_brightness, 180, 15);
                // lv_obj_set_pos(ui.screen_set_slider_brightness, 5, 20);
                lv_obj_set_style_bg_color(ui.screen_set_slider_brightness, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(ui.screen_set_slider_brightness, lv_color_hex(0xa6a6a6), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_grad_dir(ui.screen_set_slider_brightness, LV_GRAD_DIR_HOR, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_grad_color(ui.screen_set_slider_brightness, lv_color_hex(0xffffff), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_color(ui.screen_set_slider_brightness, lv_color_hex(0xdec9ad), LV_PART_KNOB | LV_STATE_DEFAULT);
            }
            lv_anim_t anim_down;
            lv_anim_init(&anim_down);
            lv_anim_set_var(&anim_down, ui.screen_set_cont_brightness);
            lv_anim_set_values(&anim_down, -50, 0);
            lv_anim_set_time(&anim_down, 800);
            lv_anim_set_exec_cb(&anim_down, lv_anim_exec_xcb_t(lv_obj_set_y));
            lv_anim_set_path_cb(&anim_down, lv_anim_path_ease_in_out);
            lv_anim_start(&anim_down);

            lv_obj_add_event_cb(ui.screen_set_slider_brightness, [](lv_event_t *e)
                                {
            lv_event_code_t code = lv_event_get_code(e);
            brightness = lv_slider_get_value(ui.screen_set_slider_brightness);
            brightness = 255 - brightness;
            Serial.printf("brightness:%d\n",brightness);
            analogWrite(TFT_BL, brightness); }, LV_EVENT_VALUE_CHANGED, NULL);
        }
        else
        {
            lv_anim_t anim_up;
            lv_anim_init(&anim_up);
            lv_anim_set_var(&anim_up, ui.screen_set_cont_brightness);
            lv_anim_set_values(&anim_up, 0, -100);
            lv_anim_set_time(&anim_up, 800);
            lv_anim_set_exec_cb(&anim_up, lv_anim_exec_xcb_t(lv_obj_set_y));
            lv_anim_set_path_cb(&anim_up, lv_anim_path_ease_in_out);
            lv_anim_start(&anim_up);

            // lv_obj_del(ui.screen_set_cont_brightness);
            lv_obj_set_style_bg_img_src(ui.screen_set_btn_brightness, &_brightness1_60x60, LV_STATE_DEFAULT);
        }
        break;
    }
    }
}

void events_init_screen_blehid(lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_btn_right, screen_btn_right_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui->screen_btn_left, screen_btn_left_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui->screen_btn_touch, screen_btn_touch_event_handler, LV_EVENT_ALL, NULL);
}
