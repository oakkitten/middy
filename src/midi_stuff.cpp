#include <Arduino.h>
#include <MIDIUSB.h>

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).
void noteOn(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOn = {0x09, byte(0x90 | channel), pitch, velocity};
    MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
    midiEventPacket_t noteOff = {0x08, byte(0x80 | channel), pitch, velocity};
    MidiUSB.sendMIDI(noteOff);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).
void controlChange(byte channel, byte control, byte value) {
    midiEventPacket_t event = {0x0B, byte(0xB0 | channel), control, value};
    MidiUSB.sendMIDI(event);
}

void MidiUSB_sendSysEx(const uint8_t *data, size_t size) {
    if (data == NULL || size == 0) return;

    size_t midiDataSize = (size+2)/3*4;
    uint8_t midiData[midiDataSize];
    const uint8_t *d = data;
    uint8_t *p = midiData;
    size_t bytesRemaining = size;

    while (bytesRemaining > 0) {
        switch (bytesRemaining) {
        case 1:
            *p++ = 5;   // SysEx ends with following single byte
            *p++ = *d;
            *p++ = 0;
            *p = 0;
            bytesRemaining = 0;
            break;
        case 2:
            *p++ = 6;   // SysEx ends with following two bytes
            *p++ = *d++;
            *p++ = *d;
            *p = 0;
            bytesRemaining = 0;
            break;
        case 3:
            *p++ = 7;   // SysEx ends with following three bytes
            *p++ = *d++;
            *p++ = *d++;
            *p = *d;
            bytesRemaining = 0;
            break;
        default:
            *p++ = 4;   // SysEx starts or continues
            *p++ = *d++;
            *p++ = *d++;
            *p++ = *d++;
            bytesRemaining -= 3;
            break;
        }
    }
    MidiUSB.write(midiData, midiDataSize);
}

// https://en.wikipedia.org/wiki/MIDI_Machine_Control#MIDI_Universal_Real_Time_SysEx_Message_Format
void send_transport(byte id) {
  uint8_t mmc[6] = {0xf0, 0x7f, 0x7f, 0x06, id, 0xf7};
  MidiUSB_sendSysEx(mmc, 6);
  Serial.println("woot");
  MidiUSB.flush();
}

void print_midi_event(midiEventPacket_t *event) {
    Serial.print("Received: ");
    Serial.print(event->header, HEX);
    Serial.print("-");
    Serial.print(event->byte1, HEX);
    Serial.print("-");
    Serial.print(event->byte2, HEX);
    Serial.print("-");
    Serial.println(event->byte3, HEX);
}
