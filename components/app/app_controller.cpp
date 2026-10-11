#include "app_controller.h"

#include <esp_err.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>

#include "app_state.h"
#include "board.h"
#include "lvgl_bsp.h"
#include "rtc_bsp.h"
#include "ui.h"

namespace {
constexpr uint32_t kMetricsPeriodMs = 1000;
constexpr uint32_t kTaskStackSize = 4096;
constexpr UBaseType_t kTaskPriority = 2;
constexpr BaseType_t kTaskCore = 1;
const char *TAG = "AppController";

AppState app_state;
bool rtc_read_error_logged = false;
bool started = false;

void UpdateFromRtc() {
  rtcTimeStruct_t time = {};
  const esp_err_t err = Rtc_GetTime(&time);
  if (err != ESP_OK) {
    if (!rtc_read_error_logged) {
      ESP_LOGE(TAG, "PCF85063 read failed: %s", esp_err_to_name(err));
      rtc_read_error_logged = true;
    }
    return;
  }

  char time_text[9];
  snprintf(time_text, sizeof(time_text), "%02d:%02d:%02d", time.hour,
           time.minute, time.second);
  app_state.updated_at_us = esp_timer_get_time();

  if (Lvgl_lock(-1)) {
    ui_title_set_time(time_text);
    Lvgl_unlock();
  }

  if (rtc_read_error_logged) {
    ESP_LOGI(TAG, "PCF85063 communication recovered");
    rtc_read_error_logged = false;
  }
}

void AppTask(void *argument) {
  vTaskDelay(pdMS_TO_TICKS(1000));
  if (Lvgl_lock(-1)) {
    _ui_screen_change(&ui_base_screen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 500, 0,
                      &ui_base_screen_init);
    Lvgl_unlock();
  }

  for (;;) {
    UpdateFromRtc();
    vTaskDelay(pdMS_TO_TICKS(kMetricsPeriodMs));
  }
}

} // namespace

void AppController_Init() { ui_init(); }

esp_err_t AppController_Start() {
  if (started) {
    return ESP_OK;
  }
  const BaseType_t result =
      xTaskCreatePinnedToCore(AppTask, "App", kTaskStackSize, nullptr,
                              kTaskPriority, nullptr, kTaskCore);
  if (result != pdPASS) {
    ESP_LOGE(TAG, "Failed to create application task");
    return ESP_ERR_NO_MEM;
  }
  started = true;
  return ESP_OK;
}
