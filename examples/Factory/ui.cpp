#include "event.h"

extern const char *cityname;
Web_data web_data;
TimerHandle_t timer;
Info_weather info_weather[3];
std::stringstream about_text;

extern SensorPCF85063 rtc;
extern struct tm timeinfo;
extern About_info about_info;

static lv_style_t btn_focused_style;
static lv_style_t main_btn_focused_style;
static lv_style_t lable_style;
static lv_style_t btn_default_style;
static lv_style_t touch_focused_style;
static lv_style_t cont_style;
static lv_style_t label_style;

typedef void (*lv_event_cb_t)(lv_event_t *event);
struct APP_ICON_INFO
{
    lv_obj_t *app_btn;
    lv_obj_t *app_label;
    lv_img_dsc_t btn_img;
    lv_obj_t *btn_img_obj;
    lv_coord_t x_pos;
    lv_coord_t y_pos;
    lv_coord_t width;
    lv_coord_t height;
    lv_event_cb_t event_cb;
    uint8_t app_id;
};

#define APP_ICON_NUM 7
APP_ICON_INFO app_icon_info[APP_ICON_NUM] = {
    {.app_btn = ui.screen_btn_weather, .btn_img = _weather_alpha_30x30, .btn_img_obj = ui.screen_img_weather, .x_pos = 10, .y_pos = 6, .width = 50, .height = 50, .event_cb = screen_btn_weather_event_handler, .app_id = 0},
    {.app_btn = ui.screen_btn_mouse, .btn_img = _mouse_alpha_30x30, .btn_img_obj = ui.screen_img_mouse, .x_pos = 80, .y_pos = 6, .width = 50, .height = 50, .event_cb = screen_btn_mouse_event_handler, .app_id = 1},
    {.app_btn = ui.screen_btn_sd, .btn_img = _wenjian1_alpha_30x30, .btn_img_obj = ui.screen_img_sd, .x_pos = 150, .y_pos = 6, .width = 50, .height = 50, .event_cb = screen_btn_sd_event_handler, .app_id = 2},
    {.app_btn = ui.screen_btn_bhi260, .btn_img = _gyroscope_alpha_35x35, .btn_img_obj = ui.screen_img_bhi260, .x_pos = 220, .y_pos = 6, .width = 50, .height = 50, .event_cb = screen_btn_bhi260_event_handler, .app_id = 3},
    {.app_btn = ui.screen_btn_set, .btn_img = _shezhi_alpha_30x30, .btn_img_obj = ui.screen_img_set, .x_pos = 290, .y_pos = 6, .width = 50, .height = 50, .event_cb = screen_btn_set_event_handler, .app_id = 4},
    {.app_btn = ui.screen_btn_info, .btn_img = _info_circle_alpha_30x30, .btn_img_obj = ui.screen_img_info, .x_pos = 360, .y_pos = 6, .width = 50, .height = 50, .event_cb = screen_btn_info_event_handler, .app_id = 5},
    {.app_btn = ui.screen_btn_shotdown, .btn_img = _shutdown_alpha_30x30, .btn_img_obj = ui.screen_img_shotdown, .x_pos = 430, .y_pos = 6, .width = 50, .height = 50, .event_cb = screen_btn_shutdown_event_handler, .app_id = 6},
};

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
    lv_obj_set_style_bg_color(ui->screen_main, lv_color_hex(0x6200d6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_main, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->screen_main, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->screen_main, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen_main, 100, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_label_time = lv_label_create(ui->screen_main);
    // lv_label_set_text(ui->screen_label_time, "11:58");
    lv_label_set_text_fmt(ui->screen_label_time, "%02d:%02d", web_data.time.hour, web_data.time.minute);
    lv_label_set_long_mode(ui->screen_label_time, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_time, 3, 3);
    lv_obj_set_size(ui->screen_label_time, 60, 22);
    lv_obj_set_style_bg_opa(ui->screen_label_time, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_time, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_time, &lv_font_Acme_Regular_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_label_time, &label_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_time, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_cont_icon = lv_obj_create(ui->screen_main);
    lv_obj_set_size(ui->screen_cont_icon, 50, 45);
    lv_obj_set_pos(ui->screen_cont_icon, 8, 28);
    lv_obj_set_scrollbar_mode(ui->screen_cont_icon, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(ui->screen_cont_icon, &cont_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_icon, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_icon, lv_color_hex(0xf5f5f5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_icon, LV_OPA_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_img_wifi_icon
    ui->screen_img_wifi_icon = lv_img_create(ui->screen_cont_icon);
    lv_obj_add_flag(ui->screen_img_wifi_icon, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_wifi_icon, &_wifi_alpha_22x22);
    lv_img_set_pivot(ui->screen_img_wifi_icon, 0, 0);
    lv_img_set_angle(ui->screen_img_wifi_icon, 0);
    lv_obj_set_pos(ui->screen_img_wifi_icon, 3, 0);
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
    lv_img_set_src(ui->screen_img_ble_icon, &_ble2_alpha_22x22);
    lv_img_set_pivot(ui->screen_img_ble_icon, 0, 0);
    lv_img_set_angle(ui->screen_img_ble_icon, 0);
    lv_obj_set_pos(ui->screen_img_ble_icon, 25, 0);
    lv_obj_set_size(ui->screen_img_ble_icon, 22, 22);

    // Write style for screen_img_ble_icon, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT->
    lv_obj_set_style_img_recolor_opa(ui->screen_img_ble_icon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_img_ble_icon, lv_color_hex(0x0055ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_ble_icon, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_ble_icon, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_ble_icon, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_label_bettery1
    ui->screen_cont_bettery1 = lv_obj_create(ui->screen_cont_icon);
    lv_obj_set_pos(ui->screen_cont_bettery1, 42, 28);
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
    lv_obj_set_pos(ui->screen_cont_bettery0, 6, 22);
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
    lv_obj_set_pos(ui->screen_label_app_bg, 66, 3);
    lv_obj_set_size(ui->screen_label_app_bg, 214, 70);
    lv_obj_set_style_border_width(ui->screen_label_app_bg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_app_bg, 35, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_app_bg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_label_app_bg, lv_color_hex(0x00ffee), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_label_app_bg, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->screen_label_app_bg, lv_color_hex(0xd000ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->screen_label_app_bg, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen_label_app_bg, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_label_app_bg3 = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_label_app_bg3, "");
    lv_label_set_long_mode(ui->screen_label_app_bg3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_app_bg3, 204, 3);
    lv_obj_set_size(ui->screen_label_app_bg3, 80, 70);
    lv_obj_set_style_border_width(ui->screen_label_app_bg3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_app_bg3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_app_bg3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_label_app_bg3, lv_color_hex(0x00ffee), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_label_app_bg3, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->screen_label_app_bg3, lv_color_hex(0xd000ff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->screen_label_app_bg3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen_label_app_bg3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_label_app_bg4 = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_label_app_bg4, "");
    lv_label_set_long_mode(ui->screen_label_app_bg4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_app_bg4, 72, 7);
    lv_obj_set_size(ui->screen_label_app_bg4, 80, 62);
    lv_obj_set_style_border_width(ui->screen_label_app_bg4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_app_bg4, 31, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_app_bg4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_label_app_bg4, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_label_app_bg2 = lv_label_create(ui->screen_main);
    lv_label_set_text(ui->screen_label_app_bg2, "");
    lv_label_set_long_mode(ui->screen_label_app_bg2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_app_bg2, 204, 7);
    lv_obj_set_size(ui->screen_label_app_bg2, 80, 62);
    lv_obj_set_style_border_width(ui->screen_label_app_bg2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_app_bg2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_app_bg2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_label_app_bg2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Write codes screen_mian_cont_app
    ui->screen_cont_app = lv_obj_create(ui->screen_main);
    lv_obj_set_pos(ui->screen_cont_app, 80, 7);
    lv_obj_set_size(ui->screen_cont_app, 204, 62);
    lv_obj_set_scrollbar_mode(ui->screen_cont_app, LV_SCROLLBAR_MODE_OFF);

    // Write style for screen_cont_app, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_cont_app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_cont_app, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_cont_app, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_cont_app, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_cont_app, LV_GRAD_DIR_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_cont_app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_cont_app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_cont_app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_cont_app, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_cont_app, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    for (int i = 0; i < APP_ICON_NUM; i++)
    {
        app_icon_info[i].btn_img_obj = lv_img_create(ui->screen_cont_app);
        lv_img_set_src(app_icon_info[i].btn_img_obj, &app_icon_info[i].btn_img);
        lv_img_set_pivot(app_icon_info[i].btn_img_obj, 50, 50);
        lv_img_set_angle(app_icon_info[i].btn_img_obj, 0);
        lv_obj_set_pos(app_icon_info[i].btn_img_obj, app_icon_info[i].x_pos + 10, 16);
        lv_obj_set_size(app_icon_info[i].btn_img_obj, app_icon_info[i].width - 20, app_icon_info[i].height - 20);
        if (app_icon_info[i].app_id == 3)
        {
            lv_obj_set_pos(app_icon_info[i].btn_img_obj, app_icon_info[i].x_pos + 8, 14);
            lv_obj_set_size(app_icon_info[i].btn_img_obj, app_icon_info[i].width - 15, app_icon_info[i].height - 15);
        }

        app_icon_info[i].app_btn = lv_btn_create(ui->screen_cont_app);
        lv_obj_set_pos(app_icon_info[i].app_btn, app_icon_info[i].x_pos, app_icon_info[i].y_pos);
        lv_obj_set_size(app_icon_info[i].app_btn, app_icon_info[i].width, app_icon_info[i].height);
        lv_obj_set_style_bg_opa(app_icon_info[i].app_btn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(app_icon_info[i].app_btn, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(app_icon_info[i].app_btn, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(app_icon_info[i].app_btn, lv_color_hex(0xf9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(app_icon_info[i].app_btn, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(app_icon_info[i].app_btn, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_add_event_cb(app_icon_info[i].app_btn, app_icon_info[i].event_cb, LV_EVENT_ALL, NULL);
    }

    lv_obj_update_snap(ui->screen_main, LV_ANIM_ON);
    lv_obj_update_layout(ui->screen_main);
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
    lv_obj_set_pos(ui->screen_weather_img_location, 3, 3);
    lv_obj_set_size(ui->screen_weather_img_location, 12, 12);
    lv_obj_set_style_img_recolor_opa(ui->screen_weather_img_location, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->screen_weather_img_location, lv_color_hex(0xff6500), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_weather_img_location, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_weather_location = lv_label_create(ui->screen_main_weather);
    lv_label_set_text_fmt(ui->screen_weather_location, "%s", cityname);
    lv_obj_set_style_text_font(ui->screen_weather_location, &lv_font_Acme_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_weather_location, 18, 3);
    lv_obj_set_size(ui->screen_weather_location, 120, 15);
    lv_obj_set_style_text_align(ui->screen_weather_location, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_location, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_weather_datatime = lv_label_create(ui->screen_main_weather);
    lv_label_set_text(ui->screen_weather_datatime, "2025-01-01 \n12:00:00"); // test
    lv_obj_set_pos(ui->screen_weather_datatime, 160, 30);
    lv_obj_set_size(ui->screen_weather_datatime, 90, 35);
    lv_obj_set_style_text_align(ui->screen_weather_datatime, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_weather_datatime, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_weather_datatime, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_datatime, &lv_font_Acme_Regular_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_weather_temp = lv_label_create(ui->screen_main_weather);
    lv_label_set_text(ui->screen_weather_temp, "0.0°C");
    lv_obj_set_pos(ui->screen_weather_temp, 160, 0);
    lv_obj_set_size(ui->screen_weather_temp, 120, 25);
    lv_obj_set_style_text_align(ui->screen_weather_temp, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_weather_temp, &lv_font_OrbitronBlack_28, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_weather_img_weather[0] = lv_img_create(ui->screen_main_weather);
    lv_img_set_src(ui->screen_weather_img_weather[0], &_sun_50x50);
    lv_obj_set_pos(ui->screen_weather_img_weather[0], 230, 23);
    lv_obj_set_size(ui->screen_weather_img_weather[0], 50, 50);
    // lv_obj_add_state(ui->screen_weather_img_weather[0],LV_OBJ_FLAG_HIDDEN);

    ui->screen_weather_img_weather[1] = lv_img_create(ui->screen_main_weather);
    lv_img_set_src(ui->screen_weather_img_weather[1], &_suncloud_alpha_50x50);
    lv_obj_set_pos(ui->screen_weather_img_weather[1], 230, 23);
    lv_obj_set_size(ui->screen_weather_img_weather[1], 50, 50);
    lv_obj_add_flag(ui->screen_weather_img_weather[1], LV_OBJ_FLAG_HIDDEN);

    ui->screen_weather_img_weather[2] = lv_img_create(ui->screen_main_weather);
    lv_img_set_src(ui->screen_weather_img_weather[2], &_rain_50x50);
    lv_obj_set_pos(ui->screen_weather_img_weather[2], 230, 23);
    lv_obj_set_size(ui->screen_weather_img_weather[2], 50, 50);
    lv_obj_add_flag(ui->screen_weather_img_weather[2], LV_OBJ_FLAG_HIDDEN);

    ui->screen_weather_img_weather[3] = lv_img_create(ui->screen_main_weather);
    lv_img_set_src(ui->screen_weather_img_weather[3], &_snow_50x50);
    lv_obj_set_pos(ui->screen_weather_img_weather[3], 230, 23);
    lv_obj_set_size(ui->screen_weather_img_weather[3], 50, 50);
    lv_obj_add_flag(ui->screen_weather_img_weather[3], LV_OBJ_FLAG_HIDDEN);

    info_weather[0] = {
        .x = 5,
        .y = 20,
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
        .y = 20,
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
        .y = 20,
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
    lv_obj_set_pos(ui->screen_btn_right, 0, 0);
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
    lv_obj_set_pos(ui->screen_btn_left, 0, 40);
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
    lv_obj_set_pos(ui->screen_btn_touch, 130, 0);
    lv_obj_set_size(ui->screen_btn_touch, 154, 76);
    lv_obj_add_style(ui->screen_btn_touch, &btn_default_style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_btn_touch, &touch_focused_style, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_btn_touch, &lv_font_OrbitronBlack_22, LV_PART_MAIN | LV_STATE_DEFAULT);

    events_init_screen_blehid(ui);
}

void ui_sd_init(lv_ui *ui)
{
    ui->screen_main_sd = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_main_sd, LV_SCREEN_WIDTH, LV_SCREEN_HEIGHT);
    lv_obj_set_pos(ui->screen_main_sd, 0, 0);
    lv_obj_set_style_bg_img_src(ui->screen_main_sd, &_bg3_284x76, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(ui->screen_main_sd, LV_SCROLLBAR_MODE_OFF);

    ui->screen_sd_label = lv_label_create(ui->screen_main_sd);
    lv_label_set_text(ui->screen_sd_label, "SD Card");
    lv_label_set_long_mode(ui->screen_sd_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_size(ui->screen_sd_label, LV_SCREEN_WIDTH, 20);
    lv_obj_set_pos(ui->screen_sd_label, 0, 5);
    lv_obj_set_style_text_color(ui->screen_sd_label, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_sd_label, &lv_font_OrbitronSemiBold_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_sd_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->screen_sd_label, 0, LV_STATE_DEFAULT);

    ui->screen_sd_line = lv_obj_create(ui->screen_main_sd);
    lv_obj_set_pos(ui->screen_sd_line, 2, 23);
    lv_obj_set_size(ui->screen_sd_line, LV_SCREEN_WIDTH - 4, 3);
    lv_obj_set_scrollbar_mode(ui->screen_sd_line, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(ui->screen_sd_line, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_sd_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_sd_line, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->screen_sd_line, lv_color_hex(0x097dff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->screen_sd_line, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen_sd_line, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->screen_sd_line, 0, LV_STATE_DEFAULT);

    ui->screen_sd_label_2 = lv_label_create(ui->screen_main_sd);
    lv_label_set_text(ui->screen_sd_label_2, "SD Card Size: NOT MOUNTED");
    lv_label_set_long_mode(ui->screen_sd_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_size(ui->screen_sd_label_2, 284, 12);
    lv_obj_set_pos(ui->screen_sd_label_2, 10, 30);
    lv_obj_set_style_text_color(ui->screen_sd_label_2, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_sd_label_2, &lv_font_Alatsi_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_sd_label_3 = lv_label_create(ui->screen_main_sd);
    lv_label_set_text(ui->screen_sd_label_3, "SD Card Used Size: NOT MOUNTED");
    lv_label_set_long_mode(ui->screen_sd_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_size(ui->screen_sd_label_3, 284, 12);
    lv_obj_set_pos(ui->screen_sd_label_3, 10, 50);
    lv_obj_set_style_text_color(ui->screen_sd_label_3, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_sd_label_3, &lv_font_Alatsi_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
}

lv_obj_t *bhi260_label[4] = {ui.screen_bhi260_roll,
                             ui.screen_bhi260_pitch,
                             ui.screen_bhi260_yaw,
                             ui.screen_bhi260_walk};
void ui_bhi260_init(lv_ui *ui)
{
    ui->screen_main_bhi260 = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_main_bhi260, LV_SCREEN_WIDTH, LV_SCREEN_HEIGHT);
    lv_obj_set_pos(ui->screen_main_bhi260, 0, 0);
    lv_obj_set_style_bg_img_src(ui->screen_main_bhi260, &_bg3_284x76, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(ui->screen_main_bhi260, LV_SCROLLBAR_MODE_OFF);

    static lv_style_t style_label;
    lv_style_init(&style_label);
    lv_style_set_text_color(&style_label, lv_color_black());
    lv_style_set_text_font(&style_label, &lv_font_Acme_Regular_14);
    lv_style_set_text_align(&style_label, LV_TEXT_ALIGN_CENTER);
    lv_style_set_text_letter_space(&style_label, 2);
    lv_style_set_text_line_space(&style_label, 10);
    lv_style_set_pad_top(&style_label, 10);
    lv_style_set_pad_bottom(&style_label, 10);
    lv_style_set_pad_right(&style_label, 0);
    lv_style_set_pad_left(&style_label, 0);

    lv_style_set_bg_opa(&style_label, LV_OPA_60);
    lv_style_set_bg_color(&style_label, lv_color_white());
    lv_style_set_radius(&style_label, 6);
    lv_style_set_border_width(&style_label, 0);

    char *bhi260_text[4] = {"Roll:\n0.00", "Pitch:\n0.00", "Yaw:\n0.00", "Walk:\n0.00"};
    for (int i = 0; i < 4; i++)
    {
        bhi260_label[i] = lv_label_create(ui->screen_main_bhi260);
        lv_obj_set_size(bhi260_label[i], 60, 60);
        lv_obj_set_pos(bhi260_label[i], 8 + i * 70, 8);
        lv_label_set_text(bhi260_label[i], bhi260_text[i]);
        lv_obj_add_style(bhi260_label[i], &style_label, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

void ui_set_init(lv_ui *ui)
{
    static lv_style_t style_set;
    lv_style_init(&style_set);
    lv_style_set_bg_color(&style_set, lv_color_hex(0x000000));
    lv_style_set_border_width(&style_set, 0);
    lv_style_set_radius(&style_set, 15);
    lv_style_set_bg_opa(&style_set, LV_OPA_COVER);

    ui->screen_main_set = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_main_set, LV_SCREEN_WIDTH, LV_SCREEN_HEIGHT);
    lv_obj_set_pos(ui->screen_main_set, 0, 0);
    lv_obj_set_style_bg_img_src(ui->screen_main_set, &_bg3_284x76, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(ui->screen_main_set, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_top(ui->screen_main_set, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_main_set, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui->screen_set_btn_wifi = lv_btn_create(ui->screen_main_set);
    lv_obj_add_style(ui->screen_set_btn_wifi, &style_set, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(ui->screen_set_btn_wifi, 60, 60);
    lv_obj_set_pos(ui->screen_set_btn_wifi, 16, 0);
    lv_obj_set_style_bg_img_src(ui->screen_set_btn_wifi, &_wifi_60x60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui->screen_set_btn_wifi, screen_btn_wifi_event_handler, LV_EVENT_ALL, NULL);

    ui->screen_set_btn_ble = lv_btn_create(ui->screen_main_set);
    lv_obj_add_style(ui->screen_set_btn_ble, &style_set, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(ui->screen_set_btn_ble, 60, 60);
    lv_obj_set_pos(ui->screen_set_btn_ble, 112, 0);
    lv_obj_set_style_bg_img_src(ui->screen_set_btn_ble, &_ble1_60x60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui->screen_set_btn_ble, screen_btn_ble_event_handler, LV_EVENT_ALL, NULL);

    ui->screen_set_btn_brightness = lv_btn_create(ui->screen_main_set);
    lv_obj_add_style(ui->screen_set_btn_brightness, &style_set, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(ui->screen_set_btn_brightness, 60, 60);
    lv_obj_set_pos(ui->screen_set_btn_brightness, 208, 0);
    lv_obj_set_style_bg_img_src(ui->screen_set_btn_brightness, &_brightness1_60x60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui->screen_set_btn_brightness, screen_btn_brightness_event_handler, LV_EVENT_ALL, NULL);
}

static void mask_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    static int16_t mask_top_id = -1;
    static int16_t mask_bottom_id = -1;

    if (code == LV_EVENT_COVER_CHECK)
    { // Check if the object is covered by another object
        lv_event_set_cover_res(e, LV_COVER_RES_MASKED);
    }
    else if (code == LV_EVENT_DRAW_MAIN_BEGIN)
    { // start drawing the object
        /* add mask */
        const lv_font_t *font = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
        lv_coord_t line_space = lv_obj_get_style_text_line_space(obj, LV_PART_MAIN);
        lv_coord_t font_h = lv_font_get_line_height(font);

        lv_area_t roller_coords;
        lv_obj_get_coords(obj, &roller_coords);

        lv_area_t rect_area;
        rect_area.x1 = roller_coords.x1;
        rect_area.x2 = roller_coords.x2;
        rect_area.y1 = roller_coords.y1;
        rect_area.y2 = roller_coords.y1 + (lv_obj_get_height(obj) - font_h - line_space) / 2;

        lv_draw_mask_fade_param_t *fade_mask_top = (lv_draw_mask_fade_param_t *)lv_mem_buf_get(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_top, &rect_area, LV_OPA_TRANSP, rect_area.y1, LV_OPA_COVER, rect_area.y2);
        mask_top_id = lv_draw_mask_add(fade_mask_top, NULL);

        rect_area.y1 = rect_area.y2 + font_h + line_space - 1;
        rect_area.y2 = roller_coords.y2;

        lv_draw_mask_fade_param_t *fade_mask_bottom = (lv_draw_mask_fade_param_t *)lv_mem_buf_get(sizeof(lv_draw_mask_fade_param_t));
        lv_draw_mask_fade_init(fade_mask_bottom, &rect_area, LV_OPA_COVER, rect_area.y1, LV_OPA_TRANSP, rect_area.y2);
        mask_bottom_id = lv_draw_mask_add(fade_mask_bottom, NULL);
    }
    else if (code == LV_EVENT_DRAW_POST_END)
    { // drawing is finished, remove the mask
        lv_draw_mask_fade_param_t *fade_mask_top = (lv_draw_mask_fade_param_t *)lv_draw_mask_remove_id(mask_top_id);
        lv_draw_mask_fade_param_t *fade_mask_bottom = (lv_draw_mask_fade_param_t *)lv_draw_mask_remove_id(mask_bottom_id);
        lv_draw_mask_free_param(fade_mask_top);
        lv_draw_mask_free_param(fade_mask_bottom);
        lv_mem_buf_release(fade_mask_top);
        lv_mem_buf_release(fade_mask_bottom);
        mask_top_id = -1;
        mask_bottom_id = -1;
    }
}

void ui_about_init(lv_ui *ui)
{
    static lv_style_t style_about;
    lv_style_init(&style_about);
    // lv_style_set_bg_color(&style_about, lv_color_hex(0x000000));
    lv_style_set_bg_opa(&style_about, 0);
    lv_style_set_text_color(&style_about, lv_color_white());
    lv_style_set_border_width(&style_about, 0);
    lv_style_set_pad_all(&style_about, 0);

    ui->screen_main_about = lv_obj_create(NULL);
    lv_obj_set_pos(ui->screen_main_about, 0, 0);
    lv_obj_set_size(ui->screen_main_about, 284, 76);
    lv_obj_add_style(ui->screen_main_about, &style_about, 0);
    lv_obj_set_scrollbar_mode(ui->screen_main_about, LV_SCROLLBAR_MODE_OFF);

    ui->screen_about_bg = lv_img_create(ui->screen_main_about);
    lv_obj_set_pos(ui->screen_about_bg, 0, 0);
    lv_obj_set_size(ui->screen_about_bg, 284, 76);
    lv_img_set_src(ui->screen_about_bg, &_bg3_284x76);

    about_text << "WiFi : " << "" << "\n";
    about_text << "WiFi Rssi: " << "" << "dBm\n";
    about_text << "Battery: " << "" << "%\n";
    about_text << "Battery Temp: " << "" << "C\n";
    about_text << "VBus: " << "" << "mV\n";
    about_text << "VBatt: " << "" << "mV\n";
    about_text << "VTarget: " << "" << "mV\n";
    about_text << "CHG STATE: " << "" << "\n";
    about_text << "CHG Current: " << "" << "mA";
    std::string str = about_text.str();

    ui->screen_about_roller_about = lv_roller_create(ui->screen_main_about);
    lv_obj_set_pos(ui->screen_about_roller_about, 0, 0);
    lv_obj_set_size(ui->screen_about_roller_about, 284, 76);
    lv_obj_add_style(ui->screen_about_roller_about, &style_about, 0);
    lv_obj_set_style_bg_opa(ui->screen_about_roller_about, LV_OPA_TRANSP, LV_PART_SELECTED);
    lv_obj_set_style_text_font(ui->screen_about_roller_about, &lv_font_OrbitronBlack_20, LV_PART_SELECTED);
    lv_obj_set_style_text_color(ui->screen_about_roller_about, lv_color_hex(0xffff00), LV_PART_SELECTED);

    lv_roller_set_options(ui->screen_about_roller_about,
                          str.c_str(),
                          LV_ROLLER_MODE_NORMAL);

    lv_obj_center(ui->screen_about_roller_about);
    lv_roller_set_visible_row_count(ui->screen_about_roller_about, 3);
    lv_obj_add_event_cb(ui->screen_about_roller_about, mask_event_cb, LV_EVENT_ALL, NULL);
}

void ui_timer_cb(lv_timer_t *timer)
{
    uint16_t static weather_count = 0;
    weather_count++;
    uint16_t static about_count = 0;
    about_count++;
    web_data.time.second++;
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
    }

    switch (src_load_page)
    {
    case HOME_PAGE: // mian page
    {
        lv_label_set_text_fmt(ui.screen_label_time, "%02d:%02d", web_data.time.hour, web_data.time.minute);
        lv_obj_set_size(ui.screen_cont_bettery2, ((uint8_t)about_info.Percentage * 28 / 100), 10);

        if (about_info.Percentage <= 15)
        {
            lv_obj_set_style_bg_color(ui.screen_cont_bettery2, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else
        {
            lv_obj_set_style_bg_color(ui.screen_cont_bettery2, lv_color_hex(0x02eb10), LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        if (WiFi.isConnected())
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
        else if (move_mouse && !bleMouse.isConnected())
        {
            bool static bleicon_flash = false;
            if (bleicon_flash)
            {
                lv_obj_set_style_img_recolor(ui.screen_img_ble_icon, lv_color_hex(0x0055ff), LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            else
            {
                lv_obj_set_style_img_recolor(ui.screen_img_ble_icon, lv_color_hex(0xd3d3d3), LV_PART_MAIN | LV_STATE_DEFAULT);
            }
            bleicon_flash = !bleicon_flash;
        }
        else
        {
            lv_obj_set_style_img_recolor(ui.screen_img_ble_icon, lv_color_hex(0xd3d3d3), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        break;
    }
    case WEATHER_PAGE: // weather page
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
    case AIR_MOUSE_PAGE: // air mouse page
        break;
    case SD_PAGE: // sd page
        lv_label_set_text_fmt(ui.screen_sd_label_2, "SD Card Size: %.2f MB", sd.sd_size);
        lv_label_set_text_fmt(ui.screen_sd_label_3, "SD Card Used Size: %.2f MB", sd.sd_used);
        break;
    case BHI260_PAGE: // set page
        lv_label_set_text_fmt(bhi260_label[0], "Roll:\n%.2f", roll);
        lv_label_set_text_fmt(bhi260_label[1], "Pitch:\n%.2f", pitch);
        lv_label_set_text_fmt(bhi260_label[2], "Yaw:\n%.2f", yaw);
        lv_label_set_text_fmt(bhi260_label[3], "Walk:\n%d", walk_count);
        break;
    case SET_PAGE: // set page
        break;
    case ABOUT_PAGE: // about page
    {
        if (about_count >= 3)
        {
            about_text.str("");
            about_text.clear();
            about_text << "WiFi : " << about_info.wifissid << "\n";
            about_text << "WiFi Rssi: " << (int)about_info.Rssi << "dBm\n";
            about_text << "Battery: " << (int)about_info.Percentage << "%\n";
            about_text << "Battery Temp: " << (int)about_info.BatteryTemp << "C\n";
            about_text << "VBus: " << (int)about_info.VBusVoltage << "mV\n";
            about_text << "VBatt: " << (int)about_info.VBattVoltage << "mV\n";
            about_text << "VTarget: " << (int)about_info.ChargeTargetVoltage << "mV\n";
            about_text << "CHG STATE: " << about_info.ChargeStatus << "\n";
            about_text << "CHG Current: " << about_info.ChargeCurrent << "mA";
            std::string str = about_text.str();

            // Serial.printf("wifissid: %s\n", about_info.wifissid.c_str());
            // Serial.printf("Percentage: %d\n",about_info.Percentage);
            // Serial.printf("BatteryTemp: %d\n",about_info.BatteryTemp);
            // Serial.printf("VBusVoltage: %d\n",about_info.VBusVoltage);
            // Serial.printf("ChargeCurrent: %d\n",about_info.ChargeCurrent);
            // Serial.printf("VBattVoltage: %d\n",about_info.VBattVoltage);
            // Serial.printf("ChargeCurrent: %d\n",about_info.ChargeCurrent);

            // Serial.println("About:");
            // Serial.println(str.c_str());

            lv_roller_set_options(ui.screen_about_roller_about,
                                  str.c_str(),
                                  LV_ROLLER_MODE_NORMAL);
            about_count = 0;
        }
        break;
    }
    default:
        break;
    }
}

void Openinganimation()
{
    lv_obj_t *scr = lv_scr_act();
    lv_obj_remove_style_all(scr);

    lv_obj_t *Openinganimation_bg = lv_img_create(scr);
    lv_img_set_src(Openinganimation_bg, &_bg3_284x76);

    lv_obj_t *logo = lv_label_create(scr);
    lv_label_set_text(logo, "");
    lv_obj_set_pos(logo, 10, (LV_SCREEN_HEIGHT - 66) / 2);
    lv_obj_set_size(logo, 58, 66);
    lv_obj_set_style_bg_opa(logo, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(logo, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(logo, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(logo, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(logo, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(logo, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(logo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(logo, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(logo, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(logo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(logo, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_img_src(logo, &_LOGO1_58x66, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(logo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_anim_t logo_a;
    lv_anim_init(&logo_a);
    lv_anim_set_var(&logo_a, logo);
    lv_anim_set_values(&logo_a, -50, 15);
    lv_anim_set_time(&logo_a, 800);
    lv_anim_set_path_cb(&logo_a, lv_anim_path_ease_out);
    lv_anim_set_exec_cb(&logo_a, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_start(&logo_a);

    // 创建容器用于放置所有字母
    lv_obj_t *cont = lv_obj_create(scr);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, LV_SCREEN_WIDTH, LV_SCREEN_HEIGHT);
    lv_obj_center(cont);
    // 字母数组
    const char *letters = "LILYGO";
    const int letterCount = 6;
    lv_obj_t *letterLabels[letterCount];

    for (int i = 0; i < letterCount; i++)
    {
        letterLabels[i] = lv_label_create(cont);
        lv_obj_set_size(letterLabels[i], 30, 40);
        lv_label_set_text_fmt(letterLabels[i], "%c", letters[i]);
        lv_obj_set_style_text_font(letterLabels[i], &lv_font_TrainOneRegular_40, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(letterLabels[i], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        int8_t startY;
        if (i % 2 == 0)
        {
            startY = -50;
            lv_obj_set_style_text_color(letterLabels[i], lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else
        {
            startY = LV_SCREEN_HEIGHT + 50;
            lv_obj_set_style_text_color(letterLabels[i], lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        // int startY = (i % 2 == 0) ? -50 : LV_SCREEN_HEIGHT + 50;
        lv_obj_set_pos(letterLabels[i], 90 + i * 30, startY);
        if (i == 5) // "O"
            lv_obj_set_pos(letterLabels[i], 95 + i * 30, startY);

        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, letterLabels[i]);
        lv_anim_set_values(&a, startY, (LV_SCREEN_HEIGHT - 40) / 2);
        lv_anim_set_time(&a, 800);
        lv_anim_set_delay(&a, i * 100);
        lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);

        lv_anim_start(&a);
    }
}

void Check_Error_ui()
{
    char title[16] = "SUCCESS";
    char check_text[200] = "";
    if (!peripheral.BHI260AP)
    {
        strcpy(title, "WARNING");
        strcat(check_text, "BHI260AP not found\n");
    }
    if (!peripheral.BQ27220)
    {
        strcpy(title, "WARNING");
        strcat(check_text, "BQ27220 not found\n");
    }
    if (!peripheral.PPM)
    {
        strcpy(title, "WARNING");
        strcat(check_text, "PPM not found\n");
    }
    if (!peripheral.RTC)
    {
        strcpy(title, "WARNING");
        strcat(check_text, "RTC not found\n");
    }
    if (!peripheral.SD)
    {
        strcpy(title, "WARNING");
        strcat(check_text, "SD not found\n");
    }
    if (!peripheral.TOUCH)
    {
        strcpy(title, "WARNING");
        strcat(check_text, "TOUCH not found\n");
    }

    if (strcmp(title, "SUCCESS") == 0)
    {
        strcpy(check_text, "All peripherals found\n");
    }
    Serial.printf("check_label:");
    Serial.println(check_text);

    lv_obj_t *scr = lv_scr_act();
    lv_obj_remove_style_all(scr);

    lv_obj_t *Openinganimation_bg = lv_img_create(scr);
    lv_img_set_src(Openinganimation_bg, &_bg3_284x76);

    lv_obj_t *screen_cheak_img = lv_img_create(scr);
    lv_obj_add_flag(screen_cheak_img, LV_OBJ_FLAG_CLICKABLE);
    if (strcmp(title, "SUCCESS") == 0)
    {
        lv_img_set_src(screen_cheak_img, &_yes_alt_alpha_50x50);
    }
    else
    {
        lv_img_set_src(screen_cheak_img, &_bxs_error_alpha_50x50);
    }
    lv_img_set_pivot(screen_cheak_img, 50, 50);
    lv_img_set_angle(screen_cheak_img, 0);
    lv_obj_set_pos(screen_cheak_img, 3, 18);
    lv_obj_set_size(screen_cheak_img, 50, 50);

    lv_obj_set_style_img_recolor_opa(screen_cheak_img, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(screen_cheak_img, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(screen_cheak_img, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(screen_cheak_img, true, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *screen_cheak_title = lv_label_create(scr);
    lv_label_set_text(screen_cheak_title, title);
    lv_label_set_long_mode(screen_cheak_title, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(screen_cheak_title, 0, 0);
    lv_obj_set_size(screen_cheak_title, 284, 20);

    lv_obj_set_style_border_width(screen_cheak_title, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(screen_cheak_title, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    if (strcmp(title, "SUCCESS") == 0)
    {
        lv_obj_set_style_text_color(screen_cheak_title, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_obj_set_style_text_color(screen_cheak_title, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    lv_obj_set_style_text_font(screen_cheak_title, &lv_font_Acme_Regular_22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(screen_cheak_title, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_cheak_title, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_cheak_title, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_cheak_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_cheak_title, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(screen_cheak_title, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_cheak_title, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *screen_cheak_label = lv_label_create(scr);
    lv_label_set_text(screen_cheak_label, check_text);
    lv_label_set_long_mode(screen_cheak_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(screen_cheak_label, 55, 22);
    lv_obj_set_size(screen_cheak_label, 220, 40);

    lv_obj_set_style_border_width(screen_cheak_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(screen_cheak_label, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    if (strcmp(title, "SUCCESS") == 0)
    {
        lv_obj_set_style_text_color(screen_cheak_label, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    else
    {
        lv_obj_set_style_text_color(screen_cheak_label, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    lv_obj_set_style_text_font(screen_cheak_label, &lv_font_Acme_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(screen_cheak_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(screen_cheak_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(screen_cheak_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(screen_cheak_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen_cheak_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_cheak_label, lv_color_hex(0x7bc0fa), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(screen_cheak_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(screen_cheak_label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void wifi_ap_init()
{
    ui.screen_wifi_ap = lv_obj_create(NULL);
    ui.wifiap_title = lv_label_create(ui.screen_wifi_ap);
    lv_label_set_text(ui.wifiap_title, "WIFI CONFIG");
    lv_obj_set_pos(ui.wifiap_title, 0, 0);
    lv_obj_set_size(ui.wifiap_title, LV_SCREEN_WIDTH, 20);

    lv_obj_set_style_border_width(ui.wifiap_title, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui.wifiap_title, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui.wifiap_title, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui.wifiap_title, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui.wifiap_title, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui.wifiap_title, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui.wifiap_title, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui.wifiap_title, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui.wifiap_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui.wifiap_title, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui.wifiap_title, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui.wifiap_title, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui.wifiap_title, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui.wifiap_title, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    String ip = WiFi.softAPIP().toString();
    Serial.println("AP IP address: " + ip);

    ui.wifiap_text = lv_label_create(ui.screen_wifi_ap);
    lv_label_set_text_fmt(ui.wifiap_text, "Please connect Display-Bar Config WIFI with your mobile phone and go to %s to configure WIFI", ip.c_str());
    lv_label_set_long_mode(ui.wifiap_text, LV_LABEL_LONG_WRAP);
    lv_obj_set_size(ui.wifiap_text, LV_SCREEN_WIDTH, 56);
    lv_obj_set_pos(ui.wifiap_text, 0, 20);
    lv_obj_set_style_bg_opa(ui.wifiap_text, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui.wifiap_text, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui.wifiap_text, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui.wifiap_text, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui.wifiap_text, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui.wifiap_text, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui.wifiap_text, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui.wifiap_text, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui.wifiap_text, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_scr_load_anim(ui.screen_wifi_ap, LV_SCR_LOAD_ANIM_FADE_IN, 0, 800, false);

}

void enter_ui(void)
{
    Serial.println("enter_ui");

    init_style();

    ui_main_init(&ui);
    ui_weather_init(&ui);
    ui_air_mouse(&ui);
    ui_sd_init(&ui);
    ui_bhi260_init(&ui);
    ui_about_init(&ui);
    ui_set_init(&ui);

    lv_scr_load_anim(ui.screen_main, LV_SCR_LOAD_ANIM_NONE, 0, 4000, false);

    lv_timer_t *timer = lv_timer_create(ui_timer_cb, 1000, NULL);
    lv_timer_ready(timer);
}