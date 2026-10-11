#pragma once

class I2cMasterBus;

void UserApp_UiInit();
void UserApp_AppInit(I2cMasterBus *i2c_bus);
void UserApp_TaskInit();
