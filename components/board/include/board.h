#pragma once

#include <esp_err.h>

class I2cMasterBus;

void Board_Init();
void Board_sleep();
void Board_wakeup();

I2cMasterBus &Board_GetI2cBus();
