#pragma once

#include <esp_err.h>

class I2cMasterBus;

esp_err_t Board_Init();
I2cMasterBus &Board_GetI2cBus();
