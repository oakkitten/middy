#ifndef HUI_H
#define HUI_H

#include "midi.h"

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