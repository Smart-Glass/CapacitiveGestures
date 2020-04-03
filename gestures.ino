/**
 * Initialize the sensors
 */
Sensor sensor[] = {
	Sensor(11, A6, 400, 175),
	Sensor(11, A5, 300, 150),
	Sensor(11, A4, 300, 150),
	Sensor(11, A3, 250, 150),
	Sensor(11, A7, 300, 200),
	Sensor(11, 8, 400, 150),
};

int size = (sizeof(sensor) / sizeof( * sensor));
bool tap_arr[5] = {false};
bool swipe_left = false;
bool swipe_right = false;
bool swipe_up = false;
bool swipe_down = false;
bool long_tap = false;
long start_time = 0;


void setup() {
	Serial.begin(9600);
}

void loop() {
	for (int i = 0; i < size; i++)
		sensor[i].sens_val = sensor[i].capacitiveSensor(30);

	for (int i = 0; i < size; i++) {
		if (sensor[i].tap()) {
			tap_arr[i] = true;
			start_time = millis();
		} else if (sensor[i].longTap()) {
			long_tap = true;
			start_time = millis();
		}
	}
	if (swipeUP(sensor))
		swipe_up = true;
	else if (swipeDown(sensor))
		swipe_down = true;
	if (swipeLeft(sensor))
		swipe_left = true;
	else if (swipeRight(sensor))
		swipe_right = true;
	if (start_time && millis() - start_time > 500) {
		if (swipe_left) {
			Serial.println("Swipe left");
			swipe_left = false;
		} else if (swipe_right) {
			Serial.println("Swipe right");
			swipe_right = false;
		} else if (swipe_up) {
			Serial.println("Swipe up");
			swipe_up = false;
		} else if (swipe_down) {
			Serial.println("Swipe down");
			swipe_down = false;
		} else if (long_tap) {
			Serial.println("long tap");
			long_tap = false;
		} else {
			for (int i = 0; i < size; i++) {
				if (tap_arr[i]) {
					Serial.print("Tap");
					Serial.println(i);
					break;
				}
			}
		}
		memset(tap_arr, 0, sizeof(tap_arr));
		start_time = 0;
	}
}