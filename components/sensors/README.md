# Sensor integration

This component defines transport-independent measurement types. Implement
hardware drivers under `bmi160`, `gnss`, and `heart_rate` modules and publish
their outputs as `ImuSample`, `GnssFix`, and `HeartRateMeasurement`.

- BMI160 uses the shared board I2C bus and feeds the rowing stroke analyzer.
- GNSS reads a UART byte stream; NMEA and UBX are parser choices for the same
  receiver, not separate navigation sources.
- Heart-rate support consumes the Bluetooth Heart Rate Service and reports
  connection state separately from the latest BPM.
