# Component architecture

```text
main
  ├── board
  └── app

components/
  ├── board       Board pins and peripheral composition
  ├── app         Application lifecycle and shared AppState
  ├── sensors     BMI160, GNSS UART/parsers, BLE heart rate
  ├── rowing      Stroke/navigation metrics and workout state
  ├── port_bsp    Peripheral drivers
  ├── lvgl_port   LVGL hardware port
  └── ui_bsp      LVGL screens

board ──> port_bsp + lvgl_port
app   ──> board + port_bsp + lvgl_port + ui_bsp
```

`port_bsp` contains peripheral drivers, `lvgl_port` binds LVGL to the display
and input hardware, and `ui_bsp` contains screen definitions. `main` only
composes the board and application entry points.

The current implementation wires the display, shared I2C bus, PCF85063 clock,
and existing UI. Measurement types and workout timer scaffolding are in place;
BMI160, GNSS, BLE heart-rate, and stroke/navigation algorithms remain to be
implemented and validated against the selected hardware.
