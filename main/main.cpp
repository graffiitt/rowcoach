#include <esp_err.h>

#include "app_controller.h"
#include "board.h"

extern "C" void app_main(void) {
  Board_Init();
  AppController_Init();
  ESP_ERROR_CHECK(AppController_Start());
}
