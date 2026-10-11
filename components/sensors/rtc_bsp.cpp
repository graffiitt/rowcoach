#include "rtc_bsp.h"

#include <cstdio>
#include <cstring>
#include <driver/i2c_master.h>

namespace {

constexpr uint8_t kControl1Register = 0x00;
constexpr uint8_t kSecondsRegister = 0x04;
constexpr uint8_t kStopBit = 1 << 5;
constexpr uint8_t kDateTimeRegisterCount = 7;

I2cMasterBus *rtc_bus = nullptr;
i2c_master_dev_handle_t rtc_device = nullptr;

uint8_t ToBcd(uint8_t value) {
  return static_cast<uint8_t>(((value / 10) << 4) | (value % 10));
}

bool FromBcd(uint8_t value, uint8_t mask, int *decoded) {
  value &= mask;
  const uint8_t units = value & 0x0f;
  const uint8_t tens = (value >> 4) & 0x0f;
  if (units > 9 || tens > 9) {
    return false;
  }
  *decoded = tens * 10 + units;
  return true;
}

esp_err_t WriteDateTime(uint8_t *date_time) {
  if (rtc_bus == nullptr || rtc_device == nullptr) {
    return ESP_ERR_INVALID_STATE;
  }
  return static_cast<esp_err_t>(rtc_bus->i2c_write_buff(
      rtc_device, kSecondsRegister, date_time, kDateTimeRegisterCount));
}

esp_err_t SetBuildTime() {
  static constexpr const char *kMonths[] = {"Jan", "Feb", "Mar", "Apr",
                                             "May", "Jun", "Jul", "Aug",
                                             "Sep", "Oct", "Nov", "Dec"};
  char month_name[4] = {};
  int day = 0;
  int year = 0;
  int hour = 0;
  int minute = 0;
  int second = 0;
  if (std::sscanf(__DATE__, "%3s %d %d", month_name, &day, &year) != 3 ||
      std::sscanf(__TIME__, "%d:%d:%d", &hour, &minute, &second) != 3) {
    return ESP_ERR_INVALID_RESPONSE;
  }

  int month = 0;
  for (int i = 0; i < 12; ++i) {
    if (std::strncmp(month_name, kMonths[i], 3) == 0) {
      month = i + 1;
      break;
    }
  }
  if (month == 0) {
    return ESP_ERR_INVALID_RESPONSE;
  }

  return Rtc_SetTime(static_cast<uint16_t>(year), static_cast<uint8_t>(month),
                     static_cast<uint8_t>(day), static_cast<uint8_t>(hour),
                     static_cast<uint8_t>(minute),
                     static_cast<uint8_t>(second));
}

} // namespace


esp_err_t Rtc_Setup(I2cMasterBus *i2cbus, uint8_t dev_addr) {
  if (i2cbus == nullptr) {
    return ESP_ERR_INVALID_ARG;
  }
  if (rtc_device != nullptr) {
    return ESP_ERR_INVALID_STATE;
  }

  i2c_device_config_t device_config = {};
  device_config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
  device_config.device_address = dev_addr;
  device_config.scl_speed_hz = 100000;
  esp_err_t err = i2c_master_bus_add_device(i2cbus->Get_I2cBusHandle(),
                                            &device_config, &rtc_device);
  if (err != ESP_OK) {
    return err;
  }

  rtc_bus = i2cbus;
  uint8_t control1 = 0;
  err = static_cast<esp_err_t>(
      rtc_bus->i2c_read_buff(rtc_device, kControl1Register, &control1, 1));
  if (err != ESP_OK) {
    return err;
  }

  if ((control1 & kStopBit) != 0) {
    control1 &= static_cast<uint8_t>(~kStopBit);
    err = static_cast<esp_err_t>(
        rtc_bus->i2c_write_buff(rtc_device, kControl1Register, &control1, 1));
  }

  if (err != ESP_OK) {
    return err;
  }

  rtcTimeStruct_t time = {};
  if (Rtc_GetTime(&time) == ESP_OK) {
    return ESP_OK;
  }
  return SetBuildTime();
}

esp_err_t Rtc_SetTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                      uint8_t minute, uint8_t second) {
  if (year < 2000 || year > 2099 || month < 1 || month > 12 || day < 1 ||
      day > 31 || hour > 23 || minute > 59 || second > 59) {
    return ESP_ERR_INVALID_ARG;
  }

  uint8_t date_time[kDateTimeRegisterCount] = {
      ToBcd(second),
      ToBcd(minute),
      ToBcd(hour),
      ToBcd(day),
      0,
      ToBcd(month),
      ToBcd(static_cast<uint8_t>(year % 100))};
  return WriteDateTime(date_time);
}

esp_err_t Rtc_GetTime(rtcTimeStruct_t *time) {
  if (time == nullptr) {
    return ESP_ERR_INVALID_ARG;
  }
  if (rtc_bus == nullptr || rtc_device == nullptr) {
    return ESP_ERR_INVALID_STATE;
  }

  uint8_t date_time[kDateTimeRegisterCount] = {};
  esp_err_t err = static_cast<esp_err_t>(rtc_bus->i2c_read_buff(
      rtc_device, kSecondsRegister, date_time, kDateTimeRegisterCount));
  if (err != ESP_OK) {
    return err;
  }
  if ((date_time[0] & 0x80) != 0) {
    return ESP_ERR_INVALID_STATE;
  }

  rtcTimeStruct_t decoded = {};
  if (!FromBcd(date_time[0], 0x7f, &decoded.second) ||
      !FromBcd(date_time[1], 0x7f, &decoded.minute) ||
      !FromBcd(date_time[2], 0x3f, &decoded.hour) ||
      !FromBcd(date_time[3], 0x3f, &decoded.day) ||
      !FromBcd(date_time[4], 0x07, &decoded.week) ||
      !FromBcd(date_time[5], 0x1f, &decoded.month)) {
    return ESP_ERR_INVALID_RESPONSE;
  }

  int year = 0;
  if (!FromBcd(date_time[6], 0xff, &year) || decoded.second > 59 ||
      decoded.minute > 59 || decoded.hour > 23 || decoded.day < 1 ||
      decoded.day > 31 || decoded.week > 6 || decoded.month < 1 ||
      decoded.month > 12) {
    return ESP_ERR_INVALID_RESPONSE;
  }

  decoded.year = 2000 + year;
  *time = decoded;
  return ESP_OK;
}
