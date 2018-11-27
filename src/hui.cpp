#include <Arduino.h>
#include <MIDIUSB.h>
#include "hui.h"

namespace hui {

byte last;
Result next(byte b) {
    Result result = Result::ignore;
    if      (last == 0x44 && b == 0x05) result = Result::play;
    else if (last == 0x44 && b == 0x45) result = Result::record;
    else if (last == 0x04 && b == 0x05) result = Result::stop;
    last = b;
    return result;
}

Result read(const midiEventPacket_t *data) {
    if (data->header == 0x0b && data->byte1 == 0xb0 && data->byte2 == 0x0c && data->byte3 == 0x0e) return Result::ignore;
    if (data->header != 0x0b || data->byte1 != 0xb0 || data->byte2 != 0x2c) return Result::unknown;

    // first 3 bytes OK
    switch (data->byte3) {
        case 0x03: 
            return Result::ignore;
        case 0x44:
        case 0x05:
        case 0x04:
        case 0x45:
            return next(data->byte3);
        default:
            return Result::unknown;
    }
}

}