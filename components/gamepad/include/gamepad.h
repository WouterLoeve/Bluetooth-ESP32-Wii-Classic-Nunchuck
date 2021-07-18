#ifndef __GAMEPAD_H__
#define __GAMEPAD_H__

#include <NimBLEServer.h>
#include <NimBLEHIDDevice.h>
#include <NimBLEAdvertising.h>
#include "NimBLECharacteristic.h"

class Gamepad : public NimBLEServerCallbacks {
    private:
    NimBLEServer* server;
    NimBLEHIDDevice* hid;
    NimBLECharacteristic* input;
    NimBLEAdvertising* advertising;
    NimBLESecurity *security;

    public:
    Gamepad();
    void onConnect(NimBLEServer* pServer);
    void onDisconnect(NimBLEServer* pServer);
};

#endif
