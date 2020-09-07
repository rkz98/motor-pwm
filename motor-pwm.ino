#define MAX_VALUE_PWM 255
#define TIME_PER_VALUE 300
#define COMPLETE_ROTATION 10

const unsigned int pinPwm = 3;

unsigned int pwmCurrentValue = 0;

void setup() {
	pinMode(pinPwm, OUTPUT);
}

void setAnalogWriteAndDelay() {
	analogWrite(pinPwm, pwmCurrentValue);
	delay(TIME_PER_VALUE);
}

void ascendingOrder() {
	for (pwmCurrentValue = 0; pwmCurrentValue <= MAX_VALUE_PWM; pwmCurrentValue++) {
		setAnalogWriteAndDelay();
	}
}

void descendingOrder() {
	for (pwmCurrentValue = MAX_VALUE_PWM; pwmCurrentValue > 0; pwmCurrentValue--) {
		setAnalogWriteAndDelay();
	}
}

void startRunning() {
	ascendingOrder();
	descendingOrder();
}

void loop() {
	startRunning();
	Serial.println(pwmCurrentValue);
}