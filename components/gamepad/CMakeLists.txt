idf_component_register(
    SRCS "BleGamepad.cpp" "BleConnectionStatus.cpp"
    INCLUDE_DIRS "include"
    REQUIRES "esp-nimble-cpp"
)