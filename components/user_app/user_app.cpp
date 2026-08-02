#include "user_app.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <stdio.h>

#include "lvgl_bsp.h"
#include "ui.h"

void Lvgl_LoopTask(void *arg) {
  vTaskDelay(pdMS_TO_TICKS(3000));

  if (Lvgl_lock(-1)) {
    lv_disp_load_scr(ui_base_screen);
    Lvgl_unlock();
  }

  for (;;) {

    vTaskDelay(pdMS_TO_TICKS(1500));
  }
}

void UserApp_AppInit() {}

void UserApp_UiInit() { ui_init(); }

void UserApp_TaskInit() {
  xTaskCreatePinnedToCore(Lvgl_LoopTask, "Lvgl_LoopTask", 4 * 1024, NULL, 2,
                          NULL, 1);
}
