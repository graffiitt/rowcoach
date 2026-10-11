#include "workout_controller.h"

void WorkoutController::Start(uint64_t now_us) {
  if (status_ == WorkoutStatus::Idle) {
    started_at_us_ = now_us;
    accumulated_us_ = 0;
    status_ = WorkoutStatus::Running;
  }
}

void WorkoutController::Pause(uint64_t now_us) {
  if (status_ == WorkoutStatus::Running) {
    accumulated_us_ += now_us - started_at_us_;
    status_ = WorkoutStatus::Paused;
  }
}

void WorkoutController::Resume(uint64_t now_us) {
  if (status_ == WorkoutStatus::Paused) {
    started_at_us_ = now_us;
    status_ = WorkoutStatus::Running;
  }
}

void WorkoutController::Reset() {
  status_ = WorkoutStatus::Idle;
  started_at_us_ = 0;
  accumulated_us_ = 0;
}

WorkoutStatus WorkoutController::Status() const { return status_; }

uint32_t WorkoutController::ElapsedSeconds(uint64_t now_us) const {
  uint64_t elapsed_us = accumulated_us_;
  if (status_ == WorkoutStatus::Running) {
    elapsed_us += now_us - started_at_us_;
  }
  return static_cast<uint32_t>(elapsed_us / 1000000);
}
