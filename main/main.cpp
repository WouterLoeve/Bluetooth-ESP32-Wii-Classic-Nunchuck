#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "BleGamepad.h"

extern "C"
{
	void app_main();
#include "wii_i2c.h"
}

#define PIN_SDA 32
#define PIN_SCL 33
#define WII_I2C_PORT 0

unsigned int controller_type = 0;
BleGamepad *gp;

void show_nunchuk(const unsigned char *data)
{
	wii_i2c_nunchuk_state state;
	wii_i2c_decode_nunchuk(data, &state);

	ESP_LOGI("main", "a = (%5d,%5d,%5d)\n", state.acc_x, state.acc_y, state.acc_z);
	ESP_LOGI("main", "d = (%5d,%5d)\n", state.x, state.y);
	ESP_LOGI("main", "c=%d, z=%d\n", state.c, state.z);
}

int scaleInput(int value, float max_val, int max_output) {
	return value / max_val * max_output;
}

void show_classic(const unsigned char *data)
{
	wii_i2c_classic_state state;
	wii_i2c_decode_classic(data, &state);

	// ESP_LOGI("main", "lx,ly = (%3d,%3d)\n", state.lx, state.ly);
	// ESP_LOGI("main", "rx,ry = (%3d,%3d)\n", state.rx, state.ry);
	// ESP_LOGI("main", "a lt,rt = (%3d,%3d)\n", state.a_lt, state.a_rt);
	// ESP_LOGI("main", "d lt,rt = (%d,%d)\n", state.d_lt, state.d_rt);
	// ESP_LOGI("main", "a,b,x,y = (%d,%d,%d,%d)\n", state.a, state.b, state.x, state.y);
	// ESP_LOGI("main", "up, down, left, right = (%d,%d,%d,%d)\n", state.up, state.down, state.left, state.right);
	// ESP_LOGI("main", "home, plus, minus = (%d,%d,%d)\n", state.home, state.plus, state.minus);
	// ESP_LOGI("main", "zl, zr = (%d,%d)\n", state.zl, state.zr);

	// jstest-gtk doesnt recognize this well but this should be the same as the xbox 360 controller....
	int lefttrigger = -32767;
	if (state.zl) {
		lefttrigger = 32767;
	}
	int righttrigger = -32767;
	if (state.zr) {
		righttrigger = 32767;
	}
	
	// // (x, y, x2, y2, ?, ?, ?, ?, DPAD)
	gp->setAxes(
		scaleInput(state.lx, 28.0, 32767), 
		-1 * scaleInput(state.ly, 28.0, 32767), 
		scaleInput(state.rx, 14.0, 32767), 
		-1 * scaleInput(state.ry, 14.0, 32767), 
		lefttrigger, righttrigger, 
		0, 0, 0
	);

	(state.b) ? gp->press(BUTTON_1) : gp->release(BUTTON_1);
	(state.a) ? gp->press(BUTTON_2) : gp->release(BUTTON_2);
	(state.y) ? gp->press(BUTTON_3) : gp->release(BUTTON_3);
	(state.x) ? gp->press(BUTTON_4) : gp->release(BUTTON_4);
	(state.d_lt) ? gp->press(BUTTON_5) : gp->release(BUTTON_5);
	(state.d_rt) ? gp->press(BUTTON_6) : gp->release(BUTTON_6);
	(state.minus) ? gp->press(BUTTON_7) : gp->release(BUTTON_7);
	(state.plus) ? gp->press(BUTTON_8) : gp->release(BUTTON_8);
	(state.minus && state.a) ? gp->press(BUTTON_9) : gp->release(BUTTON_9);
	(state.plus && state.a) ? gp->press(BUTTON_10) : gp->release(BUTTON_10);
	// TODO: dpad

	gp->sendReport();
}


void setup()
{
	ESP_LOGI("main", "Starting...\n");

	if (wii_i2c_init(WII_I2C_PORT, PIN_SDA, PIN_SCL) != 0)
	{
		ESP_LOGI("main", "ERROR initializing wii i2c controller\n");
		return;
	}
	const unsigned char *ident = wii_i2c_read_ident();
	if (!ident)
	{
		ESP_LOGI("main", "no ident :(\n");
		return;
	}

	controller_type = wii_i2c_decode_ident(ident);
	switch (controller_type)
	{
	case WII_I2C_IDENT_NUNCHUK:
		ESP_LOGI("main", "-> nunchuk detected\n");
		break;
	case WII_I2C_IDENT_CLASSIC:
		ESP_LOGI("main", "-> classic controller detected\n");
		break;
	default:
		ESP_LOGI("main", "-> unknown controller detected: 0x%06x\n", controller_type);
		break;
	}
	wii_i2c_request_state();
}

void loop()
{
	const unsigned char *data = wii_i2c_read_state();
	wii_i2c_request_state();
	show_classic(data);
	// if (data)
	// {
	// 	switch (controller_type)
	// 	{
	// 	case WII_I2C_IDENT_NUNCHUK:
	// 		show_nunchuk(data);
	// 		break;
	// 	case WII_I2C_IDENT_CLASSIC:
	// 		show_classic(data);
	// 		break;
	// 	default:
	// 		ESP_LOGI("main", "data: %02x %02x %02x %02x %02x %02x\n",
	// 						data[0], data[1], data[2], data[3], data[4], data[5]);
	// 		break;
	// 	}
	// }
	// else
	// {
	// 	ESP_LOGI("main", "no data :(\n");
	// }

	vTaskDelay(pdMS_TO_TICKS(100));
}

void app_main()
{
	setup();
	gp = new BleGamepad();
	gp->begin(11, 1, true, true, true, true, true, true, false, false, false, false, false, false, false);
	gp->setAutoReport(false);
	while (true)
	{
		loop();
	}
}
