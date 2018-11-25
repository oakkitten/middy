#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <midi.h>

namespace config {
    // led used for indicating play/rec state
    const byte LED = 5;
    const byte LED_DELAY = 100;

    // pin → midi cc
    const byte KNOBS[][2] = {
        {A0, 7},
        {A1, 8}
    };

    // pin → transport
    const byte BUTTONS[][2]  = {
        {7, midi::Transport::play},
        {8, midi::Transport::stop},
        {9, midi::Transport::record_strobe}
    };

    // midi
    const byte MIDI_CHANNEL = 0;
}

#endif