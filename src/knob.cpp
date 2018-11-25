#include <Arduino.h>
#include "knob.h"

namespace knob {

const int THRESHOLD = 3;

Knob::KnobHandler handler = [](Knob* knob, int value) {};
void Knob::set_handler(Knob::KnobHandler h) {
    handler = h;
}

Knob::Knob(byte pin, byte cc) {
    this->pin = pin;
    this->cc = cc;
}

byte Knob::get_cc() {
    return this->cc;
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
