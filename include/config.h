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

    // pin → default release state → transport (with flag) or midi cc
    const byte TRANSPORT = 0b10000000;
    const byte BUTTONS[][3] = {
        {7, HIGH, TRANSPORT | midi::Transport::play},
        {8, HIGH, TRANSPORT | midi::Transport::stop},
        {9, HIGH, TRANSPORT | midi::Transport::record_strobe},
        
        {4, LOW,  80},
        {2, HIGH, 81}
    };

    // midi
    const byte MIDI_CHANNEL = 0;
}

#endif