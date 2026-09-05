#pragma once

#include "lvgl.h"

#define LVGL_TICK_PERIOD_MS 5
#define LVGL_TASK_MAX_DELAY_MS 10

typedef void (*DispFlushCb)(struct _lv_disp_drv_t *disp_drv,
                            const lv_area_t *area, lv_color_t *color_p);

extern lv_indev_t *btn_indev;

void Lvgl_PortInit(int width, int height, DispFlushCb flush_cb);
bool Lvgl_lock(int timeout_ms);
void Lvgl_unlock(void);