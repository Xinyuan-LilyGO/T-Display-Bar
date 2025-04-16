#include "ui.h"
#include "event.h"

extern lv_ui ui;
extern const char *cityname;
Web_data web_data;
// Web_data last_web_data;
TimerHandle_t timer;
Info_weather info_weather[3];
extern SensorPCF85063 rtc;
extern struct tm timeinfo;
extern Power_info power_info;
extern BQ27220 bq27220;
extern BQ27220::BQ27220_TypeDef data;

extern bool wifi_on;
extern bool wifi_connect_status;
extern bool move_mouse;


static lv_style_t btn_focused_style;
static lv_style_t main_btn_focused_style;
static lv_style_t lable_style;
static lv_style_t btn_default_style;
static lv_style_t touch_focused_style;
static lv_style_t cont_style;
static lv_style_t label_style;

void init_style(void)
{
    lv_style_init(&btn_default_style);
    lv_style_set_border_width(&btn_default_style, 2);
    lv_style_set_radius(&btn_default_style, 10);
    lv_style_set_pad_all(&btn_default_style, 0);
    lv_style_set_bg_opa(&btn_default_style, LV_OPA_COVER);
    lv_style_set_shadow_width(&btn_default_style, 0);
    lv_style_set_bg_color(&btn_default_style, lv_color_hex(0x000000));
    lv_style_set_text_color(&btn_default_style, lv_color_hex(0xffffff));
    lv_style_set_border_color(&btn_default_style, lv_color_hex(0xffffff));
    lv_style_set_text_font(&btn_default_style, &lv_font_OrbitronBlack_20);
    lv_style_set_text_align(&btn_default_style, LV_TEXT_ALIGN_CENTER);

    lv_style_init(&btn_focused_style);
    lv_style_set_border_width(&btn_focused_style, 2);
    lv_style_set_radius(&btn_focused_style, 10);
    lv_style_set_pad_all(&btn_focused_style, 0);
    lv_style_set_bg_opa(&btn_focused_style, LV_OPA_COVER);
    lv_style_set_shadow_width(&btn_focused_style, 0);
    lv_style_set_bg_color(&btn_focused_style, lv_color_hex(0x002fa7));
    lv_style_set_text_color(&btn_focused_style, lv_color_hex(0xffffff));
    lv_style_set_border_color(&btn_focused_style, lv_color_hex(0xffffff));
    lv_style_set_text_font(&btn_focused_style, &lv_font_OrbitronBlack_20);
    lv_style_set_text_align(&btn_focused_style, LV_TEXT_ALIGN_CENTER);

    lv_style_init(&main_btn_focused_style);
    lv_style_set_border_width(&main_btn_focused_style, 2);
    lv_style_set_radius(&main_btn_focused_style, 10);
    lv_style_set_pad_all(&main_btn_focused_style, 0);
    lv_style_set_bg_opa(&main_btn_focused_style, LV_OPA_COVER);
    lv_style_set_shadow_width(&main_btn_focused_style, 0);
    lv_style_set_bg_color(&main_btn_focused_style, lv_color_hex(0x002fa7));
    lv_style_set_text_color(&main_btn_focused_style, lv_color_hex(0xffffff));
    lv_style_set_border_color(&main_btn_focused_style, lv_color_hex(0xff00f0));
    lv_style_set_text_font(&main_btn_focused_style, &lv_font_OrbitronBlack_20);
    lv_style_set_text_align(&main_btn_focused_style, LV_TEXT_ALIGN_CENTER);

    lv_style_init(&touch_focused_style);
    lv_style_set_border_width(&touch_focused_style, 2);
    lv_style_set_radius(&touch_focused_style, 10);
    lv_style_set_pad_all(&touch_focused_style, 0);
    lv_style_set_bg_opa(&touch_focused_style, LV_OPA_COVER);
    lv_style_set_shadow_width(&touch_focused_style, 0);
    lv_style_set_bg_color(&touch_focused_style, lv_color_hex(0xFFEA00));
    lv_style_set_text_color(&touch_focused_style, lv_color_hex(0xffffff));
    lv_style_set_border_color(&touch_focused_style, lv_color_hex(0xffffff));
    lv_style_set_text_font(&touch_focused_style, &lv_font_OrbitronBlack_20);
    lv_style_set_text_align(&touch_focused_style, LV_TEXT_ALIGN_CENTER);

    lv_style_init(&cont_style);
    lv_style_set_border_width(&cont_style, 0);
    lv_style_set_radius(&cont_style, 10);
    lv_style_set_pad_all(&cont_style, 0);
    lv_style_set_bg_opa(&cont_style, LV_OPA_COVER);
    lv_style_set_shadow_width(&cont_style, 0);
    lv_style_set_bg_color(&cont_style, lv_color_hex(0x000000));
    lv_style_set_text_color(&cont_style, lv_color_hex(0xffffff));
    lv_style_set_border_color(&cont_style, lv_color_hex(0xffffff));
    lv_style_set_text_align(&cont_style, LV_TEXT_ALIGN_CENTER);

    lv_style_init(&label_style);
    lv_style_set_border_width(&label_style, 0);
    lv_style_set_radius(&label_style, 0);
    lv_style_set_pad_all(&label_style, 0);
    lv_style_set_bg_opa(&label_style, LV_OPA_COVER);
    lv_style_set_shadow_width(&label_style, 0);
    lv_style_set_text_line_space(&label_style, 0);
    lv_style_set_text_letter_space(&label_style, 2);
    lv_style_set_bg_color(&label_style, lv_color_hex(0x000000));
    lv_style_set_text_color(&label_style, lv_color_hex(0xffffff));
}

void ui_main_init(lv_ui *ui)
{
    ui->screen_main = lv_obj_create(NULL);
    lv_obj_set_pos(ui->screen_main, 0, 0);
    lv_obj_set_size(ui->screen_main, LV_SCREEN_WIDTH, LV_SCREEN_HEIGHT);
    lv_obj_set_scrollbar_mode(ui->screen_main, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(ui->screen_main, lv_color_hex(0xe72074), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_main, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->screen_main, lv_color_hex(0xec00ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->screen_main, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen_main, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_cont_info = lv_obj_create(ui->screen_main);
    lv_obj_set_size(ui->screen_cont_info, 60, 70);
    lv_obj_set_pos(ui->screen_cont_info, 3, 8);
    lv_obj_set_scrollbar_mode(ui->screen_cont_info, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(ui->screen_cont_info, &cont_style, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_label_time = lv_label_create(ui->screen_cont_info);
    // lv_label_set_text(ui->screen_label_time, "11:58");
    lv_label_set_text_fmt(ui->screen_label_time, "%02d:%02d", web_data.time.hour, web_data.time.minute);
    lv_label_set_long_mode(ui->screen_label_time, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_time, 0, 5);
    lv_obj_set_size(ui->screen_label_time, 60, 20);
    lv_obj_set_style_bg_opa(ui->screen_label_time, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_time, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_time, &lv_font_OrbitronBlack_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_label_time, &label_style, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_cont_icon = lv_obj_create(ui->screen_cont_info);
    lv_obj_set_size(ui->screen_cont_icon, 50, 45);
    lv_obj_set_pos(ui->screen_cont_icon, 5, 20);
    lv_obj_set_scrollbar_mode(ui->screen_cont_icon, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(ui->screen_cont_icon, &cont_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_icon, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_icon, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_icon, LV_OPA_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_img_wifi_icon
    ui->screen_img_wifi_icon = lv_img_create(ui->screen_cont_icon);
    lv_obj_add_flag(ui->screen_img_wifi_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_wifi_icon, &_wifi_alpha_22x22);
    lv_img_set_pivot(ui->screen_img_wifi_icon, 0, 0);
    lv_img_set_angle(ui->screen_img_wifi_icon, 0);
    lv_obj_set_pos(ui->screen_img_wifi_icon, 2, 0);
    lv_obj_set_size(ui->screen_img_wifi_icon, 22, 22);

    // Write style for screen_img_wifi_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT->
    lv_obj_set_style_img_recolor_opa(ui->screen_img_wifi_icon, 193, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_img_wifi_icon, lv_color_hex(0x15ff15), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_wifi_icon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_wifi_icon, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_wifi_icon, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_img_ble_icon
    ui->screen_img_ble_icon = lv_img_create(ui->screen_cont_icon);
    lv_obj_add_flag(ui->screen_img_ble_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_ble_icon, &_ble_alpha_16x16);
    lv_img_set_pivot(ui->screen_img_ble_icon, 0, 0);
    lv_img_set_angle(ui->screen_img_ble_icon, 0);
    lv_obj_set_pos(ui->screen_img_ble_icon, 28, 3);
    lv_obj_set_size(ui->screen_img_ble_icon, 16, 16);

    // Write style for screen_img_ble_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT->
    lv_obj_set_style_img_recolor_opa(ui->screen_img_ble_icon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_img_ble_icon, lv_color_hex(0x0055ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_ble_icon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_ble_icon, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_ble_icon, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_bettery1
    ui->screen_cont_bettery1 = lv_obj_create(ui->screen_cont_icon);
    lv_obj_set_pos(ui->screen_cont_bettery1, 41, 29);
    lv_obj_set_size(ui->screen_cont_bettery1, 2, 6);
    lv_obj_set_scrollbar_mode(ui->screen_cont_bettery1, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_cont_bettery1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_bettery1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_bettery1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_bettery1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_bettery1, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_bettery1, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_bettery1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_bettery1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_bettery1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_bettery1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_bettery1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_cont_bettery0
    ui->screen_cont_bettery0 = lv_obj_create(ui->screen_cont_icon);
    lv_obj_set_pos(ui->screen_cont_bettery0, 5, 23);
    lv_obj_set_size(ui->screen_cont_bettery0, 36, 18);
    lv_obj_set_scrollbar_mode(ui->screen_cont_bettery0, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_cont_bettery0, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_bettery0, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_cont_bettery0, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_cont_bettery0, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_cont_bettery0, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_bettery0, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_bettery0, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_bettery0, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_bettery0, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_bettery0, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_bettery0, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_bettery0, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_cont_bettery2
    ui->screen_cont_bettery2 = lv_obj_create(ui->screen_cont_bettery0);
    lv_obj_set_pos(ui->screen_cont_bettery2, 2, 2);
    lv_obj_set_size(ui->screen_cont_bettery2, 28, 10); // 100/25= 4 steps

    lv_obj_set_scrollbar_mode(ui->screen_cont_bettery2, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_cont_bettery2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_bettery2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_bettery2, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_bettery2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_bettery2, lv_color_hex(0x02eb10), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_bettery2, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_bettery2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_bettery2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_bettery2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_bettery2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_bettery2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_label_app_bg = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_label_app_bg, "");
    lv_label_set_long_mode(ui->screen_label_app_bg, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_app_bg, 65, 8);
    lv_obj_set_size(ui->screen_label_app_bg, 216, 70);
    lv_obj_set_style_border_width(ui->screen_label_app_bg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_app_bg, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_app_bg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_label_app_bg, lv_color_hex(0x7b009c), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_label_app_bg, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->screen_label_app_bg, lv_color_hex(0x01dec3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->screen_label_app_bg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen_label_app_bg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_mian_cont_app
    ui->screen_cont_app = lv_obj_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_cont_app, 70, 12);
    lv_obj_set_size(ui->screen_cont_app, 206, 62);
    lv_obj_set_scrollbar_mode(ui->screen_cont_app, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_cont_app, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_app, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_app, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_app, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_app, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_img_weather
    ui->screen_img_weather = lv_img_create(ui->screen_cont_app);
    lv_obj_add_flag(ui->screen_img_weather, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_weather, &_weather_alpha_30x30);
    lv_img_set_pivot(ui->screen_img_weather, 50, 50);
    lv_img_set_angle(ui->screen_img_weather, 0);
    lv_obj_set_pos(ui->screen_img_weather, 19, 16);
    lv_obj_set_size(ui->screen_img_weather, 30, 30);

    // Write style for screen_img_weather, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT->
    lv_obj_set_style_img_recolor_opa(ui->screen_img_weather, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_weather, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_weather, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_weather, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_img_mouse
    ui->screen_img_mouse = lv_img_create(ui->screen_cont_app);
    lv_obj_add_flag(ui->screen_img_mouse, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_mouse, &_mouse_alpha_30x30);
    lv_img_set_pivot(ui->screen_img_mouse, 0, 0);
    lv_img_set_angle(ui->screen_img_mouse, 0);
    lv_obj_set_pos(ui->screen_img_mouse, 87, 16);
    lv_obj_set_size(ui->screen_img_mouse, 30, 30);

    // Write style for screen_img_mouse, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT->
    lv_obj_set_style_img_recolor_opa(ui->screen_img_mouse, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_mouse, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_mouse, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_mouse, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_img_set
    ui->screen_img_set = lv_img_create(ui->screen_cont_app);
    lv_obj_add_flag(ui->screen_img_set, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_set, &_info_circle_alpha_30x30);
    lv_img_set_pivot(ui->screen_img_set, 50, 50);
    lv_img_set_angle(ui->screen_img_set, 0);
    lv_obj_set_pos(ui->screen_img_set, 156, 16);
    lv_obj_set_size(ui->screen_img_set, 30, 30);

    // Write style for screen_img_set, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->screen_img_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_set, true, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui->screen_img_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_set, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_btn_weather
    ui->screen_btn_weather = lv_btn_create(ui->screen_cont_app);
    ui->screen_btn_weather_label = lv_label_create(ui->screen_btn_weather);
    lv_label_set_text(ui->screen_btn_weather_label, "");
    lv_label_set_long_mode(ui->screen_btn_weather_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_btn_weather_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_btn_weather, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_btn_weather, 8, 6);
    lv_obj_set_size(ui->screen_btn_weather, 50, 50);

    // Write style for screen_btn_weather, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT->
    lv_obj_set_style_bg_opa(ui->screen_btn_weather, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_btn_weather, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_btn_weather, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_btn_weather, lv_color_hex(0xff00f0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_btn_weather, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_btn_weather, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_btn_weather, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // lv_obj_set_style_bg_img_src(ui->screen_btn_weather, &_weather_alpha_35x35, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_add_style(ui->screen_btn_weather, &main_btn_focused_style, LV_PART_MAIN | LV_STATE_PRESSED);

    // Write codes screen_btn_mouse
    ui->screen_btn_mouse = lv_btn_create(ui->screen_cont_app);
    ui->screen_btn_mouse_label = lv_label_create(ui->screen_btn_mouse);
    lv_label_set_text(ui->screen_btn_mouse_label, "");
    lv_label_set_long_mode(ui->screen_btn_mouse_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_btn_mouse_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_btn_mouse, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_btn_mouse, 77, 6);
    lv_obj_set_size(ui->screen_btn_mouse, 50, 50);

    // Write style for screen_btn_mouse, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT->
    lv_obj_set_style_bg_opa(ui->screen_btn_mouse, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_btn_mouse, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_btn_mouse, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_btn_mouse, lv_color_hex(0xff00f0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_btn_mouse, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_btn_mouse, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_btn_mouse, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_btn_mouse, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    // lv_obj_set_style_bg_img_src(ui->screen_btn_mouse, &_mouse_alpha_35x35, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_add_style(ui->screen_btn_mouse, &main_btn_focused_style, LV_PART_MAIN | LV_STATE_PRESSED);

    // Write codes screen_mian_btn_set
    ui->screen_btn_set = lv_btn_create(ui->screen_cont_app);
    ui->screen_btn_set_label = lv_label_create(ui->screen_btn_set);
    lv_label_set_text(ui->screen_btn_set_label, "");
    lv_label_set_long_mode(ui->screen_btn_set_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_btn_set_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_btn_set, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_btn_set, 146, 6);
    lv_obj_set_size(ui->screen_btn_set, 50, 50);

    // Write style for screen_btn_set, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->screen_btn_set, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_btn_set, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_btn_set, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_btn_set, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_btn_set, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_btn_set, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_btn_set, lv_color_hex(0xff00f0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_btn_set, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_btn_set, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // lv_obj_set_style_bg_img_src(ui->screen_btn_set, &_shezhi_alpha_35x35, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_add_style(ui->screen_btn_set, &main_btn_focused_style, LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_update_snap(ui->screen_main, LV_ANIM_ON);
    lv_obj_update_layout(ui->screen_main);

    // Init events for screen.
    events_init_screen_mian(ui);
}

void ui_weather_init(lv_ui *ui)
{
    ui->screen_main_weather = lv_obj_create(NULL);
    lv_obj_set_pos(ui->screen_main_weather, 0, 0);
    lv_obj_set_size(ui->screen_main_weather, LV_SCREEN_WIDTH, LV_SCREEN_HEIGHT);
    lv_obj_set_scrollbar_mode(ui->screen_main_weather, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(ui->screen_main_weather, lv_color_hex(0x00ffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_src(ui->screen_main_weather, &_werther_bg2_284x76, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_weather_img_location = lv_img_create(ui->screen_main_weather);
    lv_img_set_src(ui->screen_weather_img_location, &_location_fill_alpha_12x12);
    lv_obj_set_pos(ui->screen_weather_img_location, 3, 8);
    lv_obj_set_size(ui->screen_weather_img_location, 12, 12);
    lv_obj_set_style_img_recolor_opa(ui->screen_weather_img_location, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_weather_img_location, lv_color_hex(0xff6500), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_weather_img_location, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_weather_location = lv_label_create(ui->screen_main_weather);
    lv_label_set_text_fmt(ui->screen_weather_location, "%s", cityname);
    lv_obj_set_style_text_font(ui->screen_weather_location, &lv_font_Acme_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_weather_location, 18, 8);
    lv_obj_set_size(ui->screen_weather_location, 120, 15);
    lv_obj_set_style_text_align(ui->screen_weather_location, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_location, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_weather_datatime = lv_label_create(ui->screen_main_weather);
    lv_label_set_text(ui->screen_weather_datatime, "2025-01-01 \n12:00:00"); // test
    lv_obj_set_pos(ui->screen_weather_datatime, 160, 35);
    lv_obj_set_size(ui->screen_weather_datatime, 90, 35);
    lv_obj_set_style_text_align(ui->screen_weather_datatime, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_datatime, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_weather_datatime, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_datatime, &lv_font_Acme_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_weather_temp = lv_label_create(ui->screen_main_weather);
    lv_label_set_text(ui->screen_weather_temp, "0.0°C");
    lv_obj_set_pos(ui->screen_weather_temp, 160, 5);
    lv_obj_set_size(ui->screen_weather_temp, 120, 25);
    lv_obj_set_style_text_align(ui->screen_weather_temp, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_temp, &lv_font_OrbitronBlack_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_weather_img_weather[0] = lv_img_create(ui->screen_main_weather);
    lv_img_set_src(ui->screen_weather_img_weather[0], &_sun_50x50);
    lv_obj_set_pos(ui->screen_weather_img_weather[0], 230, 28);
    lv_obj_set_size(ui->screen_weather_img_weather[0], 50, 50);
    // lv_obj_add_state(ui->screen_weather_img_weather[0],LV_OBJ_FLAG_HIDDEN);

    ui->screen_weather_img_weather[1] = lv_img_create(ui->screen_main_weather);
    lv_img_set_src(ui->screen_weather_img_weather[1], &_suncloud_alpha_50x50);
    lv_obj_set_pos(ui->screen_weather_img_weather[1], 230, 28);
    lv_obj_set_size(ui->screen_weather_img_weather[1], 50, 50);
    lv_obj_add_flag(ui->screen_weather_img_weather[1], LV_OBJ_FLAG_HIDDEN);

    ui->screen_weather_img_weather[2] = lv_img_create(ui->screen_main_weather);
    lv_img_set_src(ui->screen_weather_img_weather[2], &_rain_50x50);
    lv_obj_set_pos(ui->screen_weather_img_weather[2], 230, 28);
    lv_obj_set_size(ui->screen_weather_img_weather[2], 50, 50);
    lv_obj_add_flag(ui->screen_weather_img_weather[2], LV_OBJ_FLAG_HIDDEN);

    ui->screen_weather_img_weather[3] = lv_img_create(ui->screen_main_weather);
    lv_img_set_src(ui->screen_weather_img_weather[3], &_snow_50x50);
    lv_obj_set_pos(ui->screen_weather_img_weather[3], 230, 28);
    lv_obj_set_size(ui->screen_weather_img_weather[3], 50, 50);
    lv_obj_add_flag(ui->screen_weather_img_weather[3], LV_OBJ_FLAG_HIDDEN);

    info_weather[0] = {
        .x = 5,
        .y = 25,
        .size_x = 45,
        .size_y = 50,
        .weather_icon = &_humidity_alpha_20x20,
    };

    // Write codes screen_weather_cont_humidity
    ui->screen_weather_cont_humidity = lv_obj_create(ui->screen_main_weather);
    lv_obj_set_pos(ui->screen_weather_cont_humidity, info_weather[0].x, info_weather[0].y);
    lv_obj_set_size(ui->screen_weather_cont_humidity, info_weather[0].size_x, info_weather[0].size_y);
    lv_obj_set_scrollbar_mode(ui->screen_weather_cont_humidity, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_weather_cont_humidity, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_cont_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_cont_humidity, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_cont_humidity, 82, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_cont_humidity, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_cont_humidity, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->screen_weather_cont_humidity, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->screen_weather_cont_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen_weather_cont_humidity, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_cont_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_cont_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_cont_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_cont_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_cont_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_weather_humidity_num
    ui->screen_weather_humidity_num = lv_label_create(ui->screen_weather_cont_humidity);
    lv_label_set_text(ui->screen_weather_humidity_num, "0%");
    lv_label_set_long_mode(ui->screen_weather_humidity_num, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_weather_humidity_num, 0, 3);
    lv_obj_set_size(ui->screen_weather_humidity_num, info_weather[0].size_x, 15);

    // Write style for screen_weather_humidity_num, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_humidity_num, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_humidity_num, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_weather_humidity_num, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_humidity_num, &lv_font_Alatsi_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_weather_humidity_num, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_humidity_num, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_weather_humidity_num, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_weather_humidity_num, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_weather_img_humidity = lv_img_create(ui->screen_weather_cont_humidity);
    lv_obj_set_pos(ui->screen_weather_img_humidity, 12, 24);
    lv_obj_set_size(ui->screen_weather_img_humidity, 20, 20);
    lv_img_set_src(ui->screen_weather_img_humidity, info_weather[0].weather_icon);
    lv_img_set_pivot(ui->screen_weather_img_humidity, 50, 50);
    lv_img_set_angle(ui->screen_weather_img_humidity, 0);
    lv_obj_set_style_img_recolor_opa(ui->screen_weather_img_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_weather_img_humidity, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_img_humidity, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_weather_img_humidity, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    info_weather[1] = {
        .x = 57,
        .y = 25,
        .size_x = 45,
        .size_y = 50,
        .weather_icon = &_Wind_alpha_20x20,
    };

    // Write codes screen_weather_cont_wind
    ui->screen_weather_cont_wind = lv_obj_create(ui->screen_main_weather);
    lv_obj_set_pos(ui->screen_weather_cont_wind, info_weather[1].x, info_weather[1].y);
    lv_obj_set_size(ui->screen_weather_cont_wind, info_weather[1].size_x, info_weather[1].size_y);
    lv_obj_set_scrollbar_mode(ui->screen_weather_cont_wind, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_weather_cont_wind, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_cont_wind, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_cont_wind, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_cont_wind, 82, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_cont_wind, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_cont_wind, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->screen_weather_cont_wind, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->screen_weather_cont_wind, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen_weather_cont_wind, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_cont_wind, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_cont_wind, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_cont_wind, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_cont_wind, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_cont_wind, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_weather_wind_num
    ui->screen_weather_wind_num = lv_label_create(ui->screen_weather_cont_wind);
    lv_label_set_text(ui->screen_weather_wind_num, "0M/S");
    lv_label_set_long_mode(ui->screen_weather_wind_num, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_weather_wind_num, 0, 3);
    lv_obj_set_size(ui->screen_weather_wind_num, info_weather[1].size_x, 15);

    // Write style for screen_weather_wind_num, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_wind_num, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_wind_num, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_weather_wind_num, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_wind_num, &lv_font_Alatsi_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_weather_wind_num, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_wind_num, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_weather_wind_num, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_weather_wind_num, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_weather_img_wind = lv_img_create(ui->screen_weather_cont_wind);
    lv_obj_set_pos(ui->screen_weather_img_wind, 12, 24);
    lv_obj_set_size(ui->screen_weather_img_wind, 20, 20);
    lv_img_set_src(ui->screen_weather_img_wind, info_weather[1].weather_icon);
    lv_img_set_pivot(ui->screen_weather_img_wind, 50, 50);
    lv_img_set_angle(ui->screen_weather_img_wind, 0);
    lv_obj_set_style_img_recolor_opa(ui->screen_weather_img_wind, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_weather_img_wind, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_img_wind, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_weather_img_wind, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    info_weather[2] = {
        .x = 110,
        .y = 25,
        .size_x = 45,
        .size_y = 50,
        .weather_icon = &_pressure_alpha_20x20,
    };

    // Write codes screen_weather_cont_pressure
    ui->screen_weather_cont_pressure = lv_obj_create(ui->screen_main_weather);
    lv_obj_set_pos(ui->screen_weather_cont_pressure, info_weather[2].x, info_weather[2].y);
    lv_obj_set_size(ui->screen_weather_cont_pressure, info_weather[2].size_x, info_weather[2].size_y);
    lv_obj_set_scrollbar_mode(ui->screen_weather_cont_pressure, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_weather_cont_pressure, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_cont_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_cont_pressure, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_weather_cont_pressure, 82, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_weather_cont_pressure, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_weather_cont_pressure, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->screen_weather_cont_pressure, lv_color_hex(0x373737), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->screen_weather_cont_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen_weather_cont_pressure, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_weather_cont_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_weather_cont_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_weather_cont_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_weather_cont_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_weather_cont_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_weather_pressure_num
    ui->screen_weather_pressure_num = lv_label_create(ui->screen_weather_cont_pressure);
    lv_label_set_text(ui->screen_weather_pressure_num, "1.00Kpa");
    lv_label_set_long_mode(ui->screen_weather_pressure_num, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_weather_pressure_num, 0, 3);
    lv_obj_set_size(ui->screen_weather_pressure_num, info_weather[2].size_x, 15);

    // Write style for screen_weather_pressure_num, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_weather_pressure_num, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_pressure_num, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_weather_pressure_num, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_pressure_num, &lv_font_Alatsi_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_weather_pressure_num, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_pressure_num, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_weather_pressure_num, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_weather_pressure_num, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_weather_img_pressure = lv_img_create(ui->screen_weather_cont_pressure);
    lv_obj_set_pos(ui->screen_weather_img_pressure, 12, 24);
    lv_obj_set_size(ui->screen_weather_img_pressure, 20, 20);
    lv_img_set_src(ui->screen_weather_img_pressure, info_weather[2].weather_icon);
    lv_img_set_pivot(ui->screen_weather_img_pressure, 50, 50);
    lv_img_set_angle(ui->screen_weather_img_pressure, 0);
    lv_obj_set_style_img_recolor_opa(ui->screen_weather_img_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_weather_img_pressure, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_weather_img_pressure, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_weather_img_pressure, true, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void ui_air_mouse(lv_ui *ui)
{
    ui->screen_main_blehid = lv_obj_create(NULL);
    // lv_obj_set_pos(ui->screen_main_blehid, 0, 0);
    lv_obj_set_size(ui->screen_main_blehid, LV_SCREEN_WIDTH, LV_SCREEN_HEIGHT);
    lv_obj_set_scrollbar_mode(ui->screen_main_blehid, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(ui->screen_main_blehid, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_btn_right
    ui->screen_btn_right = lv_btn_create(ui->screen_main_blehid);
    lv_obj_set_pos(ui->screen_btn_right, 0, 5);
    lv_obj_set_size(ui->screen_btn_right, 124, 36);
    ui->screen_btn_right_label = lv_label_create(ui->screen_btn_right);
    lv_label_set_text(ui->screen_btn_right_label, "RIGHT");
    lv_label_set_long_mode(ui->screen_btn_right_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_btn_right_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_width(ui->screen_btn_right_label, LV_PCT(100));
    lv_obj_add_style(ui->screen_btn_right, &btn_default_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_btn_right, &lv_font_OrbitronBlack_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_btn_right, &btn_focused_style, LV_PART_MAIN | LV_STATE_PRESSED);

    ui->screen_btn_left = lv_btn_create(ui->screen_main_blehid);
    ui->screen_btn_left_label = lv_label_create(ui->screen_btn_left);
    lv_label_set_text(ui->screen_btn_left_label, "LEFT");
    lv_label_set_long_mode(ui->screen_btn_left_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_btn_left_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_btn_left, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_btn_left_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_btn_left, 0, 45);
    lv_obj_set_size(ui->screen_btn_left, 124, 36);
    lv_obj_add_style(ui->screen_btn_left, &btn_default_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_btn_left, &lv_font_OrbitronBlack_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_btn_left, &btn_focused_style, LV_PART_MAIN | LV_STATE_PRESSED);

    // Write codes screen_btn_touch
    ui->screen_btn_touch = lv_btn_create(ui->screen_main_blehid);
    ui->screen_btn_touch_label = lv_label_create(ui->screen_btn_touch);
    lv_label_set_text(ui->screen_btn_touch_label, "BACK\nTOUCH");
    lv_label_set_long_mode(ui->screen_btn_touch_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_btn_touch_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_btn_touch, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->screen_btn_touch_label, LV_PCT(100));
    lv_obj_set_pos(ui->screen_btn_touch, 130, 5);
    lv_obj_set_size(ui->screen_btn_touch, 154, 76);
    lv_obj_add_style(ui->screen_btn_touch, &btn_default_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_btn_touch, &touch_focused_style, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_btn_touch, &lv_font_OrbitronBlack_22, LV_PART_MAIN | LV_STATE_DEFAULT);

    events_init_screen_blehid(ui);
}

static void cont_event_cb(lv_event_t *e)
{
    lv_obj_t *list = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    switch (code)
    {
    case LV_EVENT_SCROLL: // 实时滚动事件
        Serial.println("LV_EVENT_SCROLL");
        // check_selected_item(list);
        break;

    case LV_EVENT_SCROLL_END: // 滚动结束
        Serial.println("Scroll end");
        // scroll_end_anim(list);
        break;

    case LV_EVENT_STYLE_CHANGED: // 样式变化时刷新
        lv_obj_scroll_to_view(lv_obj_get_child(list, 0), LV_ANIM_OFF);
        break;
    }
}

void ui_about_init(lv_ui *ui)
{
    ui->screen_main_about = lv_obj_create(NULL);
    lv_obj_set_pos(ui->screen_main_about, 0, 0);
    lv_obj_set_size(ui->screen_main_about, LV_SCREEN_WIDTH, LV_SCREEN_HEIGHT);
    lv_obj_set_scrollbar_mode(ui->screen_main_about, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_opa(ui->screen_main_about, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_main_about, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_cont_about
    ui->screen_about_cont_about = lv_obj_create(ui->screen_main_about);
    lv_obj_set_pos(ui->screen_about_cont_about, 0, 0);
    lv_obj_set_size(ui->screen_about_cont_about, LV_SCREEN_WIDTH, LV_SCREEN_HEIGHT);
    lv_obj_set_scrollbar_mode(ui->screen_about_cont_about, LV_SCROLLBAR_MODE_AUTO);
    // lv_obj_add_event_cb(ui->screen_about_cont_about,cont_event_cb,LV_EVENT_ALL,NULL);

    // Write style for screen_about_cont_about, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_about_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_about_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_about_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_about_cont_about, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_about_cont_about, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_about_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_about_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_about_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_about_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_about_cont_about, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_set_label_mcu
    ui->screen_about_label_mcu = lv_label_create(ui->screen_about_cont_about);
    about_info_label(ui->screen_about_label_mcu, "MCU: ESP32S3", 10, 15);
    ui->screen_about_label_wifirssi = lv_label_create(ui->screen_about_cont_about);
    about_info_label(ui->screen_about_label_wifirssi, "WiFi Rssi: -0dBm", 10, 35);
    ui->screen_about_label_Battery_level = lv_label_create(ui->screen_about_cont_about);
    about_info_label(ui->screen_about_label_Battery_level, "Battery: 0%", 10, 55);
    ui->screen_about_label_vbus = lv_label_create(ui->screen_about_cont_about);
    about_info_label(ui->screen_about_label_vbus, "VBus: 0V", 10, 75);
    ui->screen_about_label_vbatt = lv_label_create(ui->screen_about_cont_about);
    about_info_label(ui->screen_about_label_vbatt, "VBatt: 0V", 10, 95);
    ui->screen_about_label_vsys = lv_label_create(ui->screen_about_cont_about);
    about_info_label(ui->screen_about_label_vsys, "VSys: 0V", 10, 115);
    ui->screen_about_label_vtarget = lv_label_create(ui->screen_about_cont_about);
    about_info_label(ui->screen_about_label_vtarget, "VTarget: 0V", 10, 135);
    ui->screen_about_label_chargestatus = lv_label_create(ui->screen_about_cont_about);
    about_info_label(ui->screen_about_label_chargestatus, "CHG STATE: NULL", 10, 155);
}

void about_info_label(lv_obj_t *param, String info, int x, int y)
{
    lv_label_set_text(param, info.c_str());
    lv_label_set_long_mode(param, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(param, x, y);
    lv_obj_set_size(param, 274, 15);

    // Write style for screen_about_label_mcu, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(param, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(param, &lv_font_Orbitron_VariableFont_wght_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(param, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(param, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(param, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(param, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void ui_timer_cb(lv_timer_t *timer)
{
    // Serial.println("ui_timer_cb");
    uint16_t static weather_count = 0;
    weather_count++;
    uint16_t static about_count = 0;
    about_count++;
    web_data.time.second++;

    switch (src_load_page)
    {
    case 0: //mian page
    {
        if (web_data.time.second > 59)
        {
            web_data.time.second = 0;
            web_data.time.minute++;
            if (web_data.time.minute > 59)
            {
                web_data.time.minute = 0;
                web_data.time.hour++;
            }
            if (web_data.time.hour > 23)
            {
                web_data.time.hour = 0;
                rtc.getDateTime(&timeinfo);
            }
            lv_label_set_text_fmt(ui.screen_label_time, "%02d:%02d", web_data.time.hour, web_data.time.minute);
        }
        lv_obj_set_size(ui.screen_cont_bettery2, ((uint8_t)power_info.Percentage * 28 / 100), 10);

        if (power_info.Percentage <= 20)
        {
            lv_obj_set_style_bg_color(ui.screen_cont_bettery2, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else
        {
            lv_obj_set_style_bg_color(ui.screen_cont_bettery2, lv_color_hex(0x02eb10), LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        if (wifi_on && wifi_connect_status)
        {
            lv_obj_set_style_img_recolor(ui.screen_img_wifi_icon, lv_color_hex(0x15ff15), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else
        {
            lv_obj_set_style_img_recolor(ui.screen_img_wifi_icon, lv_color_hex(0xd3d3d3), LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        if (move_mouse && bleMouse.isConnected())
        {
            lv_obj_set_style_img_recolor(ui.screen_img_ble_icon, lv_color_hex(0x0055ff), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else
        {
            lv_obj_set_style_img_recolor(ui.screen_img_ble_icon, lv_color_hex(0xd3d3d3), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        break;
    }
    case 1: //weather page
    {
        if(web_data.time.second > 59)
        {
            web_data.time.second = 0;
            web_data.time.minute++;
            if (web_data.time.minute > 59)
            {
                web_data.time.minute = 0;
                web_data.time.hour++;
            }
            if (web_data.time.hour > 23)
            {
                web_data.time.hour = 0;
                rtc.getDateTime(&timeinfo);
            }
        }
        lv_label_set_text_fmt(ui.screen_weather_datatime, "%04d-%02d-%02d \n%02d:%02d:%02d",
                              web_data.time.year, web_data.time.month, web_data.time.day,
                              web_data.time.hour, web_data.time.minute, web_data.time.second);

        if (weather_count >= 5)
        {
            lv_label_set_text_fmt(ui.screen_weather_temp, "%.1f°C", web_data.weather.temperature);
            lv_label_set_text_fmt(ui.screen_weather_humidity_num, "%d%%", web_data.weather.humidity);
            lv_label_set_text_fmt(ui.screen_weather_wind_num, "%dM/S", web_data.weather.wind_speed);
            lv_label_set_text_fmt(ui.screen_weather_pressure_num, "%.2fKpa", (float)web_data.weather.pressure * 0.001);
        }

        if (weather_count >= WEATHER_UPDATE_TIME * 60)
        {
            static const char *last_werther = "null";
            if (strcmp(web_data.weather.weather, WEATHER_CLEAR) == 0 && last_werther != web_data.weather.weather)
            {
                Serial.println("clear");
                last_werther = WEATHER_CLEAR;
                lv_obj_clear_flag(ui.screen_weather_img_weather[0], LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui.screen_weather_img_weather[1], LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui.screen_weather_img_weather[2], LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui.screen_weather_img_weather[3], LV_OBJ_FLAG_HIDDEN);
            }
            else if (strcmp(web_data.weather.weather, WEATHER_CLOUDS) == 0 && last_werther != web_data.weather.weather)
            {
                Serial.println("clouds");
                last_werther = WEATHER_CLOUDS;
                lv_obj_add_flag(ui.screen_weather_img_weather[0], LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui.screen_weather_img_weather[1], LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui.screen_weather_img_weather[2], LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui.screen_weather_img_weather[3], LV_OBJ_FLAG_HIDDEN);
            }
            else if (strcmp(web_data.weather.weather, WEATHER_RAIN) == 0 && last_werther != web_data.weather.weather)
            {
                Serial.println("rain");
                last_werther = WEATHER_RAIN;
                lv_obj_add_flag(ui.screen_weather_img_weather[0], LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui.screen_weather_img_weather[1], LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui.screen_weather_img_weather[2], LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui.screen_weather_img_weather[3], LV_OBJ_FLAG_HIDDEN);
            }
            else if (strcmp(web_data.weather.weather, WEATHER_SNOW) == 0 && last_werther != web_data.weather.weather)
            {
                Serial.println("snow");
                last_werther = WEATHER_SNOW;
                lv_obj_add_flag(ui.screen_weather_img_weather[0], LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui.screen_weather_img_weather[1], LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui.screen_weather_img_weather[2], LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui.screen_weather_img_weather[3], LV_OBJ_FLAG_HIDDEN);
            }
            weather_count = 0;
        }
        break;
    }
    case 2: //air mouse page
        break;
    case 3: //about page
    {
        if (about_count >= 3)
        {
            lv_label_set_text_fmt(ui.screen_about_label_wifirssi, "WiFi Rssi: %ddBm", power_info.Rssi);
            lv_label_set_text_fmt(ui.screen_about_label_vbatt, "VBatt: %04dmV", power_info.VBattVoltage);
            lv_label_set_text_fmt(ui.screen_about_label_Battery_level, "Battery: %.2f%%", power_info.Percentage);
            lv_label_set_text_fmt(ui.screen_about_label_vbus, "VBus: %04dmV", power_info.VBusVoltage);
            lv_label_set_text_fmt(ui.screen_about_label_vsys, "VSys: %04dmV", power_info.VSysVoltage);
            lv_label_set_text_fmt(ui.screen_about_label_vtarget, "VTarget: %04dmV", power_info.ChargeTargetVoltage);
            lv_label_set_text_fmt(ui.screen_about_label_chargestatus, "CHG State: %s", power_info.ChargeStatus);
            about_count = 0;
        }
        break;
    }
    default:
        break;
    }
}

void enter_ui(void)
{
    Serial.println("enter_ui");
    init_style();

    ui_main_init(&ui);
    ui_weather_init(&ui);
    ui_air_mouse(&ui);
    ui_about_init(&ui);

    lv_timer_t *timer = lv_timer_create(ui_timer_cb, 1000, NULL);
    lv_timer_ready(timer);
    lv_scr_load(ui.screen_main);
    // lv_scr_load(ui.screen_main_about);
}