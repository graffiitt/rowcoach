
#ifndef UI_BASE_SCREEN_H
#define UI_BASE_SCREEN_H

#include "core/lv_obj.h"
#ifdef __cplusplus
extern "C" {
#endif

extern lv_obj_t *ui_base_screen;

extern void ui_base_screen_init(void);
extern void ui_base_screen_destroy(void);
extern void ui_base_screen_set_widget(lv_obj_t *widget);

extern void ui_title_set_time();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif