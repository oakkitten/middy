#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <midi.h>
#include <button.h>

#define T midi::Transport
#define K button::Button::Kind

namespace config {
    // led used for indicating play/rec state
    const byte LED = 5;
    const byte LED_DELAY = 100;

    // knobs
    const struct {byte pin; byte cc;} KNOBS[] = {
        {A0, 7},
        {A1, 8}
    };

    // buttons, where tag is transport for click buttons and midi cc for toggle/push buttons
    const struct {byte pin; byte tag; byte off_state; K kind;} BUTTONS[] = {
        {7, T::play,          HIGH, K::click},
        {8, T::stop,          HIGH, K::click},
        {9, T::record_strobe, HIGH, K::click},
        
        {2, 80,               LOW,  K::toggle},
        {3, 81,               HIGH, K::toggle},
        {4, 82,               HIGH, K::push}
    };

    // midi
    const byte MIDI_CHANNEL = 0;
}

#undef T
#undef K

#endif