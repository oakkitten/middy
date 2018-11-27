#ifndef HUI_H
#define HUI_H

#include "midi.h"

namespace hui {
    enum class Result {
        unknown,
        ignore,
        stop,
        play,
        record
    };

    Result read(const midiEventPacket_t *data);
}

#endif