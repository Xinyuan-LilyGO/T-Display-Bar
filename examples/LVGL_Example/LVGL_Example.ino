/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

#include <lvgl.h>
#include <TFT_eSPI.h>
#include "TouchDrvCSTXXX.hpp"

/*To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 *You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 Note that the `lv_examples` library is for LVGL v7 and you shouldn't install it for this version (since LVGL v8)
 as the examples and demos are now part of the main LVGL library. */

/*Change to your screen resolution*/
uint16_t screenWidth = TFT_WIDTH;
uint16_t screenHeight = TFT_HEIGHT;
uint16_t move_X = 0;
uint16_t move_Y = 0;
#define LV_ROTATION 1
#define BUFFER_SIZE (TFT_HEIGHT * 5)
#define LV_SCREEN_COMPENSATION 5

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[BUFFER_SIZE];
static lv_color_t buf2[BUFFER_SIZE];

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
TouchDrvCSTXXX touch;
int16_t x[5], y[5];
#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
  Serial.printf(buf);
  Serial.flush();
}
#endif

/* Display flushing */
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
    data->point.y = y[0];

    Serial.print("Data x ");
    Serial.println(x[0]);

    Serial.print("Data y ");
    Serial.println(y[0]);
  }
}

void lvgl_setRotation(int rotation)
{
  tft.setRotation(rotation); /* Landscape orientation, flipped */
  switch (rotation)
  {
  case 0:
    screenHeight = TFT_WIDTH + LV_SCREEN_COMPENSATION;
    screenWidth = TFT_HEIGHT;
    move_X = -LV_SCREEN_COMPENSATION;
    move_Y = 0;
    touch.setSwapXY(0);
    touch.setMirrorXY(0, 0);
    break;
  case 1:
    screenHeight = TFT_HEIGHT;
    screenWidth = TFT_WIDTH + LV_SCREEN_COMPENSATION;
    move_X = 0;
    move_Y = -LV_SCREEN_COMPENSATION;
    touch.setSwapXY(1);
    touch.setMirrorXY(1, 0);
    break;
  case 2:
    screenHeight = TFT_WIDTH + LV_SCREEN_COMPENSATION;
    screenWidth = TFT_HEIGHT;
    move_X = -LV_SCREEN_COMPENSATION;
    move_Y = 0;
    touch.setSwapXY(0);
    touch.setMirrorXY(1, 0);
    break;
  case 3:
    screenHeight = TFT_HEIGHT;
    screenWidth = TFT_WIDTH + LV_SCREEN_COMPENSATION;
    move_X = 0;
    move_Y = -LV_SCREEN_COMPENSATION;
    touch.setSwapXY(1);
    touch.setMirrorXY(0, 1);
    break;
  }
}

void setup()
{
  Serial.begin(115200); /* prepare for possible serial debug */

  touch.setPins(1, 21);
  touch.begin(Wire, CST816_SLAVE_ADDRESS, 3, 2);

  tft.begin();                  /* TFT init */
  lvgl_setRotation(LV_ROTATION);
  tft.fillScreen(TFT_RED);

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(4);
  tft.print("Hello World!");

  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  /*Set the touchscreen calibration data,
   the actual data for your display can be acquired using
   the Generic -> Touch_calibrate example from the TFT_eSPI library*/
  // uint16_t calData[5] = {275, 3620, 264, 3532, 1};
  //  tft.setTouch( calData );

  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, BUFFER_SIZE);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenHeight;
  disp_drv.ver_res = screenWidth;
  disp_drv.offset_x = move_X;
  disp_drv.offset_y = move_Y;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  /* Create simple label */
  String LVGL_Arduino = "LVGL!\n";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch(); // 版本

  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0);
  lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xFFFFFF), 0);

  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, LVGL_Arduino.c_str());
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
  lv_obj_set_style_text_color(label, lv_color_hex(0xff0000), LV_PART_MAIN);
  lv_obj_set_style_bg_color(label, lv_color_white(), LV_PART_MAIN);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  Serial.println("Setup done");
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}
