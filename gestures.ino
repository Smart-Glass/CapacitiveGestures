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
long start_time = 0;
/*
 *
 * Bitmask to house gestures
 * Bits 0-5  = taps
 * Bits 6-11 = long taps
 * Bit 12    = swipe left
 * Bit 13    = swipe right
 * Bit 14    = swipe up
 * Bit 15    = swipe down
 */
uint16_t g_mask = 0x0;

void setup() {
	Serial.begin(9600);
}

void loop() {
	for (int i = 0; i < size; i++)
		sensor[i].sens_val = sensor[i].capacitiveSensor(30);

	for (int i = 0; i < size; i++) {
		if (sensor[i].tap()) {
			bitSet(g_mask, i);
			start_time = millis();
		} else if (sensor[i].longTap()) {
			bitSet(g_mask, i + 6);
			start_time = millis();
		}
	}
	if (swipeUP(sensor))
		bitSet(g_mask, SWIPE_UP_BIT);
	else if (swipeDown(sensor))
		bitSet(g_mask, SWIPE_DOWN_BIT);
	if (swipeLeft(sensor))
		bitSet(g_mask, SWIPE_LEFT_BIT);
	else if (swipeRight(sensor))
		bitSet(g_mask, SWIPE_RIGHT_BIT);

	if (start_time && millis() - start_time > GESTURE_TIMEOUT) {
		if (bitRead(g_mask, SWIPE_LEFT_BIT)) {
			Serial.println("Swipe left");
		} else if (bitRead(g_mask, SWIPE_RIGHT_BIT)) {
			Serial.println("Swipe right");
		} else if (bitRead(g_mask, SWIPE_UP_BIT)) {
			Serial.println("Swipe up");
		} else if (bitRead(g_mask, SWIPE_DOWN_BIT)) {
			Serial.println("Swipe down");
		} else {
			for (int i = 0; i < size; i++) {
				if (bitRead(g_mask, i + 6)) {
					Serial.print("Long Tap");
					Serial.println(i);
					break;
				} else if (bitRead(g_mask, i)) {
					Serial.print("Tap");
					Serial.println(i);
					break;
				}
			}
		}
		g_mask = 0;
		start_time = 0;
	}
}