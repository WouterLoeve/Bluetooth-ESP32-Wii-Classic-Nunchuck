# ESP32-LoRa

## How to run
1. Install esp-idf: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-setup.html
2. Go to settings.h: and uncomment the board definition
3. Execute ```idf.py menuconfig```
4. Go to Component config and edit the lora configuration variables to your heart's content. The configuration we use:
```c
const lmic_pinmap lmic_pins = {
  .nss = 18,
  .rst = 14,
  .dio = { 26, 34, 35 },
  .spi = { /* MISO = */ 19, /* MOSI = */ 27, /* SCK = */ 5 },
  .rxtx = LMIC_UNUSED_PIN,
};
```
5. Execute ```idf.py flash```.
6. Execute ```idf.py monitor```

## Todos
1. Cleanup + improvement LoRa building blocks
    - Message lengths
    - Data format
    - move options to kconfig
2. Message guarantees
    - msg-id
    - ack
    - timeout sending
3. Game
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
├── components/lora            LoRa module
├── main
│   ├── CMakeLists.txt         Add any new source files here
│   └── main.c                 HTTP 
└── README.md                  This is the file you are currently reading
```
