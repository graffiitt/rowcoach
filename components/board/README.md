# Board support

This component owns board-level peripherals and initializes the display, LVGL
port, and shared I2C bus. Sensor components use `Board_GetI2cBus()` to share
the I2C controller instead of creating their own bus.
