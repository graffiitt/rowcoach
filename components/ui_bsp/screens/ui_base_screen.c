#include "../ui.h"
#include "core/lv_obj.h"
#include "core/lv_obj_pos.h"
#include "core/lv_obj_tree.h"
#include <string.h>

lv_obj_t *ui_base_screen = NULL;
lv_obj_t *ui_time_label = NULL;
lv_obj_t *ui_base_container = NULL;

void ui_base_screen_init(void) {
  ui_base_screen = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_base_screen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

  /*Add time label*/
  ui_time_label = lv_label_create(ui_base_screen);
  lv_label_set_text(ui_time_label, "12:00");
  lv_obj_align(ui_time_label, LV_ALIGN_TOP_MID, 0, 10);

  ui_base_container = lv_obj_create(ui_base_screen);
  lv_obj_clear_flag(ui_base_container, LV_OBJ_FLAG_SCROLLABLE); /// Flags
  lv_obj_set_pos(ui_base_container, 0, 40);
  lv_obj_set_size(ui_base_container, 400, 260);

  
}

void ui_base_screen_destroy(void) {
  if (ui_base_screen) {
    lv_obj_del(ui_base_screen);
  }
}

extern void ui_base_screen_set_widget(lv_obj_t *widget) {
  lv_obj_set_parent(widget, ui_base_container);
}

void ui_title_set_time() {
  
}
