#include <esp_err.h>

#include "app_controller.h"
#include "board.h"

extern "C" void app_main(void) {
  ESP_ERROR_CHECK(Board_Init());
  ESP_ERROR_CHECK(AppController_Init());
  ESP_ERROR_CHECK(AppController_Start());
}
