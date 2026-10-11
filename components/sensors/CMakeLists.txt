idf_component_register(
    SRCS 
    "sensor_interfaces.cpp"     
    "rtc_bsp.cpp"
    INCLUDE_DIRS "include"
    REQUIRES
    driver
    esp_lcd
    port_bsp
    )
