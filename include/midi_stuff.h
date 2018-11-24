#ifndef MIDI_STUFF_H
#define MIDI_STUFF_H

#include <Arduino.h>
#include <MIDIUSB.h>

void noteOn(byte channel, byte pitch, byte velocity);

void noteOff(byte channel, byte pitch, byte velocity);

void controlChange(byte channel, byte control, byte value);

void MidiUSB_sendSysEx(const uint8_t *data, size_t size);

void send_transport(byte id);

void print_midi_event(midiEventPacket_t *event);

#endif