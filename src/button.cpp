#include <Arduino.h>
#include "button.h"

namespace button {

const unsigned long SETTLE_TIME= 50;

Button::ButtonHandler handler = [](Button* button, Event event) {};
void Button::set_handler(Button::ButtonHandler h) {
    handler = h;
}

Button::Button(byte pin, byte tag, byte off_state, Kind kind) {
    this->pin = pin;
    this->tag = tag;
    this->off_state = off_state;
    this->last_reading = off_state;
    this->pressed = false;
    this->kind = kind;
    this->last_reading_change_time = 0;
    pinMode(pin, INPUT_PULLUP);
}

byte Button::get_tag() {
    return this->tag;
}

Kind Button::get_kind() {
    return this->kind;
}

void Button::check() {
    unsigned long current_time = millis();
    bool reading = digitalRead(this->pin) != this->off_state;

    if (current_time - this->last_reading_change_time < SETTLE_TIME) {
        // button is settling. in case the reading changed during that time, prolong (restart) settling
        if (this->last_reading != reading) this->last_reading_change_time = current_time;
    } else if (reading != this->pressed) {
        // not settling, and the state of the button changed. fire event and start settling
        this->last_reading_change_time = current_time;
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