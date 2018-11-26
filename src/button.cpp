#include <Arduino.h>
#include "button.h"

namespace button {

const unsigned long DEBOUNCE_DELAY = 50;

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
    int reading = digitalRead(this->pin);
    if (reading != this->last_reading) {
        this->last_reading = reading;
        this->last_reading_change_time = millis();
        return;
    }

    if (this->last_reading_change_time == 0) return;

    if (millis() - this->last_reading_change_time > DEBOUNCE_DELAY) {
        bool pressed = reading != this->off_state;
        if (this->pressed == pressed) return;
        this->pressed = pressed;
        this->last_reading_change_time = 0;
        this->dispatch_change();
    }
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