# ESP32-LoRa

## How to run
1. Install esp-idf: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-setup.html
2. Clone https://github.com/TaraHoleInIt/tarablessd1306 from components dir
3. Go to settings.h: and uncomment the board definition
4. Execute ```idf.py menuconfig```
5. Go to Component config and edit the lora configuration variables to your heart's content. The configuration we use:
```c
const lmic_pinmap lmic_pins = {
  .nss = 18,
  .rst = 14,
  .dio = { 26, 34, 35 },
  .spi = { /* MISO = */ 19, /* MOSI = */ 27, /* SCK = */ 5 },
  .rxtx = LMIC_UNUSED_PIN,
};
```
6. Execute ```idf.py flash```.
7. Execute ```idf.py monitor```

## Todos
1. Cleanup + improvement LoRa building blocks
    - [x] Message lengths
    - [x] Data format
    - [x] move options to kconfig
2. Message guarantees
    - [x] msg-id
    - [x] ack
    - [x] timeout sending
3. Game
    - [ ] Broadcast for finding people to play with
    - [ ] tic tac toe
    - [ ] message sequence numbers with retrospective resend
4. Screen
5. Gold plating
    - Encryption

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