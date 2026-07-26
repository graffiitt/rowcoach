#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <esp_log.h>

#include "user_app.h"
#include "ui.h" 

void Lvgl_LoopTask(void *arg) {
    for(;;) {
      
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}


void UserApp_AppInit() {
    
}

void UserApp_UiInit() {
    ui_init();
}

void UserApp_TaskInit() {
    xTaskCreatePinnedToCore(Lvgl_LoopTask, "Lvgl_LoopTask", 4 * 1024, NULL, 2, NULL,1);
}
