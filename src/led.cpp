#include <Arduino.h>

unsigned long led_last_switch = 0;
bool led_on = false;

void pulse() {
    unsigned long time = millis();
    if (time - led_last_switch > 100) {
        Serial.print(time - led_last_switch);
        led_last_switch = time;
        led_on = !led_on;
        digitalWrite(5, led_on ? HIGH : LOW);
    }
}
