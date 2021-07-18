#include "gamepad.h"
#include <NimBLEDevice.h>

Gamepad::Gamepad() {
    BLEDevice::init("ESP32 Gamepad");

    this->server = BLEDevice::createServer();
    this->server->setCallbacks(this);

    this->hid = new NimBLEHIDDevice(this->server);
    this->input = this->hid->inputReport(3);
    this->hid->manufacturer()->setValue("WiP Projects");
    this->hid->pnp(0x01,0x02e5,0xabbb,0x0110);
    this->hid->hidInfo(0x00,0x01);

    this->security = new NimBLESecurity();
    this->security->setAuthenticationMode(ESP_LE_AUTH_BOND);

    // uint8_t *customHidReportDescriptor = new uint8_t[hidReportDescriptorSize];
    // memcpy(customHidReportDescriptor, tempHidReportDescriptor, hidReportDescriptorSize);

    // BleGamepadInstance->hid->reportMap((uint8_t*)customHidReportDescriptor, hidReportDescriptorSize);
    this->hid->startServices();
 
    this->advertising = this->server->getAdvertising();
    this->advertising->setAppearance(HID_GAMEPAD);

    auto uuid = this->hid->hidService()->getUUID();
    ESP_LOGD("GAMEPAD", "uuid: %s", uuid);
    this->advertising->addServiceUUID(uuid);
    this->advertising->start();
    this->hid->setBatteryLevel(69);
    vTaskDelay(portMAX_DELAY);
}

void Gamepad::onConnect(NimBLEServer* server) {
    ESP_LOGD("GAMEPAD", "OnConnect");
}

void Gamepad::onDisconnect(NimBLEServer* server) {
    ESP_LOGD("GAMEPAD", "OnDisconnect");
}

//  BLEService *pService = pServer->createService();
//   BLECharacteristic *pCharacteristic = pService->createCharacteristic(
//                                         CHARACTERISTIC_UUID,
//                                   /***** Enum Type NIMBLE_PROPERTY now *****      
//                                         BLECharacteristic::PROPERTY_READ   |
//                                         BLECharacteristic::PROPERTY_WRITE  
//                                         );
//                                   *****************************************/
//                                         NIMBLE_PROPERTY::READ |
//                                         NIMBLE_PROPERTY::WRITE 
//                                        );

  //pCharacteristic->setValue("Hello World says Neil");
  //pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  //BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  //pAdvertising->addServiceUUID(SERVICE_UUID);
  //pAdvertising->setScanResponse(true);
  
  /** These methods have been removed **
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  */
  
  
//   printf("Characteristic defined! Now you can read it in your phone!\n");
// }