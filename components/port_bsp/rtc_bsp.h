#ifndef RTC_BSP
#define RTC_BSP

#include "i2c_bsp.h"

typedef struct {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  int week;
} rtcTimeStruct_t;

void Rtc_Setup(I2cMasterBus *i2cbus, uint8_t dev_addr);
void Rtc_SetTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                 uint8_t minute, uint8_t second);
void Rtc_GetTime(rtcTimeStruct_t *time);

#endif