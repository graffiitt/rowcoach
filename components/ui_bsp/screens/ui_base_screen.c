#include "../ui.h"

lv_obj_t *ui_base_screen = NULL;

lv_obj_t *ui_time_label = NULL;
lv_obj_t *ui_line_top = NULL;
lv_obj_t *ui_base_container = NULL;

void ui_base_screen_init(void) {
  ui_base_screen = lv_obj_create(NULL);

  /*Add time label*/
  ui_time_label = lv_label_create(ui_base_screen);
  lv_label_set_text(ui_time_label, "12:00");
  lv_obj_align(ui_time_label, LV_ALIGN_TOP_MID, 0, 10);
  lv_obj_set_style_text_font(ui_time_label, &ui_font_NUMERIC1, LV_PART_MAIN | LV_STATE_DEFAULT);

  /*Create line style*/
  static lv_style_t style_line;
  lv_style_init(&style_line);
  lv_style_set_line_width(&style_line, 4);
  lv_style_set_line_color(&style_line, lv_color_hex(0000));

  ui_line_top = lv_line_create(ui_base_screen);
  static const lv_point_t points[] = {{0, 40}, {400, 40}};
  lv_line_set_points(ui_line_top, points, 2);
  lv_obj_add_style(ui_line_top, &style_line, 0);
}

void ui_base_screen_destroy(void) {
  if (ui_base_screen)
    lv_obj_del(ui_base_screen);
}
