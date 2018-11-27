#include <Arduino.h>
#include "button.h"

namespace button {

const unsigned long SETTLE_TIME = 50;

Button::Handler handler = [](const Button* button, Button::Event event) {};
void Button::set_handler(Button::Handler h) {
    handler = h;
}

Button::Button(byte pin, byte tag, byte off_state, Kind kind) :
            pin(pin), tag(tag), off_state(off_state), kind(kind) {
    this->last_reading = off_state;
    this->pressed = false;
    this->last_reading_change_time = 0;
    pinMode(pin, INPUT_PULLUP);
}

void Button::check() {
    unsigned long now = millis();
    bool reading = digitalRead(this->pin) != this->off_state;

    if (now - this->last_reading_change_time < SETTLE_TIME) {
        // button is settling. in case the reading changed during that time, prolong (restart) settling
        if (this->last_reading != reading) this->last_reading_change_time = now;
    } else if (reading != this->pressed) {
        // not settling, and the state of the button changed. fire event and start settling
        this->last_reading_change_time = now;
        this->pressed = reading;
        this->dispatch_change();
    }
    
    this->last_reading = reading;
}

void Button::dispatch_change() {
    switch (this->kind) {
        case Kind::push:
            handler(this, this->pressed? Event::press : Event::release);
            break;
        case Kind::click:
            if (this->pressed) handler(this, Event::click);
            break;
        case Kind::toggle:
            if (this->pressed) {
                this->toggle_state = !this->toggle_state;
                handler(this, this->toggle_state ? Event::toggle_on : Event::toggle_off);
            }
            break;
    }
}

}