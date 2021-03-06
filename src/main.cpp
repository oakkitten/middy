#include <Arduino.h>
#include <MIDIUSB.h>

#include "hui.h"
#include "midi.h"
#include "led.h"
#include "config.h"
#include "knob.h"
#include "button.h"

using namespace button;
using namespace knob;

Button *buttons[sizeof(config::BUTTONS)/sizeof(config::BUTTONS[0])];
Knob *knobs[sizeof(config::KNOBS)/sizeof(config::KNOBS[0])];

void button_handler(const Button* button, Button::Event event) {
    switch (button->kind) {
        case Button::Kind::push:
        case Button::Kind::toggle:
            midi::send_control_change(config::MIDI_CHANNEL, button->tag, Button::midi_value(event));
            break;
        case Button::Kind::click:
            midi::send_transport(static_cast<midi::Transport>(button->tag));
            break;
    }
}

void knob_handler(const Knob* knob, int value) {
    byte midi_value = map(value, 0, 1023, 0, 127);
    midi::send_control_change(config::MIDI_CHANNEL, knob->cc, midi_value);
}

bool led_pulsing = false;
void process_input() {
    midiEventPacket_t event = MidiUSB.read();
    if (event.header != 0) {
        hui::Result result = hui::read(&event);
        if (result == hui::Result::unknown) {
            Serial.print("Received: ");
            midi::println_event(&event);
        } 
        else if (result == hui::Result::play) led::on();
        else if (result == hui::Result::stop) led::off();
        led_pulsing = result == hui::Result::record;
    }
    if (led_pulsing) led::pulse();
}

void check_buttons() {
    for (auto &button : buttons) button->check();   
}

void check_knobs() {
    for (auto &knob : knobs) knob->check();
}

///////////////////////////////////////////////////////////////////////////////

void setup() {
    Serial.begin(115200);

    pinMode(config::LED, OUTPUT);

    Knob::set_handler(knob_handler);
    int i = 0; for (auto &k : config::KNOBS) {
        knobs[i++] = new Knob(k.pin, k.cc);
    }
    
    Button::set_handler(button_handler);
    i = 0; for (auto &b : config::BUTTONS) {
        buttons[i++] = new Button(b.pin, b.tag, b.off_state, b.kind);
    }
}

void loop() {
    process_input();
    check_buttons();
    check_knobs();
    midi::flush();
    delay(1);
}
