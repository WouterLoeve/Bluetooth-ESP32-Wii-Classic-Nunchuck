#include "gamepad.h"
#include <NimBLEDevice.h>

Gamepad::Gamepad() {
uint8_t tempHidReportDescriptor[150];
int hidReportDescriptorSize = 0;

// USAGE_PAGE (Generic Desktop)
tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

// USAGE (Joystick - 0x04; Gamepad - 0x05; Multi-axis Controller - 0x08)
tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;
tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;

// COLLECTION (Application)
tempHidReportDescriptor[hidReportDescriptorSize++] = 0xa1;
tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_ID (first report, so ID 3)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x85;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x03;

    // USAGE_PAGE (Button)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x05;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x09;

    // USAGE_MINIMUM (Button 1)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x19;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // USAGE_MAXIMUM (Up to 1 buttons possible)            
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x29;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // LOGICAL_MINIMUM (0)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x15;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

    // LOGICAL_MAXIMUM (1)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x25;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_SIZE (a button is 1 or 0 so a 1 bit report size)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_COUNT (# of buttons)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // UNIT_EXPONENT (0)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x55;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

    // UNIT (None)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x65;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x00;

    // INPUT (Data,Var,Abs)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x02;

    // PADDING REPORT
    // REPORT_SIZE (1)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x75;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x01;

    // REPORT_COUNT (# of padding bits)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x95;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x07;

    // INPUT (Const,Var,Abs)
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x81;
    tempHidReportDescriptor[hidReportDescriptorSize++] = 0x03;

// END_COLLECTION (Application)
tempHidReportDescriptor[hidReportDescriptorSize++] = 0xc0;

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

    uint8_t *customHidReportDescriptor = new uint8_t[hidReportDescriptorSize];
    memcpy(customHidReportDescriptor, tempHidReportDescriptor, hidReportDescriptorSize);

    for (uint8_t i = 0; i < hidReportDescriptorSize; i++) {
        ESP_LOGD("GAMEPAD", "%02X ", tempHidReportDescriptor[i]);
    }

    this->hid->reportMap((uint8_t*)customHidReportDescriptor, hidReportDescriptorSize);
    
    this->hid->startServices();

    this->onStarted(this->server);
 
    this->advertising = this->server->getAdvertising();
    this->advertising->setAppearance(HID_GAMEPAD);

    auto uuid = this->hid->hidService()->getUUID();
    ESP_LOGD("GAMEPAD", "uuid: %s", uuid.toString().c_str());
    this->advertising->addServiceUUID(uuid);
    this->advertising->start();
    this->hid->setBatteryLevel(69);

    this->sendReport();

    vTaskDelay(portMAX_DELAY);
}

void Gamepad::sendReport() {
    uint8_t data = 0x00;
    this->input->setValue(&data, sizeof(data));
	this->input->notify();
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