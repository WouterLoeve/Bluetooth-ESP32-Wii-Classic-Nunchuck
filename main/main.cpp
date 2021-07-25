#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "thumbstick.h"
#include "BleGamepad.h"

extern "C" {
	void app_main();
}

void app_main() {
	BleGamepad *gp = new BleGamepad();
	gp->begin(16, 1, true, true, false, true, true, false, false, false, false, false, false, false, false);
	while(true) {
		gp->setAxes(32767, 0, 0, 0, 0, 0, 0, 0, DPAD_CENTERED);

		vTaskDelay(pdMS_TO_TICKS(5000));
		gp->setAxes(0, 32767, 0, 0, 0, 0, 0, 0, DPAD_CENTERED);

		vTaskDelay(pdMS_TO_TICKS(5000));
		gp->setAxes(0, 0, 0, 0, 0, 0, 0, 0, DPAD_RIGHT);

		//gp->press(BUTTON_5);
		//gp->press(BUTTON_16);
		//gp->setAxes(32767, 32767, 0, 32767, 32767, 0, 0, 0, DPAD_DOWN_RIGHT);
		// All axes, sliders, hats etc can also be set independently. See the IndividualAxes.ino example
		vTaskDelay(pdMS_TO_TICKS(5000));

		//gp->release(BUTTON_5);
		//gp->setAxes(-32767, -32767, -32767, -32767, -32767, -32767, -32767, -32767, DPAD_CENTERED);
		//vTaskDelay(pdMS_TO_TICKS(500));
	}
}


// TS_THUMBSTICK_ACTION *ts_result = NULL;
// void app_main( void ) {
// 	ts_init();
	
// 	ts_result = (TS_THUMBSTICK_ACTION*) malloc(sizeof(TS_THUMBSTICK_ACTION));
// 	assert(ts_result);
// 	while(true) {
// 		vTaskDelay(pdMS_TO_TICKS(100));
// 		ts_get_input(ts_result);
// 		ESP_LOGD("JOYSTICK", "U:%d D:%d L:%d R:%d", ts_result->UP, ts_result->DOWN, ts_result->LEFT, ts_result->RIGHT);
// 	}
// }