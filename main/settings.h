#ifndef SETTINGS_H
#define SETTINGS_H

#include "lmic.h"

// #define WIFI_SSID           CONFIG_ESP_WIFI_SSID
// #define WIFI_PASS           CONFIG_ESP_WIFI_PASSWORD
// #define WIFI_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY
// #define SERVER_HTTP_PORT    CONFIG_ESP_SERVER_HTTP_PORT
// #define SERVER_ADDR         CONFIG_ESP_SERVER_ADDR
// #define DEEPSLEEP_MIN       CONFIG_ESP_DEEPSLEEP_TIME_MINUTES

const lmic_pinmap lmic_pins = {
  .nss = 18,
  .rst = 14,
  .dio = { 26, 34, 35 },
  .spi = { /* MISO = */ 19, /* MOSI = */ 27, /* SCK = */ 5 },
  .rxtx = LMIC_UNUSED_PIN,
};


#endif /* SETTINGS_H */