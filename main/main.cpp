#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "BleGamepad.h"

extern "C" {
	void app_main();
	#include "wii_i2c.h"
}

#define PIN_SDA  32
#define PIN_SCL  33
#define WII_I2C_PORT 0

unsigned int controller_type = 0;


void show_nunchuk(const unsigned char *data)
{
  wii_i2c_nunchuk_state state;
  wii_i2c_decode_nunchuk(data, &state);
        
  ESP_LOGD("main", "a = (%5d,%5d,%5d)\n", state.acc_x, state.acc_y, state.acc_z);
  ESP_LOGD("main", "d = (%5d,%5d)\n", state.x, state.y);
  ESP_LOGD("main", "c=%d, z=%d\n", state.c, state.z);
}

void show_classic(const unsigned char *data)
{
  wii_i2c_classic_state state;
  wii_i2c_decode_classic(data, &state);

  ESP_LOGD("main", "lx,ly = (%3d,%3d)\n", state.lx, state.ly);
  ESP_LOGD("main", "rx,ry = (%3d,%3d)\n", state.rx, state.ry);
  ESP_LOGD("main", "a lt,rt = (%3d,%3d)\n", state.a_lt, state.a_rt);
  ESP_LOGD("main", "d lt,rt = (%d,%d)\n", state.d_lt, state.d_rt);
  ESP_LOGD("main", "a,b,x,y = (%d,%d,%d,%d)\n", state.a, state.b, state.x, state.y);
  ESP_LOGD("main", "up, down, left, right = (%d,%d,%d,%d)\n", state.up, state.down, state.left, state.right);
  ESP_LOGD("main", "home, plus, minus = (%d,%d,%d)\n", state.home, state.plus, state.minus);
  ESP_LOGD("main", "zl, zr = (%d,%d)\n", state.zl, state.zr);
}

void setup()
{
//   Serial.begin(115200);
  ESP_LOGD("main", "Starting...\n");

  if (wii_i2c_init(WII_I2C_PORT, PIN_SDA, PIN_SCL) != 0) {
    ESP_LOGD("main", "ERROR initializing wii i2c controller\n");
    return;
  }
  const unsigned char *ident = wii_i2c_read_ident();
  if (! ident) {
    ESP_LOGD("main", "no ident :(\n");
    return;
  }
  
  controller_type = wii_i2c_decode_ident(ident);
  switch (controller_type) {
  case WII_I2C_IDENT_NUNCHUK: ESP_LOGD("main", "-> nunchuk detected\n"); break;
  case WII_I2C_IDENT_CLASSIC: ESP_LOGD("main", "-> classic controller detected\n"); break;
  default:                    ESP_LOGD("main", "-> unknown controller detected: 0x%06x\n", controller_type); break;
  }
  wii_i2c_request_state();
}

void loop() {
  const unsigned char *data = wii_i2c_read_state();
  wii_i2c_request_state();
  if (data) {
    switch (controller_type) {
		case WII_I2C_IDENT_NUNCHUK: show_nunchuk(data); break;
		case WII_I2C_IDENT_CLASSIC: show_classic(data); break;
		default:
		ESP_LOGD("main", "data: %02x %02x %02x %02x %02x %02x\n",
						data[0], data[1], data[2], data[3], data[4], data[5]);
		break;
    }
  } else {
    ESP_LOGD("main", "no data :(\n");
  }
  
  vTaskDelay(pdMS_TO_TICKS(100));
}


void app_main() {
	setup();
	BleGamepad *gp = new BleGamepad();
	gp->begin(16, 1, true, true, true, true, false, false, false, false, false, false, false, false, false);
	while(true) {
		loop();
		// // (x, y, x2, y2, ?, ?, ?, ?, DPAD)
		// gp->setAxes(32767, 0, 32767/2, 32767/4, 0, 0, 0, 0, DPAD_UP_RIGHT);
		
		// // vTaskDelay(pdMS_TO_TICKS(5000));
		// // gp->setAxes(0, 32767, 0, 0, 0, 32767, 0, 0, DPAD_RIGHT);

		// vTaskDelay(pdMS_TO_TICKS(5000));
		// gp->setAxes(0, 0, 0, 0, 0, 0, 0, 0, DPAD_CENTERED);

		// //gp->press(BUTTON_5);
		// //gp->press(BUTTON_16);

		// //gp->release(BUTTON_5);
	}
}
