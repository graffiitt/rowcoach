#include "user_app.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <stdio.h>

#include "lvgl_bsp.h"
#include "rtc_bsp.h"
#include "ui.h"

namespace {

constexpr uint8_t kPcf85063Address = 0x51;
const char *TAG = "UserApp";
esp_err_t rtc_init_result = ESP_ERR_INVALID_STATE;
bool rtc_read_error_logged = false;

}  // namespace

void Lvgl_LoopTask(void *arg) {

  vTaskDelay(pdMS_TO_TICKS(1000));
  if (Lvgl_lock(-1)) {

    _ui_screen_change(&ui_base_screen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 500, 0,
                      &ui_base_screen_init);
    // lv_disp_load_scr(ui_base_screen);

    Lvgl_unlock();
  }

  for (;;) {
    if (rtc_init_result == ESP_OK) {
      rtcTimeStruct_t time = {};
      const esp_err_t err = Rtc_GetTime(&time);
      if (err == ESP_OK) {
        char time_text[9];
        snprintf(time_text, sizeof(time_text), "%02d:%02d:%02d", time.hour,
                 time.minute, time.second);
        if (Lvgl_lock(-1)) {
          ui_title_set_time(time_text);
          Lvgl_unlock();
        }
        if (rtc_read_error_logged) {
          ESP_LOGI(TAG, "RTC communication recovered");
          rtc_read_error_logged = false;
        }
      } else if (!rtc_read_error_logged) {
        ESP_LOGE(TAG, "Failed to read time from PCF85063: %s",
                 esp_err_to_name(err));
        rtc_read_error_logged = true;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void UserApp_AppInit(I2cMasterBus *i2c_bus) {
  rtc_init_result = Rtc_Setup(i2c_bus, kPcf85063Address);
  if (rtc_init_result != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize PCF85063: %s",
             esp_err_to_name(rtc_init_result));
  }
}

void UserApp_UiInit() { ui_init(); }

void UserApp_TaskInit() {
  xTaskCreatePinnedToCore(Lvgl_LoopTask, "Lvgl_LoopTask", 4 * 1024, NULL, 2,
                          NULL, 1);
}
