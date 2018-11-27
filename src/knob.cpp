#include <Arduino.h>
#include "knob.h"

namespace knob {

const int THRESHOLD = 3;

Knob::Handler handler = [](const Knob* knob, int value) {};
void Knob::set_handler(Knob::Handler h) {
    handler = h;
}

Knob::Knob(byte pin, byte cc) : pin(pin), cc(cc) {
    this->old_value = -100;
}

// don't emit change if value has changed only slightly, but snap to the ends—0 or 1023
void Knob::check() {
    int value = analogRead(this->pin);
    int difference = abs(value - this->old_value);
    if (difference > THRESHOLD || (difference && (value == 0 || value == 1023))) {
        this->old_value = value;
        handler(this, value);
    }
}

}
