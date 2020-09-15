#include <Servo.h>

#define QUANTITY_OF_CI 1

#define PIN_DS 8
#define PIN_SH_CP 4
#define PIN_ENABLE 7
#define PIN_ST_CP 12

#define BIT_MOTOR_1_A 2
#define BIT_MOTOR_1_B 3
#define BIT_MOTOR_2_A 1
#define BIT_MOTOR_2_B 4
#define BIT_MOTOR_3_A 5
#define BIT_MOTOR_3_B 7
#define BIT_MOTOR_4_A 0
#define BIT_MOTOR_4_B 6

#define PIN_MOTOR_2_PWM 3
#define PIN_MOTOR_3_PWM 5
#define PIN_MOTOR_4_PWM 6
#define PIN_MOTOR_1_PWM 11

unsigned long serialMaxRotations = 0;

void setup() {
    Serial.begin(9600);

    pinMode(PIN_DS, OUTPUT);
    pinMode(PIN_SH_CP, OUTPUT);
    pinMode(PIN_ST_CP, OUTPUT);
    pinMode(PIN_ENABLE, OUTPUT);

    pinMode(PIN_MOTOR_1_PWM, OUTPUT);
    pinMode(PIN_MOTOR_2_PWM, OUTPUT);
    pinMode(PIN_MOTOR_3_PWM, OUTPUT);
    pinMode(PIN_MOTOR_4_PWM, OUTPUT);

    digitalWrite(PIN_ENABLE, LOW);

    setSerialMaxRotations();

    setMotorStatusByPin(BIT_MOTOR_1_B, LOW);
    setMotorStatusByPin(BIT_MOTOR_1_A, HIGH);

    setMotorStatusByPin(BIT_MOTOR_2_B, LOW);
    setMotorStatusByPin(BIT_MOTOR_2_A, HIGH);

    setMotorStatusByPin(BIT_MOTOR_3_B, LOW);
    setMotorStatusByPin(BIT_MOTOR_3_A, HIGH);

    setMotorStatusByPin(BIT_MOTOR_4_B, LOW);
    setMotorStatusByPin(BIT_MOTOR_4_A, HIGH);
}

void setSerialMaxRotations() {
    while (!Serial.available()) {}
    serialMaxRotations = Serial.read();
}

void setMotorStatusByPin(byte pin, bool status) {
    static byte ciBuffer[QUANTITY_OF_CI];

    bitWrite(ciBuffer[pin / 8], pin % 8, status);

    digitalWrite(PIN_DS, LOW);
    digitalWrite(PIN_SH_CP, LOW);
    digitalWrite(PIN_ST_CP, LOW);

    for (unsigned int nC = QUANTITY_OF_CI - 1; nC >= 0; nC--) {
        for (unsigned int nB = 7; nB >= 0; nB--) {
            digitalWrite(PIN_SH_CP, LOW);
            digitalWrite(PIN_DS, bitRead(ciBuffer[nC], nB));
            digitalWrite(PIN_SH_CP, HIGH);
            digitalWrite(PIN_DS, LOW);
        }
    }

    digitalWrite(PIN_ST_CP, HIGH);
}

void mapValueToAnalogWrite(unsigned long index, unsigned long firstIndex, unsigned long lastIndex) {
    byte mappedValue = map(index, firstIndex, lastIndex, 25, 230);
    analogWrite(PIN_MOTOR_1_PWM, mappedValue);
    analogWrite(PIN_MOTOR_2_PWM, mappedValue);
    analogWrite(PIN_MOTOR_3_PWM, mappedValue);
    analogWrite(PIN_MOTOR_4_PWM, mappedValue);
    delay(1000);
}

void delayPWM(unsigned long maxRotations) {
    for (unsigned long index = 0; index <= maxRotations; index++) {
        mapValueToAnalogWrite(index, 0, maxRotations);
    }

    for (unsigned long index = maxRotations; index >= 0; index--) {
        mapValueToAnalogWrite(index, maxRotations, 0);
    }
}

void loop() {
    delayPWM(serialMaxRotations);
}
