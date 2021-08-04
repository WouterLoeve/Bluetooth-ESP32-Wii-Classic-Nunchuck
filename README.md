# Bluetooth-ESP32-Wii-Classic/Nunchuck 
ESP32 Library which uses a [wii nunchuck/classic controller breakout adapter](https://www.aliexpress.com/item/32666887418.html) to read out the inputs from a nunchuck or wii classic controller and outputs them via bluetooth to another device. For this we use three libraries:
1. [esp32-wii-nunchuk](https://github.com/moefh/esp32-wii-nunchuk): Used to communicate with the wii controllers
2. [esp32-BLE-gamepad](https://github.com/lemmingDev/ESP32-BLE-Gamepad): Used to connect with a host via bluetooth and send input as HID device
3. [esp-nimble-cpp](https://github.com/h2zero/esp-nimble-cpp): Dependency of 2.
The first two libraries were originally meant for arduino but where converted to work with the ESP-IDF environment.

## How to run
1. Install esp-idf: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-setup.html
2. Clone ```git@github.com:h2zero/esp-nimble-cpp.git``` from components dir (This should be a submodule....)
3. Go to settings.h: and uncomment the board definition
4. Execute ```idf.py menuconfig```
5. Go to Component config->Bluetooth enable Bluetooth and in Bluetooth host NimBLE.


## Folder contents

The project **ESP32-Wii-Classic** contains one source file in C++ [main.c](main/main.cpp). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── components
│   ├── /esp32-wii-nunchuck Fork of nunchuck/classic library: https://github.com/moefh/esp32-wii-nunchuk
│   ├── /esp-nimble-cpp  esp nimble BLE library
│   └── /BleGamepad      Fork of Gamepad lib: https://github.com/lemmingDev/ESP32-BLE-Gamepad
├── main
│   ├── CMakeLists.txt         Add any new source files here
│   └── main.cpp                 
└── README.md                  This is the file you are currently reading
```
