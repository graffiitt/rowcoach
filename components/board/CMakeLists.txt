idf_component_register(
    SRCS "board.cpp"
    PRIV_REQUIRES lvgl_port port_bsp
    INCLUDE_DIRS "include")
