#include "lvgl_bsp.h"
#include <cstddef>
#include <esp_log.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <stdio.h>
#include <string.h>

#include "button_gpio.h"
#include "button_types.h"
#include "core/lv_group.h"
#include "hal/lv_hal_indev.h"
#include "iot_button.h"

static const char *TAG = "LvglPort";

static lv_disp_draw_buf_t
    disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
static lv_disp_drv_t disp_drv; // contains callback functions
static SemaphoreHandle_t lvgl_mux = NULL;

static lv_indev_drv_t btn_drv;
lv_indev_t *btn_indev = NULL;

const button_config_t btn_cfg = {0, 0};
const button_gpio_config_t btn_gpio_cfg[4] = {{.gpio_num = 18, // key
                                               .active_level = 0,
                                               .enable_power_save = false,
                                               .disable_pull = false}};
button_handle_t gpio_btn[4] = {NULL};
struct state_button_t {
  uint32_t key;
  lv_indev_state_t state;
} state_button[5]{{LV_KEY_ENTER, LV_INDEV_STATE_RELEASED}};

static void button_push_cb(void *arg, void *usr_data) {
  // button_event_t event = iot_button_get_event((button_handle_t)arg);
  // switch ((int)event) {
  // case BUTTON_PRESS_DOWN:
  //   state_button[0].state = LV_INDEV_STATE_PRESSED;
  //   break;
  // case BUTTON_SINGLE_CLICK:
  //   state_button[0].state = LV_INDEV_STATE_RELEASED;
  //   break;
  // }
  // state_button[0].key = ((state_button_t *)usr_data)->key;
}

static void lv_button_cb(struct _lv_indev_drv_t *indev_drv,
                         lv_indev_data_t *data) {
  if (state_button[0].key != 0) {
    data->key = state_button[0].key;
    data->state = state_button[0].state;
  } else
    data->state = LV_INDEV_STATE_RELEASED;
}

static void Increase_lvgl_tick(void *arg) { lv_tick_inc(LVGL_TICK_PERIOD_MS); }

bool Lvgl_lock(int timeout_ms) {
  const TickType_t timeout_ticks =
      (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
  return xSemaphoreTake(lvgl_mux, timeout_ticks) == pdTRUE;
}

void Lvgl_unlock(void) {
  assert(lvgl_mux && "bsp_display_start must be called first");
  xSemaphoreGive(lvgl_mux);
}

static void Lvgl_port_task(void *arg) {
  uint32_t task_delay_ms = LVGL_TASK_MAX_DELAY_MS;
  for (;;) {
    if (Lvgl_lock(-1)) {
      task_delay_ms = lv_timer_handler();
      // Release the mutex
      Lvgl_unlock();
    }
    if (task_delay_ms > LVGL_TASK_MAX_DELAY_MS) {
      task_delay_ms = LVGL_TASK_MAX_DELAY_MS;
    }
    vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
  }
}

void Lvgl_PortInit(int width, int height, DispFlushCb flush_cb) {
  lvgl_mux = xSemaphoreCreateMutex();
  lv_init();
  lv_color_t *buffer1 = (lv_color_t *)heap_caps_malloc(
      width * height * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
  assert(buffer1);
  lv_color_t *buffer2 = (lv_color_t *)heap_caps_malloc(
      width * height * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
  assert(buffer2);

  lv_disp_draw_buf_init(&disp_buf, buffer1, buffer2, width * height);
  ESP_LOGI(TAG, "Register display driver to LVGL");

  esp_err_t ret =
      iot_button_new_gpio_device(&btn_cfg, &btn_gpio_cfg[0], &gpio_btn[0]);
  if (ret != ESP_OK) {
    ESP_LOGE("MAIN", "Button create failed");
  }

  iot_button_register_cb(gpio_btn[0], BUTTON_SINGLE_CLICK, NULL, button_push_cb,
                         &state_button[0]);
  //   iot_button_register_cb(gpio_btn[0], BUTTON_DOUBLE_CLICK, NULL,
  //   button_push_cb,
  //                          &state_button[0]);
  iot_button_register_cb(gpio_btn[0], BUTTON_PRESS_DOWN, NULL, button_push_cb,
                         &state_button[0]);

  lv_indev_drv_init(&btn_drv);
  btn_drv.type = LV_INDEV_TYPE_KEYPAD;
  btn_drv.read_cb = lv_button_cb;
  btn_indev = lv_indev_drv_register(&btn_drv);

  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = width;
  disp_drv.ver_res = height;
  disp_drv.flush_cb = flush_cb;
  disp_drv.full_refresh = 1;
  disp_drv.antialiasing = 0;
  disp_drv.draw_buf = &disp_buf;
  lv_disp_drv_register(&disp_drv); 

  ESP_LOGI(TAG, "Install LVGL tick timer");
  esp_timer_create_args_t lvgl_tick_timer_args = {};
  lvgl_tick_timer_args.callback = &Increase_lvgl_tick;
  lvgl_tick_timer_args.name = "lvgl_tick";
  esp_timer_handle_t lvgl_tick_timer = NULL;
  ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
  ESP_ERROR_CHECK(
      esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000));

  xTaskCreatePinnedToCore(Lvgl_port_task, "LVGL", 8 * 1024, NULL, 5, NULL, 0);
}
