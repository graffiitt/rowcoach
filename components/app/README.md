# Application layer

`AppState` is the shared snapshot passed from sensor/workout logic toward the
UI. Sensor tasks should update their own measurements; the UI must only be
updated while holding the LVGL mutex.

`AppController` currently preserves the existing startup-screen transition and
PCF85063 clock display. BMI160, GNSS, BLE heart-rate, and workout services can
publish their measurements into `AppState` as they are implemented.
