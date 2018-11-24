#include <Arduino.h>
#include <AceButton.h>
#include <MIDIUSB.h>

#include "hui.h"
#include "midi_stuff.h"
#include "led.h"

using namespace ace_button;

ButtonConfig buttonConfig;
AceButton button1(&buttonConfig);
AceButton button2(&buttonConfig);

void buttonEventHandler(AceButton* button, uint8_t eventType, uint8_t state) {
    switch (eventType) {
        case AceButton::kEventPressed:
            uint8_t id = button->getId();
            send_transport(id);
            break;
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(5, OUTPUT);
    pinMode(7, INPUT_PULLUP);
    button1.init(7, HIGH, 2);
    buttonConfig.setEventHandler(buttonEventHandler);
}

int old_btn_value = LOW;
int old_value = 0;
int old_value_ = 0;

byte state;

void loop() {
    midiEventPacket_t event = MidiUSB.read();
    if (event.header != 0) {
        state = read_hui(&event);
        Serial.print("Result: ");
        Serial.println(state);
        print_midi_event(&event);
        if (state == PLAY) digitalWrite(5, HIGH);
        if (state == STOP) digitalWrite(5, LOW);
    }

    if (state == REC) pulse();
    
    button1.check();
    
    int value = analogRead(A0);
    if (abs(value - old_value) > 3) {
        old_value = value;
        int midi_value = map(value, 0, 1023, 0, 127);
        controlChange(0, 7, midi_value);
        Serial.println(midi_value);
        MidiUSB.flush();
    }

    int value_ = analogRead(A9);
    if (abs(value_ - old_value_) > 3) {
        old_value_ = value_;
        int midi_value_ = map(value_, 0, 1023, 0, 127);
        controlChange(0, 8, midi_value_);
        Serial.println(midi_value_);
        MidiUSB.flush();
    }

    
    delay(1);
}
