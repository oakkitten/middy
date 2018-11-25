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

void button_handler(AceButton* button, uint8_t eventType, uint8_t state) {
    if (eventType != AceButton::kEventPressed && eventType != AceButton::kEventReleased) return;
    bool pressed = eventType == AceButton::kEventPressed;
    byte id = button->getId();

    if (id & config::TRANSPORT) {
        if (pressed) midi::send_transport(static_cast<midi::Transport>(~config::TRANSPORT & id));
    } else {
        midi::send_control_change(config::MIDI_CHANNEL, id, pressed ? 127 : 0);
    }
}

void knob_handler(Knob* knob, int value) {
    byte midi_value = map(value, 0, 1023, 0, 127);
    midi::send_control_change(config::MIDI_CHANNEL, knob->get_cc(), midi_value);
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

    Knob::set_handler(knob_handler);
    int i = 0; for (auto &pin_cc : config::KNOBS) {
        knobs[i++] = new Knob(pin_cc[0], pin_cc[1]);
    }
    
    buttonConfig.setEventHandler(button_handler);
    i = 0; for (auto &pin_hilo_id : config::BUTTONS) {
        buttons[i] = new AceButton(&buttonConfig);
        buttons[i]->init(pin_hilo_id[0], pin_hilo_id[1], pin_hilo_id[2]);
        pinMode(pin_hilo_id[0], INPUT_PULLUP);
        i++;
    }
}

void loop() {
    process_input();
    check_buttons();
    check_knobs();
    midi::flush();
    delay(1);
}
