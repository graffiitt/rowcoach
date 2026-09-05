#include "../ui.h"
#include "core/lv_disp.h"
#include "core/lv_obj.h"
#include <string.h>

lv_obj_t *ui_start_menu = NULL;

extern void ui_start_menu_init(void) {
  ui_start_menu = lv_obj_create(lv_scr_act());
  lv_obj_set_pos(ui_start_menu, 0, 40);
  lv_obj_set_size(ui_start_menu, 20, 10);
}

extern void ui_start_menu_destroy(void) {
  if (ui_start_menu)
    lv_obj_del(ui_start_menu);
}
