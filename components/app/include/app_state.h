#pragma once

#include <stdint.h>

struct AppState {
  uint64_t updated_at_us = 0;

  float stroke_rate_spm = 0.0f;
  float speed_mps = 0.0f;
  float distance_m = 0.0f;
  uint32_t workout_time_s = 0;
  uint16_t heart_rate_bpm = 0;

  bool imu_valid = false;
  bool gnss_fix_valid = false;
  bool heart_rate_connected = false;
  bool workout_active = false;
};
