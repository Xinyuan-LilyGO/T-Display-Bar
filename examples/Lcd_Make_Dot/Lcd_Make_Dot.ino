#include <TFT_eSPI.h>
#include "TouchDrvCSTXXX.hpp"

#define SCREEN_ROTATION 3
#define TOUCH_RST 1
#define TOUCH_IRQ 21
#define TOUCH_SDA 3
#define TOUCH_SCL 2
#define HOME_BUTTON_X 30
#define HOME_BUTTON_Y 400
#define POINT_RADIUS 3

TFT_eSPI tft = TFT_eSPI();
TouchDrvCSTXXX touch;

int16_t touch_x[5];
int16_t touch_y[5];
bool touch_ready = false;

void clear_screen(void *user_data)
{
  tft.fillScreen(TFT_BLACK);
  Serial.println("Home key pressed, screen cleared");
}

void configure_touch_rotation(uint8_t rotation)
{
  switch (rotation)
  {
  case 0:
    touch.setSwapXY(false);
    touch.setMirrorXY(false, false);
    break;
  case 1:
    touch.setSwapXY(true);
    touch.setMirrorXY(true, false);
    break;
  case 2:
    touch.setSwapXY(false);
    touch.setMirrorXY(true, false);
    break;
  case 3:
    touch.setSwapXY(true);
    touch.setMirrorXY(true, false);
    break;
  }

  touch.setMaxCoordinates(tft.width() - 1, tft.height() - 1);
}

void setup()
{
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(SCREEN_ROTATION);
  tft.fillScreen(TFT_BLACK);

  touch.setPins(TOUCH_RST, TOUCH_IRQ);
  touch_ready = touch.begin(Wire, CST816_SLAVE_ADDRESS, TOUCH_SDA, TOUCH_SCL);
  if (!touch_ready)
  {
    Serial.println("Failed to initialize touch controller");
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(1);
    tft.drawString("Touch init failed", 2, 2);
    return;
  }

  configure_touch_rotation(SCREEN_ROTATION);
  touch.setCenterButtonCoordinate(HOME_BUTTON_X, HOME_BUTTON_Y);
  touch.setHomeButtonCallback(clear_screen, nullptr);

  Serial.print("Touch controller: ");
  Serial.println(touch.getModelName());
  Serial.printf("Screen size: %d x %d\n", tft.width(), tft.height());
}

void loop()
{
  if (!touch_ready)
  {
    delay(1000);
    return;
  }

  uint8_t point_count = touch.getPoint(touch_x, touch_y, touch.getSupportTouchPoint());
  for (uint8_t i = 0; i < point_count; i++)
  {
    int16_t point_x = touch_x[i];
    int16_t point_y = touch_y[i];

    Serial.printf("Point[%u] X:%d Y:%d\n", i, point_x, point_y);

    if (point_x >= 0 && point_x < tft.width() && point_y >= 0 && point_y < tft.height())
    {
      tft.fillCircle(point_x, point_y, POINT_RADIUS, TFT_GREEN);
    }
  }

  delay(5);
}
