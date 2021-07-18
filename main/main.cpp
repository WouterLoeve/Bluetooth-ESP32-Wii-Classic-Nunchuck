#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "thumbstick.h"
#include "gamepad.h"

extern "C" {
	void app_main();
}

void app_main() {
	Gamepad *gp = new Gamepad();
	while(true) {
		vTaskDelay(pdMS_TO_TICKS(100));
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