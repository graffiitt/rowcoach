idf_component_register(
    SRCS "app_controller.cpp"
    PRIV_REQUIRES board lvgl_port port_bsp ui_bsp sensors
    INCLUDE_DIRS "include")
