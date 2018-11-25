#include <Arduino.h>
#include "config.h"

namespace led {

unsigned long last_switch = 0;
bool led_on = false;

void pulse() {
    unsigned long time = millis();
    if (time - last_switch < config::LED_DELAY) return;

    last_switch = time;
    led_on = !led_on;
    digitalWrite(config::LED, led_on ? HIGH : LOW);
}

void on() {
    digitalWrite(config::LED, HIGH);
}

void off() {
    digitalWrite(config::LED, LOW);
}

}