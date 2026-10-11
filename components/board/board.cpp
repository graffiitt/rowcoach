#include "board.h"

#include <esp_err.h>
#include <esp_log.h>

#include "board_config.h"
#include "display_bsp.h"
#include "i2c_bsp.h"
#include "lvgl_bsp.h"
#include "rtc_bsp.h"

namespace {
constexpr uint8_t kPcf85063Address = 0x51;

const char *TAG = "BOARD";

DisplayPort display(RLCD_MOSI_PIN, RLCD_SCK_PIN, RLCD_DC_PIN, RLCD_CS_PIN,
                    RLCD_RST_PIN, LCD_WIDTH, LCD_HEIGHT);
I2cMasterBus i2c_bus(ESP32_I2C_SCL_PIN, ESP32_I2C_SDA_PIN, 0);

void DisplayFlushCallback(lv_disp_drv_t *driver, const lv_area_t *area,
                          lv_color_t *color_map) {
  uint16_t *pixel = reinterpret_cast<uint16_t *>(color_map);
  for (int y = area->y1; y <= area->y2; ++y) {
    for (int x = area->x1; x <= area->x2; ++x) {
      const uint8_t color = (*pixel < 0x7fff) ? ColorBlack : ColorWhite;
      display.RLCD_SetPixel(x, y, color);
      ++pixel;
    }
  }
  display.RLCD_Display();
  lv_disp_flush_ready(driver);
}

} // namespace

void Board_Init() {
  display.RLCD_Init();
  Lvgl_PortInit(LCD_WIDTH, LCD_HEIGHT, DisplayFlushCallback);
  auto rtc_init_result = Rtc_Setup(&Board_GetI2cBus(), kPcf85063Address);
  if (rtc_init_result != ESP_OK) {
    ESP_LOGE(TAG, "PCF85063 initialization failed");
  }
}

void Board_sleep();

void Board_wakeup();

I2cMasterBus &Board_GetI2cBus() { return i2c_bus; }
