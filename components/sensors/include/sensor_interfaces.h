#pragma once

#include <stdint.h>

namespace sensors {

struct ImuSample {
  int16_t accel_x_mg;
  int16_t accel_y_mg;
  int16_t accel_z_mg;
  int16_t gyro_x_mdps;
  int16_t gyro_y_mdps;
  int16_t gyro_z_mdps;
  uint64_t timestamp_us;
};

struct GnssFix {
  double latitude_deg;
  double longitude_deg;
  float speed_mps;
  uint8_t satellites;
  bool valid;
  uint64_t timestamp_us;
};

struct HeartRateMeasurement {
  uint16_t bpm;
  bool connected;
  uint64_t timestamp_us;
};

}  // namespace sensors
