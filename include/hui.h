#ifndef HUI_H
#define HUI_H

const byte IGNORE = 0;
const byte UNKNOWN = 1;
const byte PLAY = 2;
const byte REC = 3;
const byte STOP = 4;

byte read_hui(midiEventPacket_t *data);

#endif