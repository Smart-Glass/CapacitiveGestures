#include "gestures.h"

/**
 * Generic function to detect a tap based gesture
 * Has paramters to tune time based on preference
 */
bool
Sensor::detectTap(int start_thresh=1000, int end_thresh=2000, int g_type=TAP) {
	if (sens_val > sens_max && !gesture[g_type].gesture_initiated) {
		gesture[g_type].start_time = millis();
		gesture[g_type].gesture_initiated = true;
	} else if (sens_val < sens_min && gesture[g_type].gesture_initiated) {
		gesture[g_type].gesture_initiated = false;
		if (millis() - gesture[g_type].start_time > start_thresh &&
				millis() - gesture[g_type].start_time < end_thresh) {
			gesture[g_type].gesture_initiated = 0;
			return true;
		}
	}
	return false;
}

/**
 * A regular tap
 */
bool
Sensor::tap(int start_thresh=500, int end_thresh=1000) {
	return detectTap(start_thresh, end_thresh, TAP);
}

/**
 * Longer than a regular tap
 */
bool
Sensor::longTap(int start_thresh=1000, int end_thresh=2000) {
	return detectTap(start_thresh, end_thresh, LONGTAP);
}

/**
 * Initialize the sensors
 */
Sensor sensor[] = {
	Sensor(4,2)
};

int size = (sizeof(sensor)/sizeof(*sensor));

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	sensor[0].sens_val = sensor[0].capacitiveSensor(30);

	for (int i=0; i < size; i++){
		if (sensor[i].tap())
			Serial.println("Tap detected");
		 else if (sensor[i].longTap())
			 Serial.println("longTap detected");
	}
}
