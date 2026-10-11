#pragma once

#include <stdint.h>

enum class WorkoutStatus : uint8_t {
  Idle,
  Running,
  Paused,
};

class WorkoutController {
 public:
  void Start(uint64_t now_us);
  void Pause(uint64_t now_us);
  void Resume(uint64_t now_us);
  void Reset();

  WorkoutStatus Status() const;
  uint32_t ElapsedSeconds(uint64_t now_us) const;

 private:
  WorkoutStatus status_ = WorkoutStatus::Idle;
  uint64_t started_at_us_ = 0;
  uint64_t accumulated_us_ = 0;
};
