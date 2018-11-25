#ifndef MIDI_H
#define MIDI_H

#include <Arduino.h>
#include <MIDIUSB.h>

namespace midi {
    enum Transport {
        stop = 1,
        play = 2,
        fast_forward = 4,
        rewind = 5,
        record_strobe = 6,
        record_exit = 7,
        pause = 9
    };

    void send_note_on(byte channel, byte pitch, byte velocity);
    void send_note_off(byte channel, byte pitch, byte velocity);
    void send_control_change(byte channel, byte control, byte value);
    void send_sysex(const uint8_t *data, size_t size);
    void send_transport(Transport id);
    void println_event(const midiEventPacket_t *event);
    void flush();
}

#endif