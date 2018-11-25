#include <Arduino.h>
#include <AceButton.h>
#include <MIDIUSB.h>

#include "hui.h"
#include "midi.h"
#include "led.h"
#include "config.h"
#include "knob.h"

using namespace ace_button;
using namespace knob;

ButtonConfig buttonConfig;

AceButton *buttons[sizeof(config::BUTTONS)/sizeof(config::BUTTONS[0])];
Knob *knobs[sizeof(config::KNOBS)/sizeof(config::KNOBS[0])];

bool midi_sent;

void button_handler(AceButton* button, uint8_t eventType, uint8_t state) {
    if (eventType != AceButton::kEventPressed) return;
    midi::Transport id = static_cast<midi::Transport>(button->getId());
    Serial.println(id);
    midi::send_transport(id);
    midi_sent = true;
}

void knob_handler(Knob* knob, int value) {
    byte midi_value = map(value, 0, 1023, 0, 127);
    Serial.println(midi_value);
    midi::send_control_change(config::MIDI_CHANNEL, knob->get_cc(), midi_value);
    midi_sent = true;
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
    for (auto const& button : buttons) button->check();   
}

void check_knobs() {
    for (auto const& knob : knobs) knob->check();
}

///////////////////////////////////////////////////////////////////////////////

void setup() {
    Serial.begin(115200);

    pinMode(config::LED, OUTPUT);

    int i = 0;
    for (auto &knob_cc : config::KNOBS) {
        knobs[i++] = new Knob(knob_cc[0], knob_cc[1]);
    }
    Knob::set_handler(knob_handler);
    
    i = 0;
    for (auto &button_transport : config::BUTTONS) {
        buttons[i] = new AceButton(&buttonConfig);
        buttons[i]->init(button_transport[0], HIGH, button_transport[1]);
        pinMode(button_transport[0], INPUT_PULLUP);
        i++;
    }
    buttonConfig.setEventHandler(button_handler);
}

void loop() {
    midi_sent = false;
    process_input();
    check_buttons();
    check_knobs();
    if (midi_sent) MidiUSB.flush();
    delay(1);
}
