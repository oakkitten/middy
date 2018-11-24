#include <Arduino.h>
#include <MIDIUSB.h>
#include "hui.h"

byte hui_next(byte b);

const midiEventPacket_t IN_IGNORE = {0x0b, 0xb0, 0x0c, 0x0e};
const midiEventPacket_t VALUABLE = {0x0b, 0xb0, 0x2c, 0x03};

byte read_hui(midiEventPacket_t *data) {
    if (data->header == 0x0b && data->byte1 == 0xb0 && data->byte2 == 0x0c && data->byte3 == 0x0e) return IGNORE;
    if (data->header != 0x0b || data->byte1 != 0xb0 || data->byte2 != 0x2c) return UNKNOWN;

    // first 3 bytes OK
    switch (data->byte3) {
        case 0x03: 
         return IGNORE;
        case 0x44:
        case 0x05:
        case 0x04:
        case 0x45:
            return hui_next(data->byte3);
        default:
            return UNKNOWN;
    }
}

byte last_byte;

byte hui_next(byte b) {
    byte result = IGNORE;
    if (last_byte == 0x44 && b == 0x05) result = PLAY;
    else if (last_byte == 0x44 && b == 0x45) result = REC;
    else if (last_byte == 0x04 && b == 0x05) result = STOP;
    last_byte = b;
    return result;
}
