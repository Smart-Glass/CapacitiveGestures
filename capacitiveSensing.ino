#include "gestures.h"

/**
  * Design of the sensor => Gesture detection will be tailored for it
  ------------------------------
  |             4              |
  ------------------------------
  |   0  |   1   |  2   |  3   |
  ------------------------------
  |             5              |
  ------------------------------
 */


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
Sensor::tap(int start_thresh=20, int end_thresh=100) {
	return detectTap(start_thresh, end_thresh, TAP);
}

/**
 * Longer than a regular tap
 */
bool
Sensor::longTap(int start_thresh=350, int end_thresh=1000) {
	return detectTap(start_thresh, end_thresh, LONGTAP);
}

bool
detectHorizontalSwipe(Sensor *sensor, int swipe, int start_thresh=50,
                      int end_thresh=300) {
  // Check for initialization only from sensors 0 and 1
  int sens1_idx = 0;
  int sens2_idx = 1;
  int sens3_idx = 3;

  if (swipe == SWIPE_RIGHT) {
    sens1_idx = 3;
    sens2_idx = 2;
    sens3_idx = 0;
  }
  int idx = sens1_idx;

  if (!sensor[sens1_idx].gesture[swipe].gesture_initiated &&
      !sensor[sens2_idx].gesture[swipe].gesture_initiated){
    if (sensor[sens1_idx].sens_val < sensor[sens2_idx].sens_val) {
      idx = sens2_idx;
    }
    if (sensor[idx].sens_val < sensor[idx].sens_min)
      return false;
    if (sensor[idx].sens_val > sensor[idx].sens_max) {
      sensor[idx].gesture[swipe].gesture_initiated = true;
      sensor[idx].gesture[swipe].start_time = millis();
    }
    // Serial.print("BEFORE::Sensor");
    // Serial.print(idx);
    // Serial.print(" start_time:");
    // Serial.println(sensor[idx].gesture[swipe].start_time);
  } else {
    if (sensor[sens2_idx].gesture[swipe].gesture_initiated)
      idx = sens2_idx;
    else if (sensor[sens1_idx].gesture[swipe].gesture_initiated)
      idx = sens1_idx;
    else
      return false;
    // Took too much / too little time for gesture to complete. Bail out
    if (millis() - sensor[idx].gesture[swipe].start_time > end_thresh ||
        millis() - sensor[idx].gesture[swipe].start_time < start_thresh) {
      sensor[idx].gesture[swipe].gesture_initiated = false;
      // Serial.println("Bailing out. Time constraint");
      return false;
    }
    if (sensor[idx].sens_val < sensor[idx].sens_min &&
        sensor[sens3_idx].sens_val > sensor[sens3_idx].sens_max) {
      sensor[idx].gesture[swipe].gesture_initiated = false;
      return true;
    }
    // Serial.print("AFTER::Sensor");
    // Serial.print(idx);
    // Serial.print(" sen3_idx_val:");
    // Serial.println(sensor[sens3_idx].sens_val);
  }
  return false;
}

bool
detectVerticalSwipe(Sensor *sensor, int swipe, int start_thresh=50,
                    int end_thresh=300) {
  int idx = 4;
  int idx1 = 5;
  if (swipe == SWIPE_UP) {
    idx = 5;
    idx1 = 4;
  }
  if (!sensor[idx].gesture[swipe].gesture_initiated) {
    if (sensor[idx].sens_val < sensor[idx].sens_max)
      return false;
    sensor[idx].gesture[swipe].gesture_initiated = true;
    sensor[idx].gesture[swipe].start_time = millis();
  } else {
    // Took too much / too little time for gesture to complete. Bail out
    if (millis() - sensor[idx].gesture[swipe].start_time > end_thresh ||
        millis() - sensor[idx].gesture[swipe].start_time < start_thresh) {
      sensor[idx].gesture[swipe].gesture_initiated = false;
      return false;
    }
    if (sensor[idx].sens_val < sensor[idx].sens_min &&
        sensor[idx1].sens_val > sensor[idx].sens_max) {
      sensor[idx].gesture[swipe].gesture_initiated = false;
      return true;
    }
  }
  return false;
}

bool swipeLeft(Sensor *sensor, int start_thresh=101, int end_thresh=1000) {
  return detectHorizontalSwipe(sensor, SWIPE_LEFT, start_thresh, end_thresh);
}

bool swipeRight(Sensor *sensor, int start_thresh=200, int end_thresh=1000) {
  return detectHorizontalSwipe(sensor, SWIPE_RIGHT, start_thresh, end_thresh);
}

bool swipeUP(Sensor *sensor, int start_thresh=200, int end_thresh=1000) {
  return detectVerticalSwipe(sensor, SWIPE_UP, start_thresh, end_thresh);
}

bool swipeDown(Sensor *sensor, int start_thresh=200, int end_thresh=1000) {
  return detectVerticalSwipe(sensor, SWIPE_DOWN, start_thresh, end_thresh);
}


/**
 * Initialize the sensors - Fictional numbers
 */
 /*
  * S/W Mapping -> Layout
  * 0->4
  * 1->3
  * 2->2
  * 3->1
  * 4->6
  * 5->5
  */
Sensor sensor[] = {
	Sensor(11,A6,400,175),
  Sensor(11,A5,300,150),
  Sensor(11,A4,300,150),
  Sensor(11,A3,250,150),
  Sensor(11,A7,300,200),
  Sensor(11,8,400,150),
};

int size = (sizeof(sensor)/sizeof(*sensor));

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	for (int i = 0; i < size; i++)
    sensor[i].sens_val = sensor[i].capacitiveSensor(30);

	for (int i=0; i < size; i++){
    if (sensor[i].tap())
			Serial.println("Tap detected");
		 else if (sensor[i].longTap())
			 Serial.println("longTap detected");
	}
  // if (swipeUP(sensor))
  //   Serial.println("SwipeUp detected");
  // else if (swipeDown(sensor))
  //   Serial.println("SwipeDown detected");
  if (swipeLeft(sensor))
    Serial.println("SwipeLeft detected");
  // else if (swipeRight(sensor))
  //   Serial.println("SwipeRight detected");
}
