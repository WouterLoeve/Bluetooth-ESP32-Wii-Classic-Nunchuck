# ESP32-BCC

## How to run
1. Install esp-idf: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-setup.html
2. Clone git@github.com:h2zero/esp-nimble-cpp.git from components dir
3. Go to settings.h: and uncomment the board definition
4. Execute ```idf.py menuconfig```
5. Go to Component config->Bluetooth enable Bluetooth and in Bluetooth host NimBLE.


## Folder contents

The project **ESP32-LoRa** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── components
│   ├── /lora            LoRa module
│   ├── /lwr             LoRaWrapper library
│   └── /tarablessd1306  OLed drawing lib: https://github.com/TaraHoleInIt/tarablessd1306
├── main
│   ├── CMakeLists.txt         Add any new source files here
│   └── main.c                 HTTP 
└── README.md                  This is the file you are currently reading
```

Oled drawing Wiki: https://github.com/TaraHoleInIt/tarablessd1306/wiki/Drawing
