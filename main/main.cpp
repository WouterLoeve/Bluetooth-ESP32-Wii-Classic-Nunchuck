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
wii_i2c_classic_state *classic_state = NULL;
wii_i2c_nunchuk_state *nunchuk_state = NULL;

/*
 * Scales joystick input as received from controller to a 
 * 		value range that can be understood by the BleGamePad library.
 */
int scaleInput(int value, float max_val, int max_output)
{
	return value / max_val * max_output;
}

/*
 * Converts the dpad received from the nunchuk controller 
 * 	to a format that can be used by the BleGamePad library.
 * Here we (for now) ignore the accelerometer data.
 * We treat x and z as being a and b so that it can be used for 
 * 		games that only use two buttons and an axis (e.g. NES)
 * 
 * @param state: state as received from the nunchuk controller
 * @param dpad: resulting direction that can be read by BLeGamePad library.
 */
void show_nunchuk(const unsigned char *data)
{
	wii_i2c_decode_nunchuk(data, nunchuk_state);

	ESP_LOGD("main", "a = (%5d,%5d,%5d)\n", nunchuk_state->acc_x, nunchuk_state->acc_y, nunchuk_state->acc_z);
	ESP_LOGD("main", "d = (%5d,%5d)\n", nunchuk_state->x, nunchuk_state->y);
	ESP_LOGD("main", "c=%d, z=%d\n", nunchuk_state->c, nunchuk_state->z);

	gp->setAxes(
		scaleInput(nunchuk_state->x, 100, 32767),		//x
		-1 * scaleInput(nunchuk_state->y, 100, 32767)  //y
	);

	(nunchuk_state->z) 		? gp->press(BUTTON_1) : gp->release(BUTTON_1);
	(nunchuk_state->c) 		? gp->press(BUTTON_2) : gp->release(BUTTON_2);

	gp->sendReport();
}


/*
 * Converts the dpad received from the classic controller 
 * 	to a format that can be used by the BleGamePad library.
 * We try to stay as close to an xbox controller as possible in the way we sent the inputs.
 * 
 * @param state: state as received from the classic controller
 * @param dpad: resulting direction that can be read by BLeGamePad library.
 */
void get_dpad(wii_i2c_classic_state *state, unsigned char *dpad)
{
	if (state->up)
	{
		*dpad = DPAD_UP;
		if (state->left)
		{
			*dpad = DPAD_UP_LEFT;
		}
		else if (state->right)
		{
			*dpad = DPAD_UP_RIGHT;
		}
		else if (state->down)
		{
			*dpad = DPAD_CENTERED;
		}
	}
	else if (state->down)
	{
		*dpad = DPAD_DOWN;
		if (state->left)
		{
			*dpad = DPAD_DOWN_LEFT;
		}
		else if (state->right)
		{
			*dpad = DPAD_DOWN_RIGHT;
		}
	}
	else if (state->left)
	{
		*dpad = DPAD_LEFT;
	}
	else if (state->right)
	{
		*dpad = DPAD_RIGHT;
	}
}

/*
 * Decodes the inputs from the classic controller,
 * Converts the data to a format readable by the BleGamePad library
 * Sends report via bluetooth to the target.
 * 
 * @param: data as received from classic controller
 */
void show_classic(const unsigned char *data)
{
	wii_i2c_decode_classic(data, classic_state);

	ESP_LOGD("main", "lx,ly = (%3d,%3d)\n", classic_state->lx, classic_state->ly);
	ESP_LOGD("main", "rx,ry = (%3d,%3d)\n", classic_state->rx, classic_state->ry);
	ESP_LOGD("main", "a lt,rt = (%3d,%3d)\n", classic_state->a_lt, classic_state->a_rt);
	ESP_LOGD("main", "d lt,rt = (%d,%d)\n", classic_state->d_lt, classic_state->d_rt);
	ESP_LOGD("main", "a,b,x,y = (%d,%d,%d,%d)\n", classic_state->a, classic_state->b, classic_state->x, classic_state->y);
	ESP_LOGD("main", "home, plus, minus = (%d,%d,%d)\n", classic_state->home, classic_state->plus, classic_state->minus);
	ESP_LOGD("main", "zl, zr = (%d,%d)\n", classic_state->zl, classic_state->zr);

	unsigned char dpad = DPAD_CENTERED;
	if (classic_state->up || classic_state->left || classic_state->right || classic_state->down)
	{
		get_dpad(classic_state, &dpad);
	}

	// jstest-gtk doesnt recognize this well but this should be the same as the xbox 360 controller....
	int lefttrigger = -32767;
	if (classic_state->zl)
	{
		lefttrigger = 32767;
	}
	int righttrigger = -32767;
	if (classic_state->zr)
	{
		righttrigger = 32767;
	}

	gp->setAxes(
		scaleInput(classic_state->lx, 28.0, 32767),		//x
		-1 * scaleInput(classic_state->ly, 28.0, 32767), //y
		scaleInput(classic_state->rx, 14.0, 32767),		//z
		-1 * scaleInput(classic_state->ry, 14.0, 32767), //rz
		lefttrigger,							//rX
		righttrigger,							//rY
		0,										//slider1
		0,										//slider2
		dpad									// hat1
	);

	(classic_state->b) 		? gp->press(BUTTON_1) : gp->release(BUTTON_1);
	(classic_state->a) 		? gp->press(BUTTON_2) : gp->release(BUTTON_2);
	(classic_state->y) 		? gp->press(BUTTON_3) : gp->release(BUTTON_3);
	(classic_state->x) 		? gp->press(BUTTON_4) : gp->release(BUTTON_4);
	(classic_state->d_lt)	? gp->press(BUTTON_5) : gp->release(BUTTON_5);
	(classic_state->d_rt) 	? gp->press(BUTTON_6) : gp->release(BUTTON_6);
	(classic_state->minus) 	? gp->press(BUTTON_7) : gp->release(BUTTON_7);
	(classic_state->plus) 	? gp->press(BUTTON_8) : gp->release(BUTTON_8);
	(classic_state->home) 	? gp->press(BUTTON_9) : gp->release(BUTTON_9);

	gp->sendReport();
}

/*
 * Setup for BleGamepad and library for reading out nunchuk and classic controller.
 */
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
		gp = new BleGamepad("Wii Nunchuck", "Nintendo", 100);
		gp->setAutoReport(false);
		gp->begin(2, 1, true, true, false, false, false, false, false, false, false, false, false, false, false);
		nunchuk_state = (wii_i2c_nunchuk_state*) malloc(sizeof(wii_i2c_nunchuk_state));
		assert(nunchuk_state);
		ESP_LOGI("main", "-> nunchuk detected\n");
		break;
	case WII_I2C_IDENT_CLASSIC:
		gp = new BleGamepad("Wii Classic Bluetooth", "Nintendo", 100);
		gp->setAutoReport(false);
		gp->begin(9, 1, true, true, true, true, true, true, false, false, false, false, false, false, false);
		classic_state = (wii_i2c_classic_state*) malloc(sizeof(wii_i2c_classic_state));
		assert(classic_state);
		ESP_LOGI("main", "-> classic controller detected\n");
		break;
	default:
		ESP_LOGI("main", "-> unknown controller detected: 0x%06x\n", controller_type);
		break;
	}
	wii_i2c_request_state();
}

/*
 * The main loop of the program which:
 * 1. Reads input from the controller
 * 2. Calls the appropriate controller handling function.
 */
void loop()
{
	const unsigned char *data = wii_i2c_read_state();
	wii_i2c_request_state();
	if (data)
	{
		switch (controller_type)
		{
		case WII_I2C_IDENT_NUNCHUK:
			show_nunchuk(data);
			break;
		case WII_I2C_IDENT_CLASSIC:
			show_classic(data);
			break;
		default:
			ESP_LOGI("main", "data: %02x %02x %02x %02x %02x %02x\n",
					 data[0], data[1], data[2], data[3], data[4], data[5]);
			break;
		}
	}
	else
	{
		ESP_LOGI("main", "no data :(\n");
	}

	vTaskDelay(pdMS_TO_TICKS(100));
}

/*
 * Main function.
 */
void app_main()
{
	setup();

	while (true)
	{
		loop();
	}
}
