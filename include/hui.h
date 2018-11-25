#ifndef HUI_H
#define HUI_H

#include "midi_stuff.h"

namespace hui {
    enum Result {
        unknown,
        ignore,
        stop,
        play,
        record
    };

    Result read(midiEventPacket_t *data);
}

#endif