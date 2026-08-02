idf_component_register(
    SRCS 
    "display_bsp.cpp" 
    PRIV_REQUIRES 
    driver
    esp_lcd
    INCLUDE_DIRS 
    "./")
