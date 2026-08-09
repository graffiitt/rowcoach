#include "user_app.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <stdio.h>

#include "button_gpio.h"
#include "button_types.h"
#include "iot_button.h"
#include "lvgl_bsp.h"
#include "ui.h"


const button_config_t btn_cfg = {0, 0};
const button_gpio_config_t btn_gpio_cfg = {.gpio_num = 18, // key
                                           .active_level = 0};
button_handle_t gpio_btn = NULL;

static void button_single_click_cb(void *arg, void *usr_data) {
  iot_button_print_event((button_handle_t)arg);
}

void Lvgl_LoopTask(void *arg) {
  esp_err_t ret =
      iot_button_new_gpio_device(&btn_cfg, &btn_gpio_cfg, &gpio_btn);
  if (ret != ESP_OK) {
    ESP_LOGE("MAIN", "Button create failed");
  }
  iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, NULL,
                         button_single_click_cb, NULL);
  iot_button_register_cb(gpio_btn, BUTTON_DOUBLE_CLICK, NULL,
                         button_single_click_cb, NULL);
  iot_button_register_cb(gpio_btn, BUTTON_LONG_PRESS_UP, NULL,
                         button_single_click_cb, NULL);

  vTaskDelay(pdMS_TO_TICKS(3000));
  if (Lvgl_lock(-1)) {
    
    _ui_screen_change(&ui_base_screen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 500, 0, &ui_base_screen_init);
    // lv_disp_load_scr(ui_base_screen);

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
