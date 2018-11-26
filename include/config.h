#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <midi.h>
#include <button.h>

namespace config {
    // led used for indicating play/rec state
    const byte LED = 5;
    const byte LED_DELAY = 100;

    // pin → midi cc
    const byte KNOBS[][2] = {
        {A0, 7},
        {A1, 8}
    };

    // pin → transport/cc → released button state → button kind
    const byte BUTTONS[][4] = {
        {7, midi::Transport::play,          HIGH, button::Kind::click},
        {8, midi::Transport::stop,          HIGH, button::Kind::click},
        {9, midi::Transport::record_strobe, HIGH, button::Kind::click},
        
        {2, 80,                             LOW,  button::Kind::toggle},
        {3, 81,                             HIGH, button::Kind::toggle},
        {4, 82,                             HIGH, button::Kind::push}
    };

    // midi
    const byte MIDI_CHANNEL = 0;
}

#endif